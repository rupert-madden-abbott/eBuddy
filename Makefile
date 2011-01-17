CC          := gcc
CFLAGS      := -Wall -pedantic -std=c99

MODULE_DIR  := main notify config gestures phidgets
SOURCE_DIR  := $(addprefix source/,$(MODULE_DIR))
BUILD_DIR   := $(addprefix build/,$(MODULE_DIR))

SOURCE      := $(foreach temp, $(SOURCE_DIR), $(wildcard $(temp)/*.c))
OBJECTS     := $(patsubst source/%.c, build/%.o, $(SOURCE))
INCLUDES    := $(addprefix -I,$(SOURCE_DIR))
LIBRARIES   := -lssl -loauth -lm -lphidget21

vpath %.c $(SOURCE_DIR)

define compile
$1/%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: checkdirs build/ebuddy.exe

build/ebuddy.exe: $(OBJECTS)
	$(CC) $(LIBRARIES) $^ -o $@

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach temp, $(BUILD_DIR), $(eval $(call compile, $(temp))))
