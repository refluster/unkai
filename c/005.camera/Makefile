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
	$(TARGET) -i jpg/before.jpg 35 100 60 255 20 255

####################
v4l2uvc.o: uvccapture-0.5/v4l2uvc.c
	g++ -Wall -DVERSION=\"0.4\" -c uvccapture-0.5/v4l2uvc.c -o v4l2uvc.o

test: camera.o v4l2uvc.o
	g++ camera.o v4l2uvc.o -o test.out `pkg-config --libs opencv`

test-run:
	./test.out 35 100 60 255 20 255
