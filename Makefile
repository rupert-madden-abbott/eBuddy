CC          := gcc
CFLAGS      := -Wall -Wpointer-arith -Wwrite-strings -Wstrict-prototypes -pedantic -std=gnu99 -g

LIB         := -ljansson -lcurl -loauth -lm
LIB_LINUX   := -lphidget21
LIB_OSX     := -framework Phidget21

INCLUDE     := -Isource/

# BUILDS

all: linux

demo: checkdir config.o demo.o demo_test.o emotion.o gesture.o gesture_interface.o input.o notify.o phidget.o queue.o react.o utility.o 
	$(CC) $(CFLAGS) build/demo build/demo_test.o build/config.o build/demo.o build/emotion.o build/gesture.o build/gesture_interface.o build/input.o build/notify.o build/phidget.o build/queue.o build/utility.o $(LIB) $(LIB_LINUX) -o build/demo

linux: checkdir config.o debug.o demo.o emotion.o phidget.o gesture.o gesture_interface.o input.o main.o mode.o notify.o  queue.o react.o utility.o 
	$(CC) $(CFLAGS) build/config.o build/debug.o build/demo.o build/emotion.o build/gesture.o build/gesture_interface.o build/input.o build/main.o build/mode.o build/notify.o build/phidget.o build/queue.o build/react.o build/utility.o $(LIB) $(LIB_LINUX) -o build/ebuddy

osx: config.o debug.o demo.o emotion.o phidget.o gesture.o gesture_interface.o input.o mode.o main.o notify.o queue.o react.o utility.o
	$(CC) $(CFLAGS) build/config.o build/debug.o build/demo.o build/emotion.o build/gesture.o build/gesture_interface.o build/input.o build/main.o build/mode.o build/notify.o build/phidget.o build/queue.o build/react.o build/utility.o $(LIB) $(LIB_OSX) -o build/ebuddy

simulation: config.o debug.o demo.o emotion.o phidget.o gesture.o gesture_interface.o input.o notify.o main.o mode.o queue.o react.o simulation.o sim_wrapper.o utility.o
	$(CC) $(CFLAGS) build/config.o build/debug.o build/demo.o build/emotion.o build/main.o build/mode.o build/notify.o build/queue.o build/react.o build/simulation.o build/sim_wrapper.o build/utility.o $(LIB) -o build/simulation
	

# MODULES

config.o: source/config.c source/config.h 
	$(CC) $(CFLAGS) -c source/config.c -o build/config.o
	
debug.o: source/debug.c source/debug.h
	$(CC) $(CFLAGS) -c source/debug.c -o build/debug.o

demo.o: source/demo.c source/demo.h 
	$(CC) $(CFLAGS) -c source/demo.c -o build/demo.o

emotion.o: source/emotion.c source/emotion.h
	$(CC) $(CFLAGS) -c source/emotion.c -o build/emotion.o

gesture.o: source/gesture.c source/gesture.h
	$(CC) $(CFLAGS) -c source/gesture.c -o build/gesture.o

gesture_interface.o: source/gesture_interface.c source/gesture_interface.h
	$(CC) $(CFLAGS) -c source/gesture_interface.c -o build/gesture_interface.o

input.o: source/input.c source/input.h
	$(CC) $(CFLAGS) -c source/input.c -o build/input.o
	
main.o: source/main.c source/main.h
	$(CC) $(CFLAGS) -c source/main.c -o build/main.o
	
mode.o: source/mode.c source/mode.h
	$(CC) $(CFLAGS) -c source/mode.c -o build/mode.o	
	
notify.o: source/notify.c source/notify.h
	$(CC) $(CFLAGS) -c source/notify.c -o build/notify.o
	
phidget.o: source/phidget.c source/phidget.h
	$(CC) $(CFLAGS) -c source/phidget.c -o build/phidget.o

queue.o: source/queue.c source/queue.h
	$(CC) $(CFLAGS) -c source/queue.c -o build/queue.o
	
react.o: source/react.c source/react.h
	$(CC) $(CFLAGS) -c source/react.c -o build/react.o
	
simulation.o : source/simulation.c source/simulation.h
	$(CC) $(CFLAGS) -c source/simulation.c -o build/simulation.o

sim_wrapper.o : source/sim_wrapper.c
	$(CC) $(CFLAGS) -c source/sim_wrapper.c -o build/sim_wrapper.o
	
utility.o: source/utility.c source/utility.h
	$(CC) $(CFLAGS) -c source/utility.c -o build/utility.o

  
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
