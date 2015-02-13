ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

include $(MAKEROOT)/mk/base.mk

TARGET = 002.humidity

all:
	gcc -c main.c
	gcc -c humidity-sensor.c
	gcc main.o humidity-sensor.o -L/usr/local/lib -lwiringPi -o $(TARGET)
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)

clean:
	rm -f *.o $(TARGET)
