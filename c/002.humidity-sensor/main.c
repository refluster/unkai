#include <stdlib.h>
#include "humidity-sensor.h"

int main(void) {
    if(dht11_init() == -1) {
        exit(1);
    }
    
    while(1) {
        dht11_read_val();
        delay(1000);
    }

    return 0;
}
