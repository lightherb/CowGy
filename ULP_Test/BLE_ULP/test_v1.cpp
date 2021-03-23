// 555µA by disabling USB CDC feature:
// Warning: This interferes with upload of the next sketch 
// to the board. I have to press reset twice in about 1s 
// when the IDE is uploading to make it work.

#include <Arduino.h>
// override the default main function to remove USB CDC feature
int main(void)
{
  init();
  initVariant();

//remove USB CDC feature
//#if defined(SERIAL_CDC)
//  PluggableUSBD().begin();
//  SerialUSB.begin(115200);
//#endif

  setup();

  for (;;) {
    loop();
    if (arduino::serialEventRun) arduino::serialEventRun();
  }

  return 0;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_PWR, LOW); // turn off power LED
  digitalWrite(PIN_ENABLE_SENSORS_3V3, LOW); // turn off sensors
}


void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000); // 5s for easier current measurment
  digitalWrite(LED_BUILTIN, LOW);
  delay(5000);
}
