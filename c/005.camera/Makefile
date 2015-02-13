ifndef MAKEROOT
	export MAKEROOT:= $(shell path=`pwd`; until test -d $$path/.git; do path=$${path%/*}; done; echo $$path)
endif

include $(MAKEROOT)/mk/base.mk

ifneq ($(CAMERA),DISABLE)
	CFLAGS = -DENABLE_CAMERA
endif

TARGET = 005.camera

ifeq ($(CAMERA),DISABLE)
SOURCE = process.cpp image_input.cpp
else
SOURCE = process.cpp image_input.cpp uvccapture-0.5/v4l2uvc.cpp
endif

OBJECT = $(SOURCE:%.cpp=%.o)

all: $(TARGET)

$(TARGET): $(OBJECT)
	g++ $^ -o $(TARGET) `pkg-config --libs opencv`
ifneq ($(CAMERA),DISABLE)
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)
endif

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@ `pkg-config --cflags opencv`

run:
	./$(TARGET) -i jpg/before.jpg -l "35:100:60:255:20:255"

run2:
	./$(TARGET) -b 20 -c 60 -g 80 -s 40 -r 320x240 -l "35:100:60:255:20:255"

run-loop:
	dir="$${HOME}/img"; mkdir -p $$dir; i=0; while [ true ]; do i=`expr $$i \+ 1`; \
		echo =========== $$i; ./005.camera -o $$dir/`date +'%Y%m%d_%H%M%S'`.jpg; sleep 600; done

clean:
	rm -f $(OBJECT) $(TARGET)
