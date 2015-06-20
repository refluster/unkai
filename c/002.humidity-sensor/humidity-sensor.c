#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include "humidity-sensor.h"

#define MAX_TIME 85
#define DHT11PIN 8

static int dht11_read_val_test(float *humidity, float *temperature, float *fahrenheit) {
	uint8_t lststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	int dht11_val[5];

	for(i = 0; i < 5; i++)
		dht11_val[i] = 0;

	pinMode(DHT11PIN, OUTPUT);
	digitalWrite(DHT11PIN, LOW);
	delay(18);
	digitalWrite(DHT11PIN, HIGH);
	delayMicroseconds(40);
	pinMode(DHT11PIN, INPUT);
	for(i = 0; i < MAX_TIME; i++) {
		counter = 0;
		while(digitalRead(DHT11PIN) == lststate) {
			counter++;
			delayMicroseconds(1);
			if(counter == 255)
				break;
		}
		lststate = digitalRead(DHT11PIN);
		if(counter == 255)
			break;
		// top 3 transistions are ignored
		if(i >= 4 && i%2 == 0){
			dht11_val[j/8] <<= 1;
			if(counter > 16)
				dht11_val[j/8] |= 1;
			j++;
		}
	}
	// verify cheksum and print the verified data
	if((dht11_val[4] == ((dht11_val[0] + dht11_val[1] + dht11_val[2] + dht11_val[3]) & 0xFF)) &&
	   (dht11_val[0] != 0 && dht11_val[2] != 0)) {
		*fahrenheit = dht11_val[2]*9.0/5.0 + 32;
		*humidity = dht11_val[0] + dht11_val[1]*0.01;
		*temperature = dht11_val[2] + dht11_val[3]*0.01;
		return 0;
	} else {
		return -1;
	}
}

void dht11_read_val() {
	float humidity, temperature, fahrenheit;
	int i;
	
	for (i = 0; i < RETRY_MAX; i++) {
		if (dht11_read_val_test(&humidity, &temperature, &fahrenheit) == 0) {
			printf("humidity:%.1f celsius:%.1f fahrenheit:%.1f\n",
				   humidity, temperature, fahrenheit);
			break;
		}
	}
}

int dht11_init() {
	//printf("Interfacing Temperature and Humidity Sensor (DHT11) With Raspberry Pi\n");
	return wiringPiSetup();
}
