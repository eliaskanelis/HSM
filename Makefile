###############################################################################
#	Toolchain
###############################################################################

CC = gcc
AS = gcc -x assembler-with-cpp
CP = objcopy
AR = ar
SZ = size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

###############################################################################
#	Configuration
###############################################################################

# Target name
TARGET = HSM
# Build path
BUILD_DIR = bin

# Debug build?
DEBUG = 1
# Optimization
OPT = -Og

###############################################################################
#	Definitions
###############################################################################

# AS defines
AS_DEFS = 

# C defines
C_DEFS = 

###############################################################################
#	FLAGS
###############################################################################

# GCC AS flags
ASFLAGS = $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

# GCC C flags
CFLAGS = $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Werror -fdata-sections -ffunction-sections
# Debug flags
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif
# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

# Linker flags
LDFLAGS = 

###############################################################################
#	Dependencies
###############################################################################	

#######################################
#	C
C_INCLUDES +=	-Iinc

C_SOURCES +=	src/main.c

#######################################
#	Application
C_INCLUDES +=	-Isrc

C_SOURCES +=	src/console.c\
				src/hsm.c\
				src/newSys.c

# Get version number from git
# https://christianhujer.github.io/Git-Version-in-Doxygen/
doc: export PROJECT_NUMBER:=$(shell git describe --always --dirty=" (with uncommitted changes)" --long --tags)

###############################################################################
#	Rules and dependencies
###############################################################################

.PHONY: default all options doc flash erase clean size run

# Defauft rule
default: options version $(BUILD_DIR)/$(TARGET).out size

all: default doc

options:
	@echo Making hierarchical state machine
	@echo
	@echo Compiler:	$(CC)
	@echo CFlags:	$(CFLAGS)

version: Makefile
	scripts/get_version.sh

doc: Makefile | default
	mkdir -p docs
	@(cd conf/doxygen/ && doxygen)

clean:
	-rm -fR .dep $(BUILD_DIR)
	-rm -fR inc/version.h

size:
	@echo
	$(SZ) $(BUILD_DIR)/$(TARGET).out
	@echo
	@echo "Build successfully!"

run:
	./$(BUILD_DIR)/$(TARGET).out

###############################################################################
#	Build the application
###############################################################################

# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).out: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@	
	
$(BUILD_DIR):
	mkdir $@

###############################################################################
#	Dependencies
###############################################################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***