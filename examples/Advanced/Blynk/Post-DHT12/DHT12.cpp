/*
	DHT12.cpp - Library for DHT12 sensor.
	v0.0.1 Beta
	Created by Bobadas, July 30,2016.
	Released into the public domain.
*/
#include "Arduino.h"
#include "Wire.h"
#include "DHT12.h"

DHT12::DHT12(byte scale,byte id)
{
	if (id==0 || id>126) _id=0x5c;
	else _id=id;
	if (scale==0 || scale>3) _scale=CELSIUS;
	else _scale=scale;
}

byte DHT12::read()
{
	Wire.beginTransmission(_id);
	Wire.write(0);
	if (Wire.endTransmission()!=0) return 1;  
	Wire.requestFrom(_id, 5);
	for (int i=0;i<5;i++) {
		datos[i]=Wire.read();
	};
	delay(50);
	if (Wire.available()!=0) return 2;
	if (datos[4]!=(datos[0]+datos[1]+datos[2]+datos[3])) return 3;
	return 0;
}

float DHT12::readTemperature(byte scale)
{
	float resultado=0;
	byte error=read();
	if (error!=0) return (float)error/100;
	if (scale==0) scale=_scale;
	switch(scale) {
		case CELSIUS:
			resultado=(datos[2]+(float)datos[3]/10);
			break;
		case FAHRENHEIT:
			resultado=((datos[2]+(float)datos[3]/10)*1.8+32);
			break;
		case KELVIN:
			resultado=(datos[2]+(float)datos[3]/10)+273.15;
			break;
	};
	return resultado;
}

float DHT12::readHumidity()
{
	float resultado;
	byte error=read();
	if (error!=0) return (float)error/100;
	resultado=(datos[0]+(float)datos[1]/10);
	return resultado;
}

