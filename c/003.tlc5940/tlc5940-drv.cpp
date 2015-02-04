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
RaspberryGPIOPin *xlat_pin;
RaspberryGPIOPin *gsclk_pin;

void tlc_init() {
	sin_pin->setLow();
	sclk_pin->setLow();
	blank_pin->setHigh();
	xlat_pin->setLow();
	gsclk_pin->setLow();
}

void update_brightness() {
	for (int ch = 15; ch >= 0; --ch) {
		for(int i = 11; i >= 0; --i) {
			int value = (brightness[ch] >> i) & 1;
			
			sin_pin->setValue(value);
			sclk_pin->pulse();
		}
	}
	
	sin_pin->setLow();
	xlat_pin->pulse();
}

void tlc_update() {
	blank_pin->setLow();
	for (int i = 0; i < 4096; ++i) {
		gsclk_pin->pulse();
	}
	blank_pin->setHigh();
}

int pwm_clock = 20;
int pwm_range= 4;

void update_thread() {
	int interval_msec = (int)(1000*(4096ull*pwm_clock*pwm_range)/19600000); // 1/freq * 4096 (sec)

	wiringPiSetup();

	sin_pin = new RaspberryGPIOPin(4);
	sclk_pin = new RaspberryGPIOPin(2);
	blank_pin = new RaspberryGPIOPin(0);
	xlat_pin = new RaspberryGPIOPin(7);
	gsclk_pin = new RaspberryGPIOPin(1);
	
	sin_pin->setOutput();
	sclk_pin->setOutput();
	blank_pin->setOutput();
	xlat_pin->setOutput();
	gsclk_pin->setOutput();

	tlc_init();

	printf("blank pulse interval: %d\n", interval_msec);

 	pinMode(1, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);

	pwmSetClock(pwm_clock);
	pwmSetRange(pwm_range);
	pwmWrite(1, pwm_range/2);

	std::chrono::milliseconds duration(interval_msec);
	
	while (true) {
		// This thread only reads the bit pattern so no lock is required
		std::this_thread::sleep_for(duration);
		blank_pin->setHigh();
		blank_pin->setLow();
	}
}

void chk_arg(int argc, char **argv) {
	int result;

	while ((result = getopt(argc, argv, "c:r:n:p:t:")) != -1){
		int t;
		switch (result) {
		case 'c':  // pwm clock
			t = atoi(optarg);
			if (t > 0) {
				pwm_clock = t;
			}
			break;
		case 'r':  // pwm range
			t = atoi(optarg);
			if (t > 0) {
				pwm_range = t;
			}
			break;
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
