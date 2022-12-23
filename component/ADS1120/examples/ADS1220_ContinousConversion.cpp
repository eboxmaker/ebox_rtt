/*
  This example shows how to get continous conversions out of the ADS1120.
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

	adc.setGain(1);				 //Set gain 1.  Possible values are 1, 2, 4, 8, 16, 32, 64, 128.
	adc.setOpMode(0x00);		 // Set Normal Mode
	adc.setDataRate(0x05);		 // Set Data rate 101.  In normal mode = 600sps
	adc.setConversionMode(0x01); // 1=Continous Mode
	adc.setMultiplexer(0x00);	 // Set AIN0  & AIN1 (differential)
}

void loop()
{
	long test = 0;
	int samples = 0;
	unsigned long tiempoInicial = millis();

	while (millis() - tiempoInicial < 1000)
	{
		if (digitalRead(DATA_READY_PIN) == LOW)
		{
			test = adc.readADC();
			samples++;
		}
	}
	Serial.print("Samples per Seconds = ");
	Serial.println(samples);
	Serial.print("Last read value = ");
	Serial.println(test);
	Serial.println();
}