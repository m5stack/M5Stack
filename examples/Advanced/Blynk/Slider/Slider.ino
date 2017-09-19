/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  You can receive x and y coords for joystick movement within App.

  App project setup:
    Two Axis Joystick on V1 in MERGE output mode.
    MERGE mode means device will receive both x and y within 1 message
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
// #define BLYNK_DEBUG

#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "21735bafd13b47e6ae6908fd240302d1";

char ssid[] = HAX_SSID;
char pass[] = HAX_PASSWD;

// You can send commands from Terminal to your hardware. Just use
uint8_t r,g,b;
uint8_t radius = 60;

BLYNK_WRITE(V0)
{
    M5.Lcd.setBrightness(param[0].asInt());
}

BLYNK_WRITE(V3)
{
    r = param[0].asInt();
    M5.Lcd.fillCircle(ILI9341_TFTWIDTH / 2, ILI9341_TFTHEIGHT / 2, radius, M5.Lcd.color565(r, g, b));
    Serial.print("r = ");
    Serial.println(r);
}

BLYNK_WRITE(V4)
{
    g = param[0].asInt();
    M5.Lcd.fillCircle(ILI9341_TFTWIDTH / 2, ILI9341_TFTHEIGHT / 2, radius, M5.Lcd.color565(r, g, b));
    Serial.print("g = ");
    Serial.println(g);
}

BLYNK_WRITE(V5)
{
    b = param[0].asInt();
    M5.Lcd.fillCircle(ILI9341_TFTWIDTH / 2, ILI9341_TFTHEIGHT / 2, radius, M5.Lcd.color565(r, g, b));
    Serial.print("b = ");
    Serial.println(b);
}

void setup()
{
    // Debug console
    Serial.begin(115200);
    M5.begin();
    M5.Lcd.setBrightness(255);

    Blynk.begin(auth, ssid, pass, "blynk.m5stack.com");

}

void loop()
{
    Blynk.run();
}
