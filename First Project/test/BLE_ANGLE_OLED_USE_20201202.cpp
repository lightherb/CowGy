// LCD Lib
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // pixel ความกว้าง
#define SCREEN_HEIGHT 64 // pixel ความสูง 
// กำหนดขาต่อ I2C กับจอ OLED
#define OLED_RESET -1 //ขา reset เป็น -1 ถ้าใช้ร่วมกับขา Arduino reset
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int var = 0; // ตัวแปรสำหรับทดสอบแสดงผล

// BLE Lib
#include <ArduinoBLE.h> // library bluetooth
#include <Arduino_LSM9DS1.h> // library accellerometer

const int UPDATE_FREQUENCY = 2000; // Update frequency in ms
long previousMillis = 0;  // last time readings were checked, in ms

//dichiaro servizio bluetooth
BLEService TotalService ("a146782a-0a2e-11eb-adc1-0242ac120002"); // BLE TotalService

// BLE - custom 128-bit UUID, read/write central
BLEFloatCharacteristic EAngle("fb89ca42-0a44-11eb-adc1-0242ac120002", BLERead | BLENotify);

//variable
float AccX = 0;
float AccY = 0;
float AccZ = 0;

//variable for millis
unsigned long time3;
unsigned long deltatime3;

//---___------***---___------***---___------***
void setup()
{
  //open serial comunication
  Serial.begin(9600);
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("ArdinoAll OLED Start Work !!!");
  }

  //Nano 33 BLE and sensors: ok
  if (!BLE.begin())
  {
    Serial.println("starting BLE failed!");
    while (1);
  }

  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("E-ANGLE");
  BLE.setAdvertisedService(TotalService);

  // add the characteristic to the service
  TotalService.addCharacteristic(EAngle);

  // add service
  BLE.addService(TotalService);

  // set the initial value for the characeristic:
  EAngle.writeValue(0.00);

  // start advertising
  BLE.advertise();
  //  Serial.println("Ready to start");
}

//---___------***---___------***---___------***
void loop() {

  //End OLED loop

  //list central
  BLEDevice central = BLE.central();

  //central is connected
  if (central)
  {
    Serial.print("Connected to central: ");
    //print MAC of central
    Serial.println(central.address());

    //while central is connected
    while (central.connected())
    {
      //call function
      long currentMillis = millis();
      if (currentMillis - previousMillis >= UPDATE_FREQUENCY) {
        previousMillis = currentMillis;
        BirdAccelerometer1 ();
      }
    }

    //print note when central is disconnected
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
  else {
    Serial.println("not connect monitor");
    BirdAccelerometer1 ();
  }
}

//---___------***---___------***---___------***
void BirdAccelerometer1 ()
{

  if (IMU.accelerationAvailable())
  {
    IMU.readAcceleration(AccX, AccY, AccZ);
    //---------------------new
//    float angleLevel1 = 180 + ((atan2(AccX, AccY) * 180) / PI); //now 20201027
    float angleLevel1 = 183 + ((atan2(AccX, AccY) * 180) / PI); //cal +3
//    float angleLevel1 = ((atan2(AccX, AccY) * 180) / PI);
    float angleLevel;
    int angleint;
//    angleint = (int (angleLevel1 * 100));
//    angleLevel = float(angleint) / 100;
    angleint = int(angleLevel1);
    angleLevel = angleint;

    EAngle.writeValue(angleLevel);
    Serial.println(angleLevel);
    OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
    OLED.setTextColor(WHITE, BLACK); //กำหนดข้อความสีขาว ฉากหลังสีดำ
    OLED.setCursor(0, 0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
    OLED.setTextSize(2); // กำหนดขนาดตัวอักษร
    OLED.println("ANGLE:"); // แสดงผลข้อความ ALL
    OLED.setCursor(0, 30);
    OLED.setTextSize(4);
    OLED.println(angleLevel); // แสดงผลข้อความ ALL
    OLED.display(); // สั่งให้จอแสดงผล
    var++;
    delay(200);
  }

}
void BirdOLED ()
{
  //OLED loop
  float xxx = 12.345;
  OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
  OLED.setTextColor(WHITE, BLACK); //กำหนดข้อความสีขาว ฉากหลังสีดำ
  OLED.setCursor(0, 0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
  OLED.setTextSize(2); // กำหนดขนาดตัวอักษร
  OLED.println("ANGLE:"); // แสดงผลข้อความ ALL
  OLED.setCursor(0, 30);
  OLED.setTextSize(4);
  OLED.println(xxx); // แสดงผลข้อความ ALL
  OLED.display(); // สั่งให้จอแสดงผล
  var++;
  delay(200);
}