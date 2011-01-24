CC          := gcc
CFLAGS      := -Wall -pedantic -std=gnu99 -g

MODULE_DIR  := main notify config gestures phidgets emotion
SOURCE_DIR  := $(addprefix source/,$(MODULE_DIR))
BUILD_DIR   := $(addprefix build/,$(MODULE_DIR))

SOURCE      := $(foreach temp, $(SOURCE_DIR), $(wildcard $(temp)/*.c))
OBJECTS     := $(patsubst source/%.c, build/%.o, $(SOURCE))
INCLUDES    := $(addprefix -I,$(SOURCE_DIR))
LIBRARIES   := -lssl -loauth -lm -lphidget21 -ljansson -lcurl
LIBRARIES_MAC:= -lssl -loauth -lm -ljansson -lcurl -framework Phidget21

vpath %.c $(SOURCE_DIR)

define compile
$1/%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $$< -o $$@
endef

.PHONY: all checkdirs clean mac debug

all: checkdirs build/ebuddy.exe
	
mac: checkdirs build/ebuddy 

build/ebuddy.exe: $(OBJECTS)
	$(CC) $(LIBRARIES) $^ -o $@
	
build/ebuddy: $(OBJECTS)
	$(CC) $(LIBRARIES_MAC) $^ -o $@

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach temp, $(BUILD_DIR), $(eval $(call compile, $(temp))))
