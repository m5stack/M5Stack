#include <M5Stack.h>
#include "clock.h"
#include "DHT.h"

#define DHTPIN 22     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
uint8_t select_app_id;

void setup()
{
  m5.begin();
  select_app_id = m5.bootSetup();

  m5.lcd.fillScreen(WHITE);
  m5.lcd.drawPicture(47, 38, 120, 96, gImage_logo);

  switch (select_app_id) {
    case 0:
      wifi_scan_setup();
      break;

    case 1:
      DHT11_setup();
      break;

    case 2:
      clock_setup();
      break;
  }
}

void loop()
{
  switch (select_app_id) {
    case 0:
      wifi_scan_loop();
      break;

    case 1:
      DHT11_loop();
      break;

    case 2:
      clock_loop();
      break;
  }
  
  m5.loop();
}

//-------------------------
void DHT11_setup()
{
  dht.begin();
  m5.lcd.fillScreen(WHITE);
  m5.lcd.drawTitle("Environment", 0x09F1);
  #define X_OFFSET 40
  #define Y_POS 165
}

void DHT11_loop()
{
  static uint32_t sampling_tick;
  static float pre_hif, pre_hic;
  static bool f2c_flag=0;
  
  if(millis() > sampling_tick) {
    sampling_tick = millis() + 1000;
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
    Serial.print(hif);
    Serial.println(" *F");

    if((pre_hif!=hif) || (pre_hic!=hic)) {
      pre_hif = hif;
      pre_hic = hic;
      
      m5.lcd.setFont(&FreeMonoBoldOblique12pt7b);
      // m5.lcd.setFont(&FreeMonoBoldOblique18pt7b);
      m5.lcd.fillRect(19, 70, 190, 30, WHITE);
      m5.lcd.setTextColor(GRAY);
      m5.lcd.setTextSize(1);
      m5.lcd.setCursor(25, 90);
      if(f2c_flag) {
        m5.lcd.printf("%2.1fF  %2.1f%%", hif, h);
      } else {
        m5.lcd.printf("%2.1fC  %2.1f%%", hic, h);
      }
    }
  }
  if(m5.pressed(BUTTON_B)) {
    f2c_flag = !f2c_flag;
    sampling_tick = 0;
    pre_hif=0;
  }
}

void wifi_scan_setup()
{
  m5.lcd.setFont();
}

void wifi_scan_loop()
{
  m5.lcd.setCursor(0, 0);
  // m5.lcd.println("scan start");
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  m5.lcd.fillScreen(BLACK);
  m5.lcd.println("wifi scan done");
  if (n == 0)
  {
    m5.lcd.println("no networks found");
  }
  else
  {
    m5.lcd.print(n);
    m5.lcd.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      m5.lcd.printf("%2d", i + 1);
      m5.lcd.print(": ");
      m5.lcd.print(WiFi.SSID(i));
      m5.lcd.print(" (");
      m5.lcd.print(WiFi.RSSI(i));
      m5.lcd.print(")");
      m5.lcd.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  delay(3000);
}
