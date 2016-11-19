#include <stdio.h>
#include <string.h>
#include "common.h"

#define BUFSIZE 256

extern int num_led;
extern void update_brightness(unsigned short brightness[]);

static void msg_decode(char *cmd, tlc5940_ctrl_info *ctrl_info) {
	int ret = 0;
	char *cmd_val;
	int i;

	printf("js-cmd: %s\n", cmd);
	
	CHECK((cmd_val = strtok(cmd, " ")) != NULL);
	ctrl_info->type = (TLC5940_CTRL_INFO)atoi(cmd_val);

	switch (ctrl_info->type) {
	case TLC5940_END:
		break;
	case TLC5940_UPDATE:
		for (i = 0; i < num_led; i++) {
			CHECK((cmd_val = strtok(NULL, " ")) != NULL);
			ctrl_info->brightness[i] = atoi(cmd_val);
		}
	}
}

void userif() {
	int end = 0;
	tlc5940_ctrl_info ctrl_info;

	while (!end) {
		char buf[BUFSIZE];

		CHECK(fgets(buf, BUFSIZE, stdin) != NULL);
		msg_decode(buf, &ctrl_info);
		
		switch (ctrl_info.type) {
		case TLC5940_END:
			exit(0);
			break;
		case TLC5940_UPDATE:
			update_brightness(ctrl_info.brightness);
			break;
		default:
			printf("unknown command\n");
			break;
		}
	}
}
