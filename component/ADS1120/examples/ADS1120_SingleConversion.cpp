/*
  This example shows how to get single conversions out of the ADS1120.
  Lucas Etchezuri 30/06/2020
*/

#define CS_PIN 21
#define DATA_READY_PIN 4

#include "ADS1120.h"

ADS1120 adc;

void setup()
{
	Serial.begin(115200);
	delay(1000);

	adc.begin(14, 32, 13, CS_PIN, DATA_READY_PIN);

	adc.setGain(1);  			//Set gain 1.  Possible values are 1, 2, 4, 8, 16, 32, 64, 128.
	adc.setOpMode(0x00);		// Set Normal Mode
	adc.setDataRate(0x00);   	// Set Data rate
	adc.setConversionMode(0x00);// 0=Single Mode
	adc.setMultiplexer(0x00);   // Set AIN0  & AIN1 (differential)
}

void loop()
{
	long test = adc.readADC_Single();
	Serial.println(test);
	delay(250);
}
