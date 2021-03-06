#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wiringPi.h>

#include <iostream>
#include <thread>
#include <chrono>

#include "common.h"

#define PWM_CLK_HZ 19600000

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

static std::thread *update_thread;
static std::thread *userif_thread;
	
static unsigned short brightness[NUM_LED_MAX];

void gpio_init() {
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

	sin_pin->setLow();
	sclk_pin->setLow();
	blank_pin->setHigh();
	xlat_pin->setLow();
	gsclk_pin->setLow();
}

void gpio_exit() {
	delete sin_pin;
	delete sclk_pin;
	delete blank_pin;
	delete xlat_pin;
	delete gsclk_pin;
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

static int update_flg = 0;

void __update_brightness() {
	for (int ch = 15; ch >= 0; --ch) {
		for(int i = 11; i >= 0; --i) {
			int value = (brightness[ch] >> i) & 1;
			sin_pin->setValue(value);
			sclk_pin->pulse();
		}
	}
	
	sin_pin->setLow();
	xlat_pin->pulse();

	update_flg = 0;
}

void update() {
	int interval_msec;

	nice(-2);

	// interval: 1/freq * 4096 (sec)
	interval_msec = (int)(1000*(4096ull*pwm_clock*pwm_range)/PWM_CLK_HZ);
	std::chrono::milliseconds duration(interval_msec);
	printf("blank pulse interval: %d\n", interval_msec);

	while (true) {
		std::this_thread::sleep_for(duration);
		blank_pin->setHigh();
#if 1
		if (update_flg) {
			__update_brightness();
		}
#endif
		blank_pin->setLow();
	}
}

void signal_handler(int signo) {
	printf("sighandler : %d\n", signo);

	delete update_thread;
	delete userif_thread;
	pwm_exit();
	gpio_exit();
}

void set_sighandler() {
	signal(SIGINT, signal_handler);
}	

void initialize() {
	num_led = NUM_LED_MAX;
	set_sighandler();
	gpio_init();
	pwm_init();
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

void update_brightness(unsigned short b[]) {
	update_flg = 1;
	memcpy(brightness, b, sizeof(brightness[0])*num_led);
}

int main(int argc, char **argv) {
	initialize();
	chk_arg(argc, argv);

	update_thread = new std::thread(update);
	userif_thread = new std::thread(userif);

	update_thread->join();
	userif_thread->join();
}
