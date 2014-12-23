ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

include $(MAKEROOT)/mk/base.mk

ifneq ($(CAMERA),DISABLE)
	CFLAGS = -DENABLE_CAMERA
endif

TARGET = 005.camera

all: $(TARGET)

ifeq ($(CAMERA),DISABLE)
$(TARGET): process.o image_input.o
	g++ process.o image_input.o -o $(TARGET) `pkg-config --libs opencv`
else
$(TARGET): process.o image_input.o v4l2uvc.o
	g++ process.o image_input.o v4l2uvc.o -o $(TARGET) `pkg-config --libs opencv`
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)
endif

process.o: process.cpp
	g++ $(CFLAGS) -c process.cpp -o process.o `pkg-config --cflags opencv`

image_input.o: image_input.cpp
	g++ $(CFLAGS) -c image_input.cpp -o image_input.o `pkg-config --cflags opencv`

ifneq ($(CAMERA),DISABLE)
v4l2uvc.o: uvccapture-0.5/v4l2uvc.c
	g++ $(CFLAGS) -c uvccapture-0.5/v4l2uvc.c -o v4l2uvc.o -DVERSION=\"0.4\"
endif

run:
	./$(TARGET) -i jpg/before.jpg -l "35:100:60:255:20:255"

run2:
	./$(TARGET) -b 20 -c 60 -g 80 -s 40 -r 640x480 -l "35:100:60:255:20:255"

