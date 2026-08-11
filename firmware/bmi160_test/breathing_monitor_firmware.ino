

#include <SPI.h>


#define BMI160_CS_PIN 5   

const unsigned long TICK_INTERVAL_MS = 20;   

void setup() {
  Serial.begin(115200);

  SPI.begin();
  pinMode(BMI160_CS_PIN, OUTPUT);
  digitalWrite(BMI160_CS_PIN, HIGH); 

  Serial.println("Firmware skeleton up. Starting tick loop...");
}

void loop() {
  unsigned long now = millis();

  if (now - lastTickMs >= TICK_INTERVAL_MS) {
    lastTickMs = now;
    tick();
  }



void tick() {
  readAndPrintAccel();

}

void readAndPrintAccel() {
  int16_t ax, ay, az;

  ax = 0; ay = 0; az = 0;  

  Serial.print("AX: "); Serial.print(ax);
  Serial.print("  AY: "); Serial.print(ay);
  Serial.print("  AZ: "); Serial.println(az);
}
