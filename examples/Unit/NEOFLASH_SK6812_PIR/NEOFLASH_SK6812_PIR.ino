/*
    Description: NEOFLASH real-time clock case: fill in your personal WiFi
   information in the code below. After the program runs, it will automatically
   send an HTTP request to obtain the current time data, combined with PIR
   induction to control the RGB LED on and off. Please install library before
   compiling: FastLED: https://github.com/FastLED/FastLED
*/
#include <Arduino.h>
#include <HTTPClient.h>
#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>

#include "DisplayCurrentTime.h"
#include "FastLED.h"
#include "time.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN 26
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    192
CRGB leds[NUM_LEDS];

#define BRIGHTNESS        5
#define FRAMES_PER_SECOND 120

// -- The core to run FastLED.show()
#define FASTLED_SHOW_CORE 0

// -- Task handles for use in the notifications
static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t userTaskHandle        = 0;

const char* ssid     = "***";
const char* password = "********";

const char* ntpServer        = "pool.ntp.org";
const long gmtOffset_sec     = 28800;
const int daylightOffset_sec = 0;

// This is GandiStandardSSLCA2.pem, the root Certificate Authority that signed
// the server certifcate for the demo server https://jigsaw.w3.org in this
// example. This certificate is valid until Sep 11 23:59:59 2024 GMT
const char* rootCACertificate =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIF6TCCA9GgAwIBAgIQBeTcO5Q4qzuFl8umoZhQ4zANBgkqhkiG9w0BAQwFADCB\n"
    "iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n"
    "cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n"
    "BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTQw\n"
    "OTEyMDAwMDAwWhcNMjQwOTExMjM1OTU5WjBfMQswCQYDVQQGEwJGUjEOMAwGA1UE\n"
    "CBMFUGFyaXMxDjAMBgNVBAcTBVBhcmlzMQ4wDAYDVQQKEwVHYW5kaTEgMB4GA1UE\n"
    "AxMXR2FuZGkgU3RhbmRhcmQgU1NMIENBIDIwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n"
    "DwAwggEKAoIBAQCUBC2meZV0/9UAPPWu2JSxKXzAjwsLibmCg5duNyj1ohrP0pIL\n"
    "m6jTh5RzhBCf3DXLwi2SrCG5yzv8QMHBgyHwv/j2nPqcghDA0I5O5Q1MsJFckLSk\n"
    "QFEW2uSEEi0FXKEfFxkkUap66uEHG4aNAXLy59SDIzme4OFMH2sio7QQZrDtgpbX\n"
    "bmq08j+1QvzdirWrui0dOnWbMdw+naxb00ENbLAb9Tr1eeohovj0M1JLJC0epJmx\n"
    "bUi8uBL+cnB89/sCdfSN3tbawKAyGlLfOGsuRTg/PwSWAP2h9KK71RfWJ3wbWFmV\n"
    "XooS/ZyrgT5SKEhRhWvzkbKGPym1bgNi7tYFAgMBAAGjggF1MIIBcTAfBgNVHSME\n"
    "GDAWgBRTeb9aqitKz1SA4dibwJ3ysgNmyzAdBgNVHQ4EFgQUs5Cn2MmvTs1hPJ98\n"
    "rV1/Qf1pMOowDgYDVR0PAQH/BAQDAgGGMBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYD\n"
    "VR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMCIGA1UdIAQbMBkwDQYLKwYBBAGy\n"
    "MQECAhowCAYGZ4EMAQIBMFAGA1UdHwRJMEcwRaBDoEGGP2h0dHA6Ly9jcmwudXNl\n"
    "cnRydXN0LmNvbS9VU0VSVHJ1c3RSU0FDZXJ0aWZpY2F0aW9uQXV0aG9yaXR5LmNy\n"
    "bDB2BggrBgEFBQcBAQRqMGgwPwYIKwYBBQUHMAKGM2h0dHA6Ly9jcnQudXNlcnRy\n"
    "dXN0LmNvbS9VU0VSVHJ1c3RSU0FBZGRUcnVzdENBLmNydDAlBggrBgEFBQcwAYYZ\n"
    "aHR0cDovL29jc3AudXNlcnRydXN0LmNvbTANBgkqhkiG9w0BAQwFAAOCAgEAWGf9\n"
    "crJq13xhlhl+2UNG0SZ9yFP6ZrBrLafTqlb3OojQO3LJUP33WbKqaPWMcwO7lWUX\n"
    "zi8c3ZgTopHJ7qFAbjyY1lzzsiI8Le4bpOHeICQW8owRc5E69vrOJAKHypPstLbI\n"
    "FhfFcvwnQPYT/pOmnVHvPCvYd1ebjGU6NSU2t7WKY28HJ5OxYI2A25bUeo8tqxyI\n"
    "yW5+1mUfr13KFj8oRtygNeX56eXVlogMT8a3d2dIhCe2H7Bo26y/d7CQuKLJHDJd\n"
    "ArolQ4FCR7vY4Y8MDEZf7kYzawMUgtN+zY+vkNaOJH1AQrRqahfGlZfh8jjNp+20\n"
    "J0CT33KpuMZmYzc4ZCIwojvxuch7yPspOqsactIGEk72gtQjbz7Dk+XYtsDe3CMW\n"
    "1hMwt6CaDixVBgBwAc/qOR2A24j3pSC4W/0xJmmPLQphgzpHphNULB7j7UTKvGof\n"
    "KA5R2d4On3XNDgOVyvnFqSot/kGkoUeuDcL5OWYzSlvhhChZbH2UF3bkRYKtcCD9\n"
    "0m9jqNf6oDP6N8v3smWe2lBvP+Sn845dWDKXcCMu5/3EFZucJ48y7RetWIExKREa\n"
    "m9T8bJUox04FB6b9HbwZ4ui3uRGKLXASUoWNjDNKD/yZkuBjcNqllEdjB+dYxzFf\n"
    "BT02Vf6Dsuimrdfp5gJ0iHRc2jTbkNJtUQoj1iM=\n"
    "-----END CERTIFICATE-----\n";

