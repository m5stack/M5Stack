/*
 * @Author: Sorzn
 * @Date: 2019-11-21 19:58:40
 * @LastEditTime: 2019-11-22 20:36:35
 * @Description: M5Stack project
 * @FilePath: /bmm150/bmm150.ino
 */

#include <Arduino.h>
#include <Wire.h>
#include "Preferences.h"
#include "M5Stack.h"
#include "math.h"
#include "bmm150.h"
#include "bmm150_defs.h"

Preferences prefs;

struct bmm150_dev dev;
bmm150_mag_data mag_offset;
bmm150_mag_data mag_max;
bmm150_mag_data mag_min;
TFT_eSprite img = TFT_eSprite(&M5.Lcd);

int8_t i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len)
{
    if(M5.I2C.readBytes(dev_id, reg_addr, len, read_data))
    {
        return BMM150_OK;
    }
    else
    {
        return BMM150_E_DEV_NOT_FOUND;
    }
}

int8_t i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len)
{
    if(M5.I2C.writeBytes(dev_id, reg_addr, read_data, len))
    {
        return BMM150_OK;
    }
    else
    {
        return BMM150_E_DEV_NOT_FOUND;
    }
}

int8_t bmm150_initialization()
{
    int8_t rslt = BMM150_OK;

    /* Sensor interface over SPI with native chip select line */
    dev.dev_id = 0x10;
    dev.intf = BMM150_I2C_INTF;
    dev.read = i2c_read;
    dev.write = i2c_write;
    dev.delay_ms = delay;

    /* make sure max < mag data first  */
    mag_max.x = -2000;
    mag_max.y = -2000;
    mag_max.z = -2000;

    /* make sure min > mag data first  */
    mag_min.x = 2000;
    mag_min.y = 2000;
    mag_min.z = 2000;

    rslt = bmm150_init(&dev);
    dev.settings.pwr_mode = BMM150_NORMAL_MODE;
    rslt |= bmm150_set_op_mode(&dev);
    dev.settings.preset_mode = BMM150_PRESETMODE_ENHANCED;
    rslt |= bmm150_set_presetmode(&dev);
    return rslt;
}

void bmm150_offset_save()
{
    prefs.begin("bmm150", false);
    prefs.putBytes("offset", (uint8_t *)&mag_offset, sizeof(bmm150_mag_data));
    prefs.end();
}

void bmm150_offset_load()
{
    if(prefs.begin("bmm150", true))
    {
        prefs.getBytes("offset", (uint8_t *)&mag_offset, sizeof(bmm150_mag_data));
        prefs.end();
        Serial.printf("bmm150 load offset finish.... \r\n");
    }
    else
    {
        Serial.printf("bmm150 load offset failed.... \r\n");
    }
}

void setup() 
{
    M5.begin(true, false, true, false);
    Wire.begin(21, 22, 400000);

    img.setColorDepth(1);
    img.setTextColor(TFT_WHITE);
    img.createSprite(320, 240);
    img.setBitmapColor(TFT_WHITE, 0);

    if(bmm150_initialization() != BMM150_OK)
    {
        img.fillSprite(0);
        img.drawCentreString("BMM150 init failed", 160, 110, 4);
        img.pushSprite(0, 0);
        for(;;)
        {
            delay(100);
        }
    }

    bmm150_offset_load();
}

void bmm150_calibrate(uint32_t calibrate_time)
{
    uint32_t calibrate_timeout = 0;

    calibrate_timeout = millis() + calibrate_time;
    Serial.printf("Go calibrate, use %d ms \r\n", calibrate_time);
    Serial.printf("running ...");

    while (calibrate_timeout > millis())
    {
        bmm150_read_mag_data(&dev);
        if(dev.data.x)
        {
            mag_min.x = (dev.data.x < mag_min.x) ? dev.data.x : mag_min.x;
            mag_max.x = (dev.data.x > mag_max.x) ? dev.data.x : mag_max.x;
        }

        if(dev.data.y)
        {
            mag_max.y = (dev.data.y > mag_max.y) ? dev.data.y : mag_max.y;
            mag_min.y = (dev.data.y < mag_min.y) ? dev.data.y : mag_min.y;
        }

        if(dev.data.z)
        {
            mag_min.z = (dev.data.z < mag_min.z) ? dev.data.z : mag_min.z;
            mag_max.z = (dev.data.z > mag_max.z) ? dev.data.z : mag_max.z;
        }
        delay(100);
    }

    mag_offset.x = (mag_max.x + mag_min.x) / 2;
    mag_offset.y = (mag_max.y + mag_min.y) / 2;
    mag_offset.z = (mag_max.z + mag_min.z) / 2;
    bmm150_offset_save();

    Serial.printf("\n calibrate finish ... \r\n");
    Serial.printf("mag_max.x: %.2f x_min: %.2f \t", mag_max.x, mag_min.x);
    Serial.printf("y_max: %.2f y_min: %.2f \t", mag_max.y, mag_min.y);
    Serial.printf("z_max: %.2f z_min: %.2f \r\n", mag_max.z, mag_min.z);
}

void loop() 
{
    char text_string[100];
    M5.update();
    bmm150_read_mag_data(&dev);
    float head_dir = atan2(dev.data.x -  mag_offset.x, dev.data.y - mag_offset.y) * 180.0 / M_PI;
    Serial.printf("Magnetometer data, heading %.2f\n", head_dir);
    Serial.printf("MAG X : %.2f \t MAG Y : %.2f \t MAG Z : %.2f \n", dev.data.x, dev.data.y, dev.data.z);
    Serial.printf("MID X : %.2f \t MID Y : %.2f \t MID Z : %.2f \n", mag_offset.x, mag_offset.y, mag_offset.z);
    
    img.fillSprite(0);
    sprintf(text_string, "MAG X: %.2f", dev.data.x);
    img.drawString(text_string, 10, 20, 4);
    sprintf(text_string, "MAG Y: %.2f", dev.data.y);
    img.drawString(text_string, 10, 50, 4);
    sprintf(text_string, "MAG Z: %.2f", dev.data.z);
    img.drawString(text_string, 10, 80, 4);
    sprintf(text_string, "HEAD Angle: %.2f", head_dir);
    img.drawString(text_string, 10, 110, 4);
    img.drawCentreString("Press BtnA enter calibrate", 160, 150, 4);
    img.pushSprite(0, 0);
    
    if(M5.BtnA.wasPressed())
    {
        img.fillSprite(0);
        img.drawCentreString("Flip + rotate core calibration", 160, 110, 4);
        img.pushSprite(0, 0);
        bmm150_calibrate(10000);
    }

    delay(100);
}