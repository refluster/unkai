ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

include $(MAKEROOT)/mk/base.mk

TARGET = 003.tlc5940

all: $(TARGET)

$(TARGET): tlc5940-drv.o tlc5940-if.o
	g++ -o $(TARGET) tlc5940-drv.o tlc5940-if.o -lwiringPi
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)

tlc5940-drv.o: tlc5940-drv.cpp
	g++ tlc5940-drv.cpp -c -std=c++11

tlc5940-if.o: tlc5940-if.cpp
	g++ tlc5940-if.cpp -c

clean:
	rm -f *.o $(TARGET)
