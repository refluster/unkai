ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

include $(MAKEROOT)/mk/base.mk

TARGET = 003.tlc5940-ctrl

LCFLAGS = -std=c++11 `pkg-config --cflags tlc5940-raspberry-1.0` \
		`pkg-config --libs tlc5940-raspberry-1.0` -lwiringPi

all:
	g++ main.cpp $(LCFLAGS) -o $(TARGET)
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)
