//--20210317 bird
//--สามารถต่อมือถือกับแอป
//--ดูค่า acc ทั้ง serial monitor and app

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

//use in platformio
void BirdAccelerometer1();

//------------------------------------------------
void setup()
{
 
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

//---------------------------------------------------
void loop() {

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

//-----------------------------------------------------------
void BirdAccelerometer1 ()
{

  if (IMU.accelerationAvailable())
  {
    IMU.readAcceleration(AccX, AccY, AccZ);
    //---------------------new
    
    float angleLevel1 = 183 + ((atan2(AccX, AccY) * 180) / PI); //cal +3
    float angleLevel;
    int angleint;

    angleint = int(angleLevel1);
    angleLevel = angleint;

    EAngle.writeValue(angleLevel);
    Serial.println(angleLevel);

  }

}
