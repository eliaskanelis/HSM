SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -c -o pipefail
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
.DEFAULT_GOAL = all


PROJ:=                  $(shell basename $(CURDIR))

AR :=                   ar
CC :=                   gcc
CXX :=                  g++
LD :=                   gcc
SZ :=                   size

LIBS:=

SRC_DIR:=               src
INC_DIR:=               inc
BIN_DIR:=               bin
OBJ_DIR:=               obj
DOC_DIR:=               doc
TEST_DIR:=              test

RM_RF:=                 rm -rf
MKDIR_P:=               mkdir -p
ECHO:=                  printf "%b"
ECHO_N:=                printf "%b\n"
ECHO_NE:=               printf "%b"
SED:=                   sed

#################################################

C_SOURCES:=             $(shell find $(SRC_DIR) ! -name "*main.c" -name "*.c")
C_SOURCE_MAIN:=         $(shell find $(SRC_DIR) -name "*main.c")

CFLAGS :=               -I$(INC_DIR) \
                        -std=c99 \
                        -Wall -Werror -Wshadow -Wpedantic -Wextra -Wundef \
                        -Wcomments -Wmisleading-indentation \
                        -Wstrict-aliasing -fstrict-aliasing \
                        -Wduplicated-branches \
                        -Wduplicated-cond -Wcast-align -Wdangling-else

CFLAGS +=               -g

# CFLAGS +=               -Wstack-usage=200 -Wbad-function-cast -Wstack-protector


#################################################
# HSM

CFLAGS += -DDEBUG

#################################################

OBJECTS:=               $(addprefix $(OBJ_DIR)/,$(C_SOURCES:%.c=%.o))
MAIN_C_OBJECTS:=        $(addprefix $(OBJ_DIR)/,$(C_SOURCE_MAIN:%.c=%.o))

#################################################
.PHONY: all
all: build tests
	@$(ECHO_N)      ""

.PHONY: run
run: $(BIN_DIR)/$(PROJ)
	@./$<

.PHONY: clean
clean:
	@$(RM_RF)       $(BIN_DIR)
	@$(RM_RF)       $(OBJ_DIR)
	@$(RM_RF)       $(DOC_DIR)

#################################################

.PHONY: build
build: $(BIN_DIR)/$(PROJ)
	@$(ECHO_N)      ""

# Get version number from git
# https://christianhujer.github.io/Git-Version-in-Doxygen/
.PHONY: doc
doc: export PROJECT_NUMBER:=$(shell git describe --always --dirty=" (with uncommitted changes)" --long --tags)
doc:
	@$(ECHO_N) "Building documentation"
	@mkdir -p $(DOC_DIR)/html
	@(cd conf/doxygen/ && doxygen)

#################################################
# Test

TEST_CXX_SOURCES:=$(shell find $(TEST_DIR) -name "*.cpp")
TEST_OBJECTS:=$(addprefix $(OBJ_DIR)/,$(TEST_CXX_SOURCES:%.cpp=%.o))

CPPUTEST_HOME ?= /usr/
TEST_CPPFLAGS += -I$(CPPUTEST_HOME)/include
TEST_CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h
TEST_CFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
TEST_LDFLAGS := -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

PHONY: tests
tests: ##@build Builds the tests.
tests: $(BIN_DIR)/$(PROJ)_runTests
	@$(ECHO)
	@./$< -c | $(SED) 's/^/\t/'

# Build test program
$(BIN_DIR)/$(PROJ)_runTests: $(TEST_OBJECTS) | $(BIN_DIR)/lib$(PROJ).a
	@$(ECHO_N)      "\tCXX $@ $^"
	$(MKDIR_P)       $(dir $@)
	@$(CXX)          $^ $(TEST_LDFLAGS) -o $@ $(BIN_DIR)/lib$(PROJ).a
	@$(ECHO)        ""

# Create test object from C++ source code
obj/%.o: %.cpp Makefile
	@$(ECHO_N)      "\tCXX $@"
	$(MKDIR_P)      $(dir $@)
	@$(CXX)         $< -c $(TEST_CXXFLAGS) $(TEST_CPPFLAGS) -I$(INC_DIR) -o $@
	@$(ECHO)        ""

#################################################
# Build rules

$(BIN_DIR)/lib$(PROJ).a: $(OBJECTS)
	@$(ECHO_N)      "\tAR  $@"
	@$(MKDIR_P)     $(dir $@)
	@$(AR)          -rcs $@ $^
	@$(ECHO)        ""

$(BIN_DIR)/$(PROJ): $(MAIN_C_OBJECTS) $(BIN_DIR)/lib$(PROJ).a
	@$(ECHO_N)      "\tLD  $@"
	@$(MKDIR_P)     $(dir $@)
	@$(LD)          -o $@ $^
	@$(ECHO)        ""
	@$(SZ)          $@

$(OBJ_DIR)/%.o: %.c Makefile
	@$(ECHO_N)      "\tCC  $<"
	@$(MKDIR_P)     $(dir $@)
	@$(CC)          -c -o $@ $< $(CFLAGS)
	@$(ECHO)        ""
