CC          := gcc
CFLAGS      := -Wall -Wpointer-arith -Wwrite-strings -Wstrict-prototypes -pedantic -std=gnu99 -g

LIB         := -ljansson -lcurl -loauth
LIB_LINUX   := -lphidget21
LIB_OSX     := -framework Phidget21

INCLUDE     := -Isource/

all: linux

demo: checkdir demo_test.o config.o notify.o emotion.o phidget.o gesture.o gesture_interface.o input.o utility.o queue.o
	$(CC) $(CFLAGS) build/demo_test.o build/config.o build/notify.o build/emotion.o build/phidget.o build/gesture.o build/gesture_interface.o build/input.o build/utility.o build/queue.o $(LIB) $(LIB_LINUX) -o build/demo

linux: checkdir main.o config.o notify.o emotion.o phidget.o gesture.o gesture_interface.o input.o utility.o queue.o
	$(CC) $(CFLAGS) build/main.o build/config.o build/notify.o build/emotion.o build/phidget.o build/gesture.o build/gesture_interface.o build/input.o build/utility.o build/queue.o $(LIB) $(LIB_LINUX) -o build/ebuddy

osx: main.o config.o notify.o emotion.o phidget.o gesture.o gesture_interface.o input.o utility.o queue.o
	$(CC) $(CFLAGS) build/main.o build/config.o build/notify.o build/emotion.o build/phidget.o build/gesture.o build/gesture_interface.o build/input.o build/utility.o $(LIB) $(LIB_OSX) build/queue.o -o build/ebuddy

simulation: main.o config.o notify.o emotion.o simulation.o sim_wrapper.o utility.o queue.o
	$(CC) $(CFLAGS) build/main.o build/config.o build/notify.o build/emotion.o build/simulation.o build/sim_wrapper.o build/utility.o build/queue.o $(LIB) -o build/simulation

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

utility.o: source/utility.c source/utility.h
	$(CC) $(CFLAGS) -c source/utility.c -o build/utility.o

input.o: source/input.c source/input.h
	$(CC) $(CFLAGS) -c source/input.c -o build/input.o

queue.o: source/queue.c source/queue.h
	$(CC) $(CFLAGS) -c source/queue.c -o build/queue.o
	
simulation.o : source/simulation.c source/simulation.h
	$(CC) $(CFLAGS) -c source/simulation.c -o build/simulation.o

sim_wrapper.o : source/sim_wrapper.c
	$(CC) $(CFLAGS) -c source/sim_wrapper.c -o build/sim_wrapper.o
  
# TESTS
test: queue_test config_test notify_test

queue_test: queue_test.o queue.o
	$(CC) $(CFLAGS) build/queue_test.o build/queue.o $(LIB) $(LIB_LINUX) -o build/queue_test

queue_test.o: test/queue_test.c test/test.h source/queue.h
	$(CC) $(CFLAGS) -c test/queue_test.c -o build/queue_test.o $(INCLUDE)
	
config_test: config.o config_test.o
	$(CC) $(CFLAGS) build/config.o build/config_test.o $(LIB) $(LIB_LINUX) -o build/config_test

config_test.o: test/config_test.c test/test.h source/config.h
	$(CC) $(CFLAGS) -c test/config_test.c -o build/config_test.o $(INCLUDE)

notify_test: notify_test.o notify.o config.o queue.o
	$(CC) $(CFLAGS) build/notify_test.o build/notify.o build/config.o build/queue.o $(LIB) $(LIB_LINUX) -o build/notify_test

notify_test.o: test/notify_test.c test/test.h source/notify.h
	$(CC) $(CFLAGS) -c test/notify_test.c -o build/notify_test.o $(INCLUDE)

demo_test.o: test/demo_test.c test/test.h source/main.h
	$(CC) $(CFLAGS) -c test/demo_test.c -o build/demo_test.o $(INCLUDE)
	
clean:
	@rm -rf build/*

checkdir:
	@mkdir build/ -p
