#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "common.h"

#define PWM_CLK_HZ 19600000

volatile int brightness[NUM_LED_MAX]; // brightness for each led
int num_led = NUM_LED_MAX; // the number of leds to control

class GPIOPin {
	int pin;
	
public:
	GPIOPin(int _pin) {
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

static GPIOPin *sin_pin;
static GPIOPin *sclk_pin;
static GPIOPin *blank_pin;
static GPIOPin *xlat_pin;
static GPIOPin *gsclk_pin;

static int pwm_clock = 20;
static int pwm_range= 4;

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

void pwm_init() {
 	pinMode(1, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(pwm_clock);
	pwmSetRange(pwm_range);
	pwmWrite(1, pwm_range/2);
}

void pwm_exit() {
	pwmSetClock(4095);
	pwmSetRange(4095);
	pwmWrite(1, 4095);
 	pinMode(1, INPUT);
}

void update_thread() {
	int interval_msec = (int)(1000*(4096ull*pwm_clock*pwm_range)/PWM_CLK_HZ); // 1/freq * 4096 (sec)

	wiringPiSetup();

	sin_pin = new GPIOPin(4);
	sclk_pin = new GPIOPin(2);
	blank_pin = new GPIOPin(0);
	xlat_pin = new GPIOPin(7);
	gsclk_pin = new GPIOPin(1);
	
	sin_pin->setOutput();
	sclk_pin->setOutput();
	blank_pin->setOutput();
	xlat_pin->setOutput();
	gsclk_pin->setOutput();

	printf("blank pulse interval: %d\n", interval_msec);

	std::chrono::milliseconds duration(interval_msec);
	
	tlc_init();

	pwm_init();

	while (true) {
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
		}
	}
}

int initialize() {
	memset((void*)brightness, 0, sizeof(brightness));
	num_led = NUM_LED_MAX;
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
