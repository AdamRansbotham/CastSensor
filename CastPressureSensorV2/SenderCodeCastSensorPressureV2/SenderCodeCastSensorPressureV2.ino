#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


const char* DEVICE_NAME = "SENDER1"; // Change to SENDER2 on second ESP32

// UUIDs (must match on both sides)
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcdefab-1234-5678-90ab-abcdefabcdef"

BLECharacteristic *pCharacteristic;

void setup() {
  Serial.begin(115200);

  // Init BLE
  BLEDevice::init(DEVICE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
  Serial.println("BLE service started");
}

void loop() {
  // Generate or update values to send
  int a = random(0, 100);
  int b = random(0, 100);
  int c = random(0, 100);

  // Format as comma-separated string
  char buf[32];
  snprintf(buf, sizeof(buf), "%d,%d,%d", a, b, c);
  pCharacteristic->setValue(buf);
  pCharacteristic->notify();  // Push update

  Serial.println("Sent: " + String(buf));
  delay(1000); // 1 second update rate
}