uint16_t time4Day;
uint16_t printLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return 0;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    time4Day = timeinfo.tm_hour * 1000 + timeinfo.tm_min;
    Serial.printf("time4Day:%d \r\n", time4Day);

    // M5.Lcd.clear(BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextFont(2);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.fillRect(0, 25, 100, 25, BLACK);
    M5.Lcd.setCursor(0, 25);
    M5.Lcd.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    return time4Day;
}

/** show() for ESP32
 *  Call this function instead of FastLED.show(). It signals core 0 to issue a
 * show, then waits for a notification that it is done.
 */
void FastLEDshowESP32() {
    if (userTaskHandle == 0) {
        // -- Store the handle of the current task, so that the show task can
        //    notify it when it's done
        userTaskHandle = xTaskGetCurrentTaskHandle();

        // -- Trigger the show task
        xTaskNotifyGive(FastLEDshowTaskHandle);

        // -- Wait to be notified that it's done
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200);
        ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
        userTaskHandle = 0;
    }
}

/** show Task
 *  This function runs on core 0 and just waits for requests to call
 * FastLED.show()
 */
void FastLEDshowTask(void* pvParameters) {
    // -- Run forever...
    for (;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // -- Do the show (synchronously)
        FastLED.show();

        // -- Notify the calling task
        xTaskNotifyGive(userTaskHandle);
    }
}

WiFiMulti WiFiMulti;

