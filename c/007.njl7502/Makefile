ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

include $(MAKEROOT)/mk/base.mk

TARGET = 007.njl7502

all:
	gcc main.c -L/usr/local/lib -lwiringPi -lrt -o $(TARGET)
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)
