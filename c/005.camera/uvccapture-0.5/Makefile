CC=gcc
APP_BINARY=uvccapture
VERSION = 0.4

WARNINGS = -Wall

CFLAGS = -DVERSION=\"$(VERSION)\" $(WARNINGS)
CPPFLAGS = $(CFLAGS)

OBJECTS= uvccapture.o v4l2uvc.o

%.o: %.c v4l2uvc.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

all:    uvccapture

clean:
	@echo "Cleaning up directory."
	rm -f *.a *.o $(APP_BINARY) core *~ log errlog

# Applications:
uvccapture: $(OBJECTS)
	$(CC)   $(OBJECTS) $(XPM_LIB) $(MATH_LIB) -ljpeg -o $(APP_BINARY)
