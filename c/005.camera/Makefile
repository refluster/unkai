ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

include $(MAKEROOT)/mk/base.mk

TARGET = 005.camera

all: $(TARGET)

$(TARGET): process.o image_input.o v4l2uvc.o
	g++ process.o image_input.o v4l2uvc.o -o $(TARGET) `pkg-config --libs opencv`
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)

process.o: process.cpp
	g++ process.cpp -c -o process.o `pkg-config --cflags opencv`

image_input.o: image_input.cpp
	g++ -Wall -c  `pkg-config --cflags opencv` image_input.cpp -o image_input.o

v4l2uvc.o: uvccapture-0.5/v4l2uvc.c
	g++ -Wall -DVERSION=\"0.4\" -c uvccapture-0.5/v4l2uvc.c -o v4l2uvc.o

run:
	./$(TARGET) -i jpg/before.jpg 35 100 60 255 20 255

run2:
	./$(TARGET) -b 20 -c 60 -g 80 -s 40 -r 640x480 -l "35:100:60:255:20:255"
