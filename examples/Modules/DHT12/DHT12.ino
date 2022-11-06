/*
    Description: Use DHT12 Sensor to read temperature, humidity and display the
   data on the screen.
*/
#include <M5Stack.h>
#include <Wire.h>  //The DHT12 uses I2C comunication.

#include "DHT12.h"
DHT12 dht12;  // Preset scale CELSIUS and ID 0x5c.

/*
For configuration library:
DHT12 dht12("Scale temperature","ID device for I2C");
On "Scale temperature" you can select the preset scale:
CELSIUS, FAHRENHEIT or KELVIN.
And on "ID device", you can put ID sensor, on DHT12
normally is 0x5c.
Examples:
  DHT12 dht12;
The preset scale is CELSIUS and ID is 0x5c.
  DHT12 dht12(KELVIN);
the preset scale is KELVIN and ID is 0x5c.
  DHT12 dht12(FAHRENHEIT,0x53);
The preset scale is FAHRENHEIT and ID is 0x53.
*/

void setup() {
    M5.begin();
    M5.Power.begin();
    Wire.begin();
    Serial.println("Prueba de libreria DHT12:");
    M5.Lcd.println("Prueba de libreria DHT12:");
}

void loop() {
    // Read temperature with preset scale.
    Serial.print("Temperatura: ");
    M5.Lcd.print("Temperatura: ");
    Serial.print(dht12.readTemperature());
    M5.Lcd.print(dht12.readTemperature());

    // Read humidity.
    Serial.print("*C  Humedad: ");
    M5.Lcd.print("*C  Humedad: ");
    Serial.print(dht12.readHumidity());
    M5.Lcd.println(dht12.readHumidity());

    // Read temperature as forced fahrenheit.
    Serial.println("%RH");
    Serial.println("%RH");
    Serial.print("Temperatura: ");
    Serial.print(dht12.readTemperature(FAHRENHEIT));

    // Read termperature as forced kelvin.
    Serial.println("*F");
    Serial.print("Temperatura: ");
    Serial.print(dht12.readTemperature(KELVIN));
    Serial.println("*K");

    delay(5000);
}
