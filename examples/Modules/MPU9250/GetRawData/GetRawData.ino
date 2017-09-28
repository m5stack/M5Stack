#define MPU9250_INSDE
#include <M5Stack.h>

MPU9250 mpusensor;

void setup() {

  M5.begin();
  Serial.println("MPU9250 begin Start!");

  // Start the MPU9250
  Wire.begin();
  mpusensor.setWire(&Wire);
  mpusensor.beginAccel();
  mpusensor.beginMag();

  // you can set your own offset for mag values
  // mpusensor.magXOffset = -50;
  // mpusensor.magYOffset = -55;
  // mpusensor.magZOffset = -10;
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(GREEN, BLACK);
  M5.Lcd.setTextSize(2);
}

void loop() {

  // Get the Accelerometer data
  mpusensor.accelUpdate();
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.println("print accel values");
  M5.Lcd.printf("accelX: %04.2f\r\n", mpusensor.accelX());
  M5.Lcd.printf("accelY: %04.2f\r\n", mpusensor.accelY());
  M5.Lcd.printf("accelZ: %04.2f\r\n", mpusensor.accelZ());
  M5.Lcd.printf("accelSqrt: %4.2f\r\n", mpusensor.accelSqrt());

  // Get the Magnetometer data
  mpusensor.magUpdate();
  M5.Lcd.println("");
  M5.Lcd.println("print mag values");
  M5.Lcd.printf("magX: %03d\r\n", mpusensor.magX());
  M5.Lcd.printf("maxY: %03d\r\n", mpusensor.magY());
  M5.Lcd.printf("magZ: %03d\r\n", mpusensor.magZ());
  M5.Lcd.printf("horizontal dirt: %04.2f\r\n", mpusensor.magHorizDirection());
  M5.Lcd.println("at " + String(millis()) + "ms");
  delay(10);
}
