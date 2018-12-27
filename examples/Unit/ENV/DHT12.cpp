/*
	DHT12.cpp - Library for DHT12 sensor.
	v0.0.1 Beta
	Created by Bobadas, July 30,2016.
	Released into the public domain.
*/
#include "DHT12.h"

DHT12::DHT12(uint8_t scale,uint8_t id)
{
	if (id==0 || id>126) _id=0x5c;
	else _id=id;
	if (scale==0 || scale>3) _scale=CELSIUS;
	else _scale=scale;
}

uint8_t DHT12::read()
{
	Wire.beginTransmission(_id);
	Wire.write(0);
	if (Wire.endTransmission()!=0) return 1;  
	Wire.requestFrom(_id, (uint8_t)5);
	for (int i=0;i<5;i++) {
		datos[i]=Wire.read();
	};
	delay(50);
	if (Wire.available()!=0) return 2;
	if (datos[4]!=(datos[0]+datos[1]+datos[2]+datos[3])) return 3;
	return 0;
}

float DHT12::readTemperature(uint8_t scale)
{
	float resultado=0;
	uint8_t error=read();
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
	uint8_t error=read();
	if (error!=0) return (float)error/100;
	resultado=(datos[0]+(float)datos[1]/10);
	return resultado;
}

