/* 
  M5Stack Fire
    
  MPU9250 Basic Example Code
  
  by: Kris Winer
  date: April 1, 2014
  license: Beerware - Use this code however you'd like. If you
  find it useful you can buy me a beer some time.
  Modified by Brent Wilkins July 19, 2016

  reduced to the minimum by ChrisMicro, September 2018

  Demonstrate basic MPU-9250 functionality including parameterizing the register
  addresses, initializing the sensor, getting properly scaled accelerometer,
  gyroscope, and magnetometer data out. Added display functions to allow display
  to on breadboard monitor.
*/

#include <M5Stack.h>
#include "utility/MPU9250.h"

MPU9250 IMU;

void setup()
{
  M5.begin();
  Wire.begin();

  IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias);
  IMU.initMPU9250();
  IMU.initAK8963(IMU.magCalibration);
}

void loop()
{
  // If intPin goes high, all data registers have new data
  // On interrupt, check if data ready interrupt
  if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
    IMU.readAccelData(IMU.accelCount);
    IMU.getAres();

    IMU.ax = (float)IMU.accelCount[0] * IMU.aRes; // - accelBias[0];
    IMU.ay = (float)IMU.accelCount[1] * IMU.aRes; // - accelBias[1];
    IMU.az = (float)IMU.accelCount[2] * IMU.aRes; // - accelBias[2];

    IMU.readGyroData(IMU.gyroCount);  // Read the x/y/z adc values
    IMU.getGres();

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    IMU.gx = (float)IMU.gyroCount[0] * IMU.gRes;
    IMU.gy = (float)IMU.gyroCount[1] * IMU.gRes;
    IMU.gz = (float)IMU.gyroCount[2] * IMU.gRes;

    IMU.readMagData(IMU.magCount);  // Read the x/y/z adc values
    IMU.getMres();
    // User environmental x-axis correction in milliGauss, should be
    // automatically calculated
    //IMU.magbias[0] = +470.;
    // User environmental x-axis correction in milliGauss TODO axis??
    //IMU.magbias[1] = +120.;
    // User environmental x-axis correction in milliGauss
    //IMU.magbias[2] = +125.;

    // Calculate the magnetometer values in milliGauss
    // Include factory calibration per data sheet and user environmental
    // corrections
    // Get actual magnetometer value, this depends on scale being set
    IMU.mx = (float)IMU.magCount[0] * IMU.mRes * IMU.magCalibration[0] -
             IMU.magbias[0];
    IMU.my = (float)IMU.magCount[1] * IMU.mRes * IMU.magCalibration[1] -
             IMU.magbias[1];
    IMU.mz = (float)IMU.magCount[2] * IMU.mRes * IMU.magCalibration[2] -
             IMU.magbias[2];

    IMU.tempCount = IMU.readTempData();  // Read the adc values
    // Temperature in degrees Centigrade
    IMU.temperature = ((float) IMU.tempCount) / 333.87 + 21.0;

    int x=64+10;
    int y=128+20;
    int z=192+30;
    
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN , BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 0); M5.Lcd.print("MPU9250/AK8963");
    M5.Lcd.setCursor(0, 32); M5.Lcd.print("x");
    M5.Lcd.setCursor(x, 32); M5.Lcd.print("y");
    M5.Lcd.setCursor(y, 32); M5.Lcd.print("z");

    M5.Lcd.setTextColor(YELLOW , BLACK);
    M5.Lcd.setCursor(0, 48 * 2); M5.Lcd.print((int)(1000 * IMU.ax));
    M5.Lcd.setCursor(x, 48 * 2); M5.Lcd.print((int)(1000 * IMU.ay));
    M5.Lcd.setCursor(y, 48 * 2); M5.Lcd.print((int)(1000 * IMU.az));
    M5.Lcd.setCursor(z, 48 * 2); M5.Lcd.print("mg");

    M5.Lcd.setCursor(0, 64 * 2); M5.Lcd.print((int)(IMU.gx));
    M5.Lcd.setCursor(x, 64 * 2); M5.Lcd.print((int)(IMU.gy));
    M5.Lcd.setCursor(y, 64 * 2); M5.Lcd.print((int)(IMU.gz));
    M5.Lcd.setCursor(z, 64 * 2); M5.Lcd.print("o/s");

    M5.Lcd.setCursor(0, 80 * 2); M5.Lcd.print((int)(IMU.mx));
    M5.Lcd.setCursor(x, 80 * 2); M5.Lcd.print((int)(IMU.my));
    M5.Lcd.setCursor(y, 80 * 2); M5.Lcd.print((int)(IMU.mz));
    M5.Lcd.setCursor(z, 80 * 2); M5.Lcd.print("mG");

    M5.Lcd.setCursor(0,  96 * 2); M5.Lcd.print("Gyro Temperature ");
    M5.Lcd.setCursor(z,  96 * 2); M5.Lcd.print(IMU.temperature, 1);
    M5.Lcd.print(" C");
    delay(100);

  }

}
