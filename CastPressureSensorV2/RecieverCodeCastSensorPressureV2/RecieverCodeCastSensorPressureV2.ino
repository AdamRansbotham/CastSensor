#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEClient.h>
#include <BLEAddress.h>
#include <BLEUtils.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcdefab-1234-5678-90ab-abcdefabcdef"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Values from sender 1 & 2
int a1 = 0, b1 = 0, c1 = 0;
int a2 = 0, b2 = 0, c2 = 0;

BLEClient *client1;
BLEClient *client2;
BLERemoteCharacteristic* char1;
BLERemoteCharacteristic* char2;

// Callback for SENDER1
static void notifyCallback1(BLERemoteCharacteristic* c, uint8_t* data, size_t length, bool isNotify) {
  String str = String((char*)data).substring(0, length);
  sscanf(str.c_str(), "%d,%d,%d", &a1, &b1, &c1);
}

// Callback for SENDER2
static void notifyCallback2(BLERemoteCharacteristic* c, uint8_t* data, size_t length, bool isNotify) {
  String str = String((char*)data).substring(0, length);
  sscanf(str.c_str(), "%d,%d,%d", &a2, &b2, &c2);
}

bool connectToSender(const char* name, BLEClient*& client, BLERemoteCharacteristic*& characteristic, void (*callback)(BLERemoteCharacteristic*, uint8_t*, size_t, bool)) {
  BLEScan* scan = BLEDevice::getScan();
  BLEScanResults results = *scan->start(5); // Dereference the pointer


  for (int i = 0; i < results.getCount(); i++) {
    BLEAdvertisedDevice dev = results.getDevice(i);
    if (dev.getName() == name) {
      Serial.println("Found " + String(name));
      client = BLEDevice::createClient();
      client->connect(&dev);
      BLERemoteService* service = client->getService(SERVICE_UUID);
      characteristic = service->getCharacteristic(CHARACTERISTIC_UUID);
      if (characteristic->canNotify()) {
        characteristic->registerForNotify(callback);
      }
      return true;
    }
  }
  Serial.println("Could not find " + String(name));
  return false;
}

void setup() {
  Serial.begin(115200);
  BLEDevice::init("RECEIVER");
  
  BLEScan* scan = BLEDevice::getScan();
  scan->setActiveScan(true);
  BLEScanResults results = *scan->start(5);

  Serial.printf("Found %d devices\n", results.getCount());
  for (int i = 0; i < results.getCount(); i++) {
    BLEAdvertisedDevice dev = results.getDevice(i);
    Serial.print("Device ");
    Serial.print(i);
    Serial.print(": Name = ");
    Serial.print(dev.getName().c_str());
    Serial.print(", Address = ");
    Serial.println(dev.getAddress().toString().c_str());
  }

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  while (!connectToSender("SENDER1", client1, char1, notifyCallback1)){
    delay(100);
    }
  //while (!connectToSender("SENDER2", client2, char2, notifyCallback2)){
    //delay(100);
    //}
}
static void displayText(int weightOne,int weightTwo){
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println(String(weightOne));


  display.setCursor(0, 20);
  display.println("Lbs");

  
  int spacing=0;
  if(weightTwo>99){
    spacing=6*2*3;
  }
  else if(weightTwo>9){
    spacing=6*2*2;
  }
  else{
    spacing=6*2;
  }
  display.setCursor(display.width()-spacing, 0);
  display.println(String(weightTwo));

  display.setCursor(display.width()-36, 20);
  display.println("Lbs");
  
  if(weightOne!=0){
    int percentDifference=1;
    if(weightOne>weightTwo){
      percentDifference=((weightOne-weightTwo)*100/weightOne);
    }
    else{
      percentDifference=((weightTwo-weightOne)*100/weightTwo);
    }
    int percentDifferenceSpacing=(log10(percentDifference)+1)*6;
    display.setCursor(display.width()/2-percentDifferenceSpacing, 50);
    display.print(percentDifference);
    display.print("%");
  }
  display.display();
}
void loop() {
  displayText(left,right);
  

  delay(500);
}
