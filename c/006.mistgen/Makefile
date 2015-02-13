ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

include $(MAKEROOT)/mk/base.mk

TARGET = 006.mistgen

all:
	gcc -c main.c
	gcc -c dev.c
	gcc main.o  dev.o -L/usr/local/lib -o $(TARGET) -lrt -lwiringPi
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)

clean:
	rm -f *.o $(TARGET)
