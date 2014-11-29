ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

include $(MAKEROOT)/mk/base.mk

TARGET = 005.camera

all: $(TARGET)

$(TARGET): camera.o
	g++ camera.o -o $(TARGET) `pkg-config --libs opencv`
#	sudo chown root:root $(TARGET)
#	sudo chmod u+s $(TARGET)

camera.o: camera.cpp
	g++ camera.cpp -c -o camera.o `pkg-config --cflags opencv`

run:
	$(TARGET) jpg/before.jpg 35 100 60 255 20 255
