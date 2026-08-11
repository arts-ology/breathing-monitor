#include "BMI160Gen.h"

const int csPin = 5;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Initializing BMI160... if you can read this send hlep");
  BMI160.begin(BMI160GenClass::SPI_MODE, csPin);

  uint8_t devId = BMI160.getDeviceID();
  Serial.print("Device ID: ");
  Serial.println(devId, HEX);

  BMI160.setGyroRate(100);
  BMI160.setAccelerometerRate(100);
  BMI160.setFullScaleGyroRange(BMI160_GYRO_RANGE_250);
  BMI160.setFullScaleAccelRange(BMI160_ACCEL_RANGE_2G);
}

void loop() {
  int gxRaw, gyRaw, gzRaw;
  int axRaw, ayRaw, azRaw;

  BMI160.readGyro(gxRaw, gyRaw, gzRaw);
  BMI160.readAccelerometer(axRaw, ayRaw, azRaw);

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

  delay(10);
}