
#include <LoRa.h>

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);
#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6
void setup() {
    //add for tpl5110
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
  // put your setup code here, to run once:
  Serial.begin(9600);
  LoRa.setPins(SS, RST, DI0);
  Serial.println("LoRa Sender");
    if (!LoRa.begin(BAND)) {
      Serial.println("Starting LoRa failed!");
      while (1);
      }
  Serial.println("LoRa Initial OK!");
  Wire.begin(21,22);
  mpu6050.begin();
  Serial.println("CalgyroOffsets");
  mpu6050.calcGyroOffsets(true);
}

void birdrun() {
  // put your main code here, to run repeatedly:
  mpu6050.update();
  LoRa.beginPacket();
  LoRa.print("TgBird;");
  LoRa.print("Temp:");
  LoRa.print(mpu6050.getTemp());
  LoRa.print(";accX:");
  LoRa.print(mpu6050.getAccX());
  LoRa.print(";gyroX:");
  LoRa.print(mpu6050.getGyroX());
  LoRa.print(";accAngX:");
  LoRa.print(mpu6050.getAccAngleX());
  LoRa.print(";AngX:");
  LoRa.print(mpu6050.getAngleX());

  LoRa.print(";accY:");
  LoRa.print(mpu6050.getAccY());
  LoRa.print(";gyroY:");
  LoRa.print(mpu6050.getGyroY());
  LoRa.print(";accAngY:");
  LoRa.print(mpu6050.getAccAngleY());
  LoRa.print(";AngY:");
  LoRa.print(mpu6050.getAngleY());

  LoRa.print(";accZ:");
  LoRa.print(mpu6050.getAccZ());
  LoRa.print(";gyroZ:");
  LoRa.print(mpu6050.getGyroZ());

  LoRa.print(";AngZ:");
  LoRa.print(mpu6050.getAngleZ());
  LoRa.print(";!");
  LoRa.endPacket();


  Serial.print("TgBird;");
  Serial.print("Temp:");
  Serial.print(mpu6050.getTemp());
  Serial.print(";accX:");
  Serial.print(mpu6050.getAccX());
  Serial.print(";gyroX:");
  Serial.print(mpu6050.getGyroX());
  Serial.print(";accAngX:");
  Serial.print(mpu6050.getAccAngleX());
  Serial.print(";AngX:");
  Serial.print(mpu6050.getAngleX());

  Serial.print(";accY:");
  Serial.print(mpu6050.getAccY());
  Serial.print(";gyroY:");
  Serial.print(mpu6050.getGyroY());
  Serial.print(";accAngY:");
  Serial.print(mpu6050.getAccAngleY());
  Serial.print(";AngY:");
  Serial.print(mpu6050.getAngleY());

  Serial.print(";accZ:");
  Serial.print(mpu6050.getAccZ());
  Serial.print(";gyroZ:");
  Serial.print(mpu6050.getGyroZ());
  
  Serial.print(";AngZ:");
  Serial.println(mpu6050.getAngleZ());
  delay(2000);
}

void loop(){
    birdrun();
    birdrun();
    birdrun();
    Serial.println("go to sleep");
    digitalWrite(13, HIGH);

}