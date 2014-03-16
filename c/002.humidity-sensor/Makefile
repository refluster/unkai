TARGET = hmd-sensor

all:
	gcc main.c -L/usr/local/lib -lwiringPi  -o $(TARGET)
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)

