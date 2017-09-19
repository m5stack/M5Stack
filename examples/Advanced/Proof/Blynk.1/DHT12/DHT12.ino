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
  This example shows how value can be pushed from Arduino to
  the Blynk App.
  NOTE:
  BlynkTimer provides SimpleTimer functionality:
    http://playground.arduino.cc/Code/SimpleTimer
  App project setup:
    Value Display widget attached to Virtual Pin V5
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define BLYNK_DEBUG

#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT12.h>
#include <Wire.h> //The DHT12 uses I2C comunication.
DHT12 dht12; //Preset scale CELSIUS and ID 0x5c.

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
// char auth[] = "b991e6eb50e44df6945f459f8ccc1ea1";
char auth[] = "ba40ae2037c24658932551072069dc11";
// char ssid[] = "LabNet";
// char pass[] = "mytradio";
char ssid[] = HAX_SSID;
char pass[] = HAX_PASSWD;

BlynkTimer timer;
BlynkTimer reset_timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent() {
    // You can send any value at any time.
    // Please don't send more that 10 values per second.
    // Blynk.virtualWrite(V9, millis() / 1000)
    float tmp = dht12.readTemperature();
    float hum = dht12.readHumidity();
    Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%\r\n", tmp, hum);

    Blynk.virtualWrite(V0, tmp);
    Blynk.virtualWrite(V1, hum);
    Blynk.virtualWrite(V8, hum);
    Blynk.virtualWrite(V9, tmp);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setTextSize(3);
    M5.Lcd.printf("Temperatura:%2.1f  \r\nHumedad: %2.0f%%", tmp, hum);
}

void onReboot() {
    ESP.restart();
}

void setup() {
    // Debug console
    M5.begin();
    Wire.begin();

    Blynk.begin(auth, ssid, pass, "blynk.m5stack.com");

    // Setup a function to be called every second
    timer.setInterval(2000L, myTimerEvent);
    reset_timer.setInterval(3600000, onReboot);
    M5.Lcd.setBrightness(10);
}

void loop() {
    if(Blynk.connected() == true) {
        Blynk.run();   
    } else {
        ESP.restart();
    }
    timer.run(); // Initiates BlynkTimer
}

