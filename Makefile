CC          := gcc
CFLAGS      := -Wall -Wpointer-arith -Wwrite-strings -Wstrict-prototypes -pedantic -std=gnu99

all: ebuddy

ebuddy: main.o config.o notify.o emotion.o phidget.o gesture.o gesture_interface.o input.o
	$(CC) $(CFLAGS) build/main.o build/config.o build/notify.o build/emotion.o build/phidget.o build/gesture.o build/gesture_interface.o build/input.o -ljansson -lcurl -lphidget21 -loauth -o build/ebuddy

main.o: source/main.c source/main.h
	$(CC) $(CFLAGS) -c source/main.c -o build/main.o
	
config.o: source/config.c source/config.h 
	$(CC) $(CFLAGS) -c source/config.c -o build/config.o

notify.o: source/notify.c source/notify.h
	$(CC) $(CFLAGS) -c source/notify.c -o build/notify.o

emotion.o: source/emotion.c source/emotion.h
	$(CC) $(CFLAGS) -c source/emotion.c -o build/emotion.o

phidget.o: source/phidget.c source/phidget.h
	$(CC) $(CFLAGS) -c source/phidget.c -o build/phidget.o

gesture.o: source/gesture.c source/gesture.h
	$(CC) $(CFLAGS) -c source/gesture.c -o build/gesture.o

gesture_interface.o: source/gesture_interface.c source/gesture_interface.h
	$(CC) $(CFLAGS) -c source/gesture_interface.c -o build/gesture_interface.o

input.o: source/input.c source/input.h
	$(CC) $(CFLAGS) -c source/input.c -o build/input.o

config_test: build/config.o build/config_test.o
	$(CC) $(CFLAGS) build/config.o build/config_test.o -l
	
clean:
	@rm -rf build/*
