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
// char auth[] = "1c1c392bfd6f4d49826b9840a08ab733";
char auth[] = "85ffe483e5694600b90cc06deea51508";
// char ssid[] = "LabNet";
// char pass[] = "mytradio";
char ssid[] = HAX_SSID;
char pass[] = HAX_PASSWD;

WidgetTerminal terminal(V0);

// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
BLYNK_WRITE(V0)
{

    // if you type "Marco" into Terminal Widget - it will respond: "Polo:"
    if (String("Marco") == param.asStr())
    {
        terminal.println("You said: 'Marco'");
        terminal.println("I said: 'Polo'");
    }
    else
    {

        // Send it back
        // terminal.print("You said:");
        // terminal.write(param.getBuffer(), param.getLength());
        // terminal.println();

        // BLYNK_PRINT.print("You said:");
        // BLYNK_PRINT.printf("%s",param.getBuffer());
        // BLYNK_PRINT.println();
        M5.Lcd.print("You said:");
        M5.Lcd.printf("%s", param.getBuffer());
        M5.Lcd.println();
    }

    // Ensure everything is sent
    terminal.flush();
}

BLYNK_WRITE(V1)
{
    int x = param[0].asInt();
    // int y = param[1].asInt();

    // Do something with x and y
    Serial.print("X = ");
    Serial.println(x);
    // Serial.print("; Y = ");
    // Serial.println(y);
}

BLYNK_WRITE(V2)
{
    int y = param[0].asInt();

    // Do something with x and y
    // Serial.print("X = ");
    // Serial.println(x);
    Serial.print("Y = ");
    Serial.println(y);
}

void setup()
{
    // Debug console
    Serial.begin(115200);
    M5.begin();

    Blynk.begin(auth, ssid, pass, "blynk.m5stack.com");

    // This will print Blynk Software version to the Terminal Widget when
    // your hardware gets connected to Blynk Server
    M5.Lcd.println(F("Blynk v" BLYNK_VERSION ": Device started"));
    M5.Lcd.println(F("-------------"));
    M5.Lcd.println(F("Type 'Marco' and get a reply, or type"));
    M5.Lcd.println(F("anything else and get it printed back."));

    terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
    terminal.println(F("-------------"));
    terminal.println(F("Type 'Marco' and get a reply, or type"));
    terminal.println(F("anything else and get it printed back."));
    terminal.flush();
    M5.Lcd.setTextSize(2);
}

void loop()
{
    Blynk.run();
}
