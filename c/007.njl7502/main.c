#include <wiringPiSPI.h>
#include <stdio.h>
#include <sys/time.h>

#define SAMPLE_COUNT 3

float med3(float *f) {
    if (f[0] < f[1])
        if (f[1] < f[2]) return f[1]; else if (f[2] < f[0]) return f[0]; else return f[2];
	else
        if (f[2] < f[1]) return f[1]; else if (f[0] < f[2]) return f[0]; else return f[2];
}

int main() {
	float lux[SAMPLE_COUNT];
	int i;
	float median;

	// setup wiringpi spi
	wiringPiSPISetup(0, 1000000) ;

	for (i = SAMPLE_COUNT - 1; i >= 0; --i) {
		unsigned char data[] = {0b00000110, 0b00000000, 0b00000000};

		// send and receive data with spi
		wiringPiSPIDataRW (0, data, sizeof(data));
	
		// convert unit to lux
		lux[i] = (float)(((data[1] << 8) + data[2]) & 0x0FFF) * 1200.0f / 4096.0f;
	}

#if SAMPLE_COUNT == 3
	median = med3(lux);
#else
# error "implement a function to find the median"
#endif	

	printf("illuminance: %.2f lux\n", median);
	
	return 0;
}
