#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>

#define NUM_LEDS    4

#define CHECK(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x); \
            exit(-1); \
        } \
    } while (0) \

typedef enum {
	TLC5940_END,
	TLC5940_UPDATE,
} TLC5940_CTRL_INFO;

typedef struct {
	TLC5940_CTRL_INFO type;
	unsigned short brightness[NUM_LEDS];
} tlc5940_ctrl_info;

#endif /* #ifndef COMMON_H_ */
