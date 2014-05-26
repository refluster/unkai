#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

const int gpio_trigger = 0;
const int gpio_echo = 3;

struct timespec t__start, t__end;

void both() {
	if (t__start.tv_nsec == 0) {
		clock_gettime(CLOCK_REALTIME, &t__start);
	} else {
		unsigned long long s, e;
		unsigned long d;

		clock_gettime(CLOCK_REALTIME, &t__end);

		d = t__end.tv_nsec - t__start.tv_nsec;
		
		printf("d = %6.02f cm\n",
			   34000.0*d/1000000000/2);
	}
}

void falling() {
	unsigned long d;
	
	clock_gettime(CLOCK_REALTIME, &t__end);
	
	d = t__end.tv_nsec - t__start.tv_nsec;
	
	printf("distance  %.02f cm\n",
		   34000.0*d/1000000000/2);
}

void rising() {
	clock_gettime(CLOCK_REALTIME, &t__start);
}

int main() {
	unsigned int t_start, t_stop;
	
	if (wiringPiSetup() == -1) {
		exit(1);
	}

	// setup interrupt handler
//	wiringPiISR( gpio_echo, INT_EDGE_FALLING, falling );
//	wiringPiISR( gpio_echo, INT_EDGE_RISING,  rising  );
	wiringPiISR( gpio_echo, INT_EDGE_BOTH,  both  );

	// set i/o direction
	pinMode(gpio_trigger, OUTPUT);
	pinMode(gpio_echo, INPUT);

	// initialize device
	digitalWrite(gpio_trigger, LOW);
	delay(100);

	// 
	digitalWrite(gpio_trigger, HIGH);
	delayMicroseconds(10);	
	digitalWrite(gpio_trigger, LOW);

	sleep(1);
	return 0;

	while (digitalRead(gpio_echo) == LOW);
	t_start = millis();

	while (digitalRead(gpio_echo) == HIGH);
	t_stop = millis();
	
	printf("distance = %d cm\n", 34000*(t_stop - t_start)/1000/2);
}
