TARGET = 002.humidity

all:
	gcc -c main.c
	gcc -c humidity-sensor.c
	gcc main.o humidity-sensor.o -L/usr/local/lib -lwiringPi -o $(TARGET)
	sudo chown root:root $(TARGET)
	sudo chmod u+s $(TARGET)
