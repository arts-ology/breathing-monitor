#include <SPI.h>
#include "BMI160Gen.h"   // SparkFun BMI160 library — matches bmi160_test.ino

// ---- Pin config (match your Day 1 SPI wiring) ----
#define BMI160_CS_PIN 4  // chip-select pin — set to whatever you wired

// ---- Tick timing ----
const unsigned long TICK_INTERVAL_MS = 20;   // 50 Hz. Change this one number to retune rate.
unsigned long lastTickMs = 0;

void setup() {
  Serial.begin(115200); 
  //while (!Serial) { ; } // Wait for serial port to connect. Needed for native USB

  Serial.println("Step 1: Serial started");

  SPI.begin();
  Serial.println("Step 2: SPI.begin() done");

  pinMode(BMI160_CS_PIN, OUTPUT);
  digitalWrite(BMI160_CS_PIN, HIGH); // CS idle high
  Serial.println("Step 3: CS pin set up");

  Serial.println("Step 4: about to call BMI160.begin()...");
  BMI160.begin(BMI160GenClass::SPI_MODE, BMI160_CS_PIN);
  Serial.println("Step 5: BMI160.begin() returned!");

  Serial.println("Firmware skeleton up. Starting tick loop...");
}

void loop() {
  unsigned long now = millis();

  if (now - lastTickMs >= TICK_INTERVAL_MS) {
    lastTickMs = now;
    tick();
  }
}

void tick() {
  readAndPrintIMU();
}

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
