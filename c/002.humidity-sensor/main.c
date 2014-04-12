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

	if (onetime_mode) {
		dht11_read_val();
	} else {
		while(1) {
			dht11_read_val();
			delay(1000);
		}
	}

	return 0;
}
