#include <stdio.h>
#include <string.h>
#include "common.h"

#define BUFSIZE 256

unsigned short brightness[NUM_LEDS];

void msg_decode(char *cmd, tlc5940_ctrl_info *ctrl_info) {
	int ret = 0;
	char *cmd_val;
	int i;

	printf("js-cmd: %s\n", cmd);
	
	CHECK((cmd_val = strtok(cmd, " ")) != NULL);
	ctrl_info->type = atoi(cmd_val);

	switch (ctrl_info->type) {
	case TLC5940_END:
		break;
	case TLC5940_UPDATE:
		for (i = 0; i < NUM_LEDS; i++) {
			CHECK((cmd_val = strtok(NULL, " ")) != NULL);
			ctrl_info->brightness[i] = atoi(cmd_val);
		}
	}
}

int main() {
	int end = 0;
	tlc5940_ctrl_info ctrl_info;
	int i;

	while (!end) {
		char buf[BUFSIZE];

		CHECK(fgets(buf, BUFSIZE, stdin) != NULL);
		msg_decode(buf, &ctrl_info);
		
		switch (ctrl_info.type) {
		case TLC5940_END:
			end = 1;
			break;
		case TLC5940_UPDATE:
			for (i = 0; i < NUM_LEDS; i++) {
				brightness[i] = ctrl_info.brightness[i];;
			}
			break;
		default:
			printf("unknown command\n");
			break;
		}
	}
}
