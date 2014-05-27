#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

const int gpio_trigger = 0;
const int gpio_echo = 3;

struct timespec t_start, t_end;

void both() {
	if (t_start.tv_nsec == 0) {
		clock_gettime(CLOCK_REALTIME, &t_start);
	} else {
		unsigned long long s, e;
		unsigned long d;

		clock_gettime(CLOCK_REALTIME, &t_end);

		d = t_end.tv_nsec - t_start.tv_nsec;
		
		printf("d = %6.02f cm\n",
			   34000.0*d/1000000000/2);
	}
}

int main() {
	if (wiringPiSetup() == -1) {
		exit(1);
	}

	// setup interrupt handler
	wiringPiISR( gpio_echo, INT_EDGE_BOTH,  both  );

	// set i/o direction
	pinMode(gpio_trigger, OUTPUT);
	pinMode(gpio_echo, INPUT);

	// initialize device
	digitalWrite(gpio_trigger, LOW);
 	delay(100);
	digitalWrite(gpio_trigger, HIGH);
	delayMicroseconds(10);	
	digitalWrite(gpio_trigger, LOW);

	sleep(1);
	return 0;
}
