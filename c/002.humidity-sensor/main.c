#include <stdlib.h>
#include "humidity-sensor.h"

int main(int argc, char **argv) {
	int onetime_mode = 0;

	if (argc >= 2) {
		if (strcmp(argv[1], "-1") == 0) {
			onetime_mode = 1;
		}
	}

	if(dht11_init() == -1) {
		exit(1);
	}

	// wait for the the setup of DHT11
	delay(10);

	if (onetime_mode) {
		dht11_read_val();
	} else {
		int i;
		for (i = 0; i < RETRY_MAX; i++) {
			dht11_read_val();
			delay(1000);
		}
	}

	return 0;
}
