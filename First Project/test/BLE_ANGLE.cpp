

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

 // BLE angle Service
BLEService angleService("a146782a-0a2e-11eb-adc1-0242ac120002");

// BLE angle Level Characteristic
BLEFloatCharacteristic angleLevelChar("fb89ca42-0a44-11eb-adc1-0242ac120002",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

int oldangleLevel = 0;  // last angle level reading from analog input
long previousMillis = 0;  // last time the angle level was checked, in ms

void setup() {
  Serial.begin(9600);    // initialize serial communication
//  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("angleMonitor");
  BLE.setAdvertisedService(angleService); // add the service UUID
  angleService.addCharacteristic(angleLevelChar); // add the angle level characteristic
  BLE.addService(angleService); // Add the angle service
  angleLevelChar.writeValue(oldangleLevel); // set initial value for this characteristic

  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    // check the angle level every 200ms
    // while the central is connected:
    while (central.connected()) {
      long currentMillis = millis();
      // if 200ms have passed, check the angle level:
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        updateangleLevel();
      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void updateangleLevel() {
  /* Read the current voltage level on the A0 analog input pin.
     This is used here to simulate the charge level of a angle.
  */

  float x, y, z;
  IMU.readAcceleration(x, y, z);
  float angleLevel = 180-((atan2(x,y) * 180) / PI);

  if (angleLevel != oldangleLevel) {      // if the angle level has changed
    Serial.print("angle Level % is now: "); // print it
    Serial.println(angleLevel);
    angleLevelChar.writeValue(angleLevel);  // and update the angle level characteristic
    oldangleLevel = angleLevel;           // save the level for next comparison
  }
}