void InitWifi() {
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");

    // init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();

    // disconnect WiFi as it's no longer needed
    // WiFi.disconnect(true);
    // WiFi.mode(WIFI_OFF);
}
void setup() {
    delay(3000);  // 3 second delay for recovery
    M5.begin();
    M5.Power.begin();

    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(40, 0);
    M5.Lcd.println("Neoflash example");
    M5.Lcd.setTextColor(WHITE);
    InitWifi();
    pinMode(36, INPUT);
    dacWrite(25, 0);
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(TypicalLEDStrip);
    // FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds,
    // NUM_LEDS).setCorrection(TypicalLEDStrip);

    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);
    // M5.Lcd.drawJpg((uint8_t *)m5_logo, 22758, 0, 0, 320, 240);
    int core = xPortGetCoreID();
    Serial.print("Main code running on core ");
    Serial.println(core);

    // -- Create the FastLED show task
    xTaskCreatePinnedToCore(FastLEDshowTask, "FastLEDshowTask", 2048, NULL, 2,
                            &FastLEDshowTaskHandle, FASTLED_SHOW_CORE);
}

// List of patterns to cycle through.  Each is defined as a separate function
// below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {confetti, sinelon};

uint8_t gCurrentPatternNumber = 0;  // Index number of which pattern is current
uint8_t gHue = 0;  // rotating "base color" used by many of the patterns
uint8_t i    = 0;
uint32_t sys_time;
uint16_t curtime;
void loop() {
    uint8_t i;
    uint16_t lasttime;
    // https_test();
    curtime = printLocalTime();

    if (digitalRead(36) == 1) {
        Serial.printf("ad read = 111\r\n");
        if (lasttime != curtime) {
            for (i = 0; i < 192; i++) {
                leds[i] = CRGB::Black;
            }
        }
        displayCurrentTime(curtime);
        diaplayPoint();
        delay(500);
        FastLED.show();
        clearPoint();
        delay(500);
        FastLED.show();
    } else {
        Serial.printf("ad read = 000\r\n");
        clearLeds();
        delay(1000);
        FastLED.show();
    }
    lasttime = curtime;
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
    // add one to the current pattern number, and wrap around at the end
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
    i++;
}

void rainbow() {
    // FastLED's built-in rainbow generator
    fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() {
    // built-in FastLED rainbow, plus some random sparkly glitter
    rainbow();
    addGlitter(80);
}

void addGlitter(fract8 chanceOfGlitter) {
    if (random8() < chanceOfGlitter) {
        leds[random16(NUM_LEDS)] += CRGB::White;
    }
}

void confetti() {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon() {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(leds, NUM_LEDS, 20);
    int pos = beatsin16(13, 0, NUM_LEDS - 1);
    leds[pos] += CHSV(gHue, 255, 192);
}

void bpm() {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette  = PartyColors_p;
    uint8_t beat           = beatsin8(BeatsPerMinute, 64, 255);
    for (int i = 0; i < NUM_LEDS; i++) {  // 9948
        leds[i] =
            ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
}

void juggle() {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(leds, NUM_LEDS, 20);
    byte dothue = 0;
    for (int i = 0; i < 8; i++) {
        leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}

void https_test(void) {
    WiFiClientSecure* client = new WiFiClientSecure;
    if (client) {
        client->setCACert(rootCACertificate);
        {
            // Add a scoping block for HTTPClient https to make sure it is
            // destroyed before WiFiClientSecure *client is
            HTTPClient https;

            Serial.print("[HTTPS] begin...\n");
            if (https.begin(
                    "https://jigsaw.w3.org/HTTP/connection.html")) {  // HTTPS
                Serial.print("[HTTPS] GET...\n");
                // start connection and send HTTP header
                int httpCode = https.GET();

                // httpCode will be negative on error
                if (httpCode > 0) {
                    // HTTP header has been send and Server response header has
                    // been handled
                    Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

                    // file found at server
                    if (httpCode == HTTP_CODE_OK ||
                        httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                        String payload = https.getString();
                        Serial.println(payload);
                    }
                } else {
                    Serial.printf("[HTTPS] GET... failed, error: %s\n",
                                  https.errorToString(httpCode).c_str());
                }

                https.end();
            } else {
                Serial.printf("[HTTPS] Unable to connect\n");
            }

            // End extra scoping block
        }

        delete client;
    } else {
        Serial.println("Unable to create client");
    }

    Serial.println();
    Serial.println("Waiting 10s before the next round...");
    delay(10000);
}
