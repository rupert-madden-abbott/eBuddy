# This is a generalised makefile that will enable easier maintenance as the    
# project gets larger. If you don't want to understand how it works just do    
# the following: 
#		# Only compile with make. 
#		# To remove generated output, use make clean.
# 	# Add c files to SOURCE_DIR, h files to INCLUDE_DIR, a files to LIBRARY_DIR
#		# Add external libraries by altering the LIBS variable
#		# Add 1 o file for every c file to the OBJS variable

LIBS = -lm
OBJS = hello.o

INCLUDE_DIR = inc
OUTPUT_DIR = out
SOURCE_DIR = src
LIBRARY_DIR = lib

################################################################################
# Everything beyond this point you can ignore unless you want to understand    #
# the magic.                                                                   #
################################################################################

# Alters OBJS by prefixing each with the OUTPUT_DIR
_OBJS = $(patsubst %,$(OUTPUT_DIR)/%,$(OBJS))

# Sets the compiler to be used
CC = gcc

# Sets the flags for the compiler
CFLAGS = -Wall -pedantic -std=c99 -I$(INCLUDE_DIR)

# Shortcuts for the linking and compiling steps
LINK = $(CC) $(CFLAGS)
COMPILE = $(CC) $(CFLAGS) -c

# Default entry point. Required by makefile standards.
all: ebuddy

# This rule tells make to ensure all of the headers are supplied as
# dependencies to the relevant source files. % is a wildcard. $@ is shorthand # for the target. $< is shorthand for the dependencies.
$(OUTPUT_DIR)/%.o: $(SOURCE_DIR)/%.c $(INCLUDE_DIR)/%.h
	$(COMPILE) -o $@ $<

# Link all of the object files into the program along with any external 
# libraries and output to the OUTPUT_DIR
ebuddy: $(_OBJS)
	$(LINK) -o $(OUTPUT_DIR)/$@ $< $(LIBS)

# make clean will quickly remove all generated files. .PHONY prevents any 
# potential naming conflicts with files called clean.		
.PHONY: clean

clean:
	rm -f $(OUTPUT_DIR)/*
