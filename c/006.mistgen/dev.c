#include <wiringPi.h>

#define MIST_PIN 7

void mist_on() {
	digitalWrite(MIST_PIN, HIGH);
}

void mist_off() {
	digitalWrite(MIST_PIN, LOW);
}

int mistgen_init() {
	wiringPiSetup();
	pinMode(MIST_PIN, OUTPUT);
	return 0;
}
