ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

include $(MAKEROOT)/mk/base.mk

TARGET = 008.date

all:
	gcc date.c -o $(TARGET)
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)
