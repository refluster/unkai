ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

include $(MAKEROOT)/mk/base.mk

DRIVER = tlc5940-drv

LCFLAGS = -std=c++11 `pkg-config --cflags tlc5940-raspberry-1.0` \
		`pkg-config --libs tlc5940-raspberry-1.0` -lwiringPi

all: $(DRIVER)

$(DRIVER):
	g++ main.cpp $(LCFLAGS) -o $(DRIVER)
	sudo chown root:root $(DRIVER)
	sudo chmod u+s $(DRIVER)
