#include <SPI.h>
#include "BMI160Gen.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define BMI160_CS_PIN 5

const unsigned long TICK_INTERVAL_MS = 20;
unsigned long lastTickMs = 0;

float filteredValue = 0;
bool filterInitialized = false;

float lastFilteredValue = 0;
bool rising = false;
int breathCount = 0;

#define SERVICE_UUID        "7A079255-3084-41FF-8CD6-41D6A1EED566"
#define CHARACTERISTIC_UUID "BE6797CE-DF28-416A-980A-B756A08AA350"

BLECharacteristic *breathingCharacteristic;
bool deviceConnected = false;

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* server) { deviceConnected = true; }
  void onDisconnect(BLEServer* server) { deviceConnected = false; }
};

void setupBLE() {
  BLEDevice::init("BreathingMonitor"); //names the device 
  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());
//creates the server ^^
  BLEService *service = server->createService(SERVICE_UUID);

  breathingCharacteristic = service->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  ); //creates teh folder and characteristic ^^
  breathingCharacteristic->addDescriptor(new BLE2902());

  service->start();

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID);
  advertising->start();

  Serial.println("BLE advertising started as 'BreathingMonitor'");
}

void bleTick(float valueToSend) {
  if (deviceConnected) {
    char buf[10];
    dtostrf(valueToSend, 4, 1, buf);
    breathingCharacteristic->setValue(buf);
    breathingCharacteristic->notify();
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("Step 1: Serial started");

  SPI.begin();
  Serial.println("Step 2: SPI.begin() done");

  pinMode(BMI160_CS_PIN, OUTPUT);
  digitalWrite(BMI160_CS_PIN, HIGH);
  Serial.println("Step 3: CS pin set up");

  Serial.println("Step 4: about to call BMI160.begin()...");
  BMI160.begin(BMI160GenClass::SPI_MODE, BMI160_CS_PIN);
  Serial.println("Step 5: BMI160.begin() returned!");

  setupBLE();

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

  filterTick((float)axRaw);
  peakTick(filteredValue);

  float fakeRate = 15.0 + sin(millis() / 2000.0) * 3.0;
  bleTick(fakeRate);

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
  Serial.print(gzRaw);
  Serial.print(",");
  Serial.println(filteredValue);
}

void filterTick(float rawValue) {
  if (!filterInitialized) {
    filteredValue = rawValue;
    filterInitialized = true;
    return;
  }
  float alpha = 0.90;
  filteredValue = alpha * filteredValue + (1 - alpha) * rawValue;
}

void peakTick(float currentFiltered) {
  if (currentFiltered > lastFilteredValue) {
    rising = true;
  } else if (currentFiltered < lastFilteredValue && rising) {
    breathCount++;
    rising = false;
    Serial.print("Breath detected! Count: ");
    Serial.println(breathCount);
  }
  lastFilteredValue = currentFiltered;
}