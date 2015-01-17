#include <wiringPiSPI.h>
#include <stdio.h>

int main() {
	unsigned char data[] = {0b00000110, 0b00000000, 0b00000000};
	float lux;

	// setup wiringpi spi
	wiringPiSPISetup (0, 1000000) ;

	// send and receive data with spi
	wiringPiSPIDataRW (0, data, sizeof(data));
	
	// convert unit to lux
	lux = (float)(((data[1] << 8) + data[2]) & 0x0FFF) * 1200.0f / 4096.0f;
	
	printf("-- %.2f\n", lux);

	return 0;
}
