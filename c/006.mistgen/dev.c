#include <wiringPi.h>

#define MIST_DATA_PIN 3

void mist_on() {
	digitalWrite(MIST_DATA_PIN, HIGH);
}

void mist_off() {
	digitalWrite(MIST_DATA_PIN, LOW);
}

int mistgen_init() {
	wiringPiSetup();
	pinMode(MIST_DATA_PIN, OUTPUT);
	return 0;
}
