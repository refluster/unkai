#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

#define MIST_GEN_INTERVAL_USEC 1000000 // usec

void mist_on();
void mist_off();
int mistgen_init();

typedef enum {
	MISTGEN_END,
	MISTGEN_UPDATE,
} MISTGEN_CTRL_INFO;

typedef struct {
	MISTGEN_CTRL_INFO type;
	int mist_gen_percent;
} mistgen_ctrl_info;

static timer_t timerId;
static int mist_gen_usec = 0;

static void handler(int signo) {
	mist_on();
	printf("mist on\n");

	usleep(mist_gen_usec);

	mist_off();
	printf("mist off\n");
}

static int start_cyclic_handler() {
	struct sigaction sigact;
    struct itimerspec   itval;

    sigact.sa_handler = handler;
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);

	// set handler
    if(sigaction(SIGALRM, &sigact, NULL) == -1) {
        perror("sigaction");
    }

    itval.it_interval.tv_sec = MIST_GEN_INTERVAL_USEC / 1000000;
    itval.it_interval.tv_nsec = MIST_GEN_INTERVAL_USEC % 1000000;
    itval.it_value.tv_sec = MIST_GEN_INTERVAL_USEC / 1000000;
    itval.it_value.tv_nsec = MIST_GEN_INTERVAL_USEC % 1000000;

	// create timer
    if(timer_create(CLOCK_REALTIME, NULL, &timerId) == -1) {
        perror("timer_create");
    }
	// set timer
    if(timer_settime(timerId, 0, &itval, NULL) == -1) {
        perror("timer_settime");
    }
}

static int exit_cyclic_handler() {
	if(timer_delete(timerId) == -1) {
        perror("timer_delete");
    }
}

static int msg_decode(char *cmd, mistgen_ctrl_info *ctrl_info) {
	int ret = 0;
	char *cmd_val;
	int i;

	static int count;

	printf("mistgen i/f cmd: %s\n", cmd);

	cmd_val = (char*)strtok(cmd, " ");
	ctrl_info->type = (MISTGEN_CTRL_INFO)atoi(cmd_val);
	
	switch (ctrl_info->type) {
	case MISTGEN_END:
		break;
	case MISTGEN_UPDATE:
		cmd_val = (char*)strtok(NULL, " ");
		ctrl_info->mist_gen_percent = atoi(cmd_val);
		break;
	default:
		ret = -1;
		break;
	}

	return ret;
}

static void command_process() {
	mistgen_ctrl_info ctrl_info;
	char buf[256];
	
	while (1) {
		if(fgets(buf, sizeof(buf), stdin) != NULL) {
			if (msg_decode(buf, &ctrl_info) == 0) {
				mist_gen_usec = (long long)MIST_GEN_INTERVAL_USEC * ctrl_info.mist_gen_percent / 100;
				printf("mistgen: cmd %d %d\n", ctrl_info.mist_gen_percent, mist_gen_usec);
			}
		}
	}
}

int main() {
	mistgen_init();
	start_cyclic_handler();
	command_process();
	exit_cyclic_handler();
}

