

#include <SPI.h>
#include "BMI160Gen.h"   // SparkFun BMI160 library — matches bmi160_test.ino

// ---- Pin config (match your Day 1 SPI wiring) ----
#define BMI160_CS_PIN 5   // chip-select pin — set to whatever you wired

// ---- Tick timing ----
const unsigned long TICK_INTERVAL_MS = 20;   // 50 Hz. Change this one number to retune rate.
unsigned long lastTickMs = 0;

void setup() {
  Serial.begin(115200);
  // NOTE: deliberately no "while (!Serial);" here — Day 2 quick fix.
  // That line was what forced a manual reset every session; without it
  // the board just starts running as soon as it has power.

  SPI.begin();
  pinMode(BMI160_CS_PIN, OUTPUT);
  digitalWrite(BMI160_CS_PIN, HIGH); // CS idle high

  BMI160.begin(BMI160GenClass::SPI_MODE, BMI160_CS_PIN);

  Serial.println("Firmware skeleton up. Starting tick loop...");
}

void loop() {
  unsigned long now = millis();

  if (now - lastTickMs >= TICK_INTERVAL_MS) {
    lastTickMs = now;
    tick();
  }

  // Nothing else should block here — this is what keeps the loop free
  // to add BLE/filter work later without them stepping on each other.
}

// Runs once per tick, at TICK_INTERVAL_MS rate.
void tick() {
  readAndPrintIMU();

  // filterTick();   // Day 3
  // bleTick();      // Day 4
  // alertTick();    // Day 6
}

// Reads raw accel + gyro values and prints them as CSV, same format
// as bmi160_test.ino — just now driven by the tick loop instead of delay(10).
void readAndPrintIMU() {
  int axRaw, ayRaw, azRaw;
  int gxRaw, gyRaw, gzRaw;

  BMI160.readAccelerometer(axRaw, ayRaw, azRaw);
  BMI160.readGyro(gxRaw, gyRaw, gzRaw);

  Serial.print(axRaw);
  Serial.print(",");
  Serial.print(ayRaw);
  Serial.print(",");
  Serial.print(azRaw);
  Serial.print(",");
  Serial.print(gxRaw);
  Serial.print(",");
  Serial.print(gyRaw);
  Serial.print(",");
  Serial.println(gzRaw);
}
