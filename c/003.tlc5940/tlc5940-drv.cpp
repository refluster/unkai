/**
 * TLC5940 controller library for the Raspberry Pi
 * ===============================================
 *
 * With this library it is possible to control your TLC5940 LED driver
 * from your Raspberry Pi. It is a pure software based solution,
 * and does not require any kernel modifcations or whatsoever.
 *
 * Copyright (c) 2013 Lucas van Dijk <info@return1.net>
 * http://return1.net
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software 
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "common.h"

volatile int brightness[NUM_LED_MAX]; // brightness for each led
int num_led = NUM_LED_MAX; // the number of leds to control
static int tlc5940_update_interval = TLC5940_UPDATE_INTERVAL_DEFAULT; // msec
static int tlc5940_print_interval = 0; // msec, do not print if this var is zero

class RaspberryGPIOPin {
	int pin;
	
public:
	RaspberryGPIOPin(int _pin) {
		pin = _pin;
	};

	void setOutput() {
		pinMode(pin, OUTPUT);
	};

	void setLow() {
		digitalWrite(pin, 0);
	};

	void setHigh() {
		digitalWrite(pin, 1);
	};

	void pulse() {
		setHigh();
		setLow();
	};

	void setValue(int val) {
		if (val == 0) {
			setLow();
		} else {
			setHigh();
		}
	};
};

RaspberryGPIOPin *sin_pin;
RaspberryGPIOPin *sclk_pin;
RaspberryGPIOPin *blank_pin;
RaspberryGPIOPin *dcprg_pin;
RaspberryGPIOPin *vprg_pin;
RaspberryGPIOPin *xlat_pin;
RaspberryGPIOPin *gsclk_pin;
int first_cycle;

void tlc_init() {
	sin_pin->setLow();
	sclk_pin->setLow();
	blank_pin->setHigh();
	dcprg_pin->setLow();
	vprg_pin->setHigh();
	xlat_pin->setLow();
	gsclk_pin->setLow();
	first_cycle = true;
}

void tlc_update() {
	////////////////////////////////////////////////////////////
	if (first_cycle) {
		vprg_pin->setLow();
	}
	
	blank_pin->setLow();
	
	////////////////////////////////////////////////////////////
	// Start with the highest channel
	int channel_counter = 15;
	int gsclk_counter = 0;
	bool pulse_gsclk = true;
	
	while(gsclk_counter < 4096) {
		if(channel_counter >= 0) {						
			// Send the first 12 bits of the color to the TLC, MSB first
			for(int i = 11; i >= 0; i--) {				
				int value = (brightness[channel_counter] >> i) & 1;

				sin_pin->setValue(value);				
				sclk_pin->pulse();
				
				gsclk_pin->pulse();
				gsclk_counter++;
			}

			channel_counter--;
		} else {
			sin_pin->setLow();			

			gsclk_pin->pulse();
			gsclk_counter++;
		}		
	}
	
	////////////////////////////////////////////////////////////
	// If we reach here all color data has been sent to the TLC
	// And the full PWM cycle has been completed
	// Send a blank signal (so the internal GSCLK counter of the TLC is reset to zero)
	// and pulse the XLAT signal, so all data is latched in
	blank_pin->setHigh();
	xlat_pin->pulse();
	
	if(first_cycle) {
		sclk_pin->pulse();
		first_cycle = false;
	}
}

void update_thread() {
	wiringPiSetup();

	sin_pin = new RaspberryGPIOPin(4);
	sclk_pin = new RaspberryGPIOPin(2);
	blank_pin = new RaspberryGPIOPin(0);
	dcprg_pin = new RaspberryGPIOPin(5);
	vprg_pin = new RaspberryGPIOPin(6);
	xlat_pin = new RaspberryGPIOPin(1);
	gsclk_pin = new RaspberryGPIOPin(7);
	
	sin_pin->setOutput();
	sclk_pin->setOutput();
	blank_pin->setOutput();
	dcprg_pin->setOutput();
	vprg_pin->setOutput();
	xlat_pin->setOutput();
	gsclk_pin->setOutput();
	

	tlc_init();

	while (true) {
		// This thread only reads the bit pattern so no lock is required
//		tlc_controller.update();
		tlc_update();
		
		std::chrono::milliseconds duration(tlc5940_update_interval);
		std::this_thread::sleep_for(duration);

		// for verification
		if (tlc5940_print_interval > 0) {
			static int count = 0;
			count += tlc5940_update_interval;

			printf(".");
			// display message every tlc5940_print_interval msec
			if (count >= tlc5940_print_interval) {
				count -= tlc5940_print_interval;
				printf("tlc5940 brightness:");
				for (int i = 0; i < num_led; i++) {
					printf(" %4d", brightness[i]);
				}
				printf("\n");
				fflush(stdout);
			}
		}
	}
}

void chk_arg(int argc, char **argv) {
	int result;

	while ((result = getopt(argc, argv, "n:p:t:")) != -1){
		int t;
		switch (result) {
		case 'n':  // the number of leds to control
			t = atoi(optarg);
			if (t > 0) {
				num_led = t;
			}
			break;
		case 'p':  // tlc5940 print interval, usually larger than update interval
			t = atoi(optarg);
			if (t > 0) {
				tlc5940_print_interval = t;
			}
			break;
		case 't':  // tlc5940 update interval
			t = atoi(optarg);
			if (t > 0) {
				tlc5940_update_interval = t;
			}
			break;
		}
	}
}

int initialize() {
	memset((void*)brightness, 0, sizeof(brightness));
	num_led = NUM_LED_MAX;
	tlc5940_update_interval = TLC5940_UPDATE_INTERVAL_DEFAULT; // msec
	tlc5940_print_interval = 0; // msec, do not print if this var is zero
	return 0;
}

int main(int argc, char **argv) {
	// Initialize GPIO Pins
	if (wiringPiSetup() == -1) {
		throw std::runtime_error("Could not setup wiringPi, running as root?");
	}

	initialize();
	chk_arg(argc, argv);

	std::thread thread1(update_thread);
	std::thread thread2(pattern_thread);
	
	thread1.join();
	thread2.join();
}
