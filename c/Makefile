ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

SUBDIR = 000.gpio-set 001.led-driver 002.humidity-sensor 003.tlc5940

include $(MAKEROOT)/mk/base.mk

