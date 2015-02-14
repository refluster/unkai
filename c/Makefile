ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

SUBDIR = 000.gpio-set 001.led-driver 002.humidity-sensor 003.tlc5940 \
		004.hc-sr04 005.camera 006.mistgen 007.njl7502 008.date

include $(MAKEROOT)/mk/base.mk


