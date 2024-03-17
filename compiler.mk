# GCC Compiler

################################################################################
#    Ccache
#

export CCACHE_DIR=/tmp/ccache/makefile/$(PROJ_NAME)
CCACHE=ccache

################################################################################
#    Toolchain
#

AS  = $(CCACHE) gcc -x assembler-with-cpp
CC  = $(CCACHE) gcc
CXX = $(CCACHE) g++
LD  = gcc
AR  = ar
SZ  = size
OC  = objcopy
NM  = nm

################################################################################
#    Flags
#

# Compiler errors and warnings
CPPFLAGS += -w -Werror -Wall -pedantic-errors -Wshadow -Wextra

# C language specification
CFLAGS   += -std=c99

# C++ language specification
CXXFLAGS += -std=c++98

# Autodependency
CPPFLAGS += -MT $@ -MMD -MP -MF $(@:%.o=%.Td)

# Generate listing
CPPFLAGS += -Wa,-a,-ad,-alms=$(@:%.o=%.lst)

# Use colors with logs
CPPFLAGS += -DLOG_USE_COLOR

# Debug/Release flags
ifeq ($(TARGET),dbg)
  CPPFLAGS+=-g3 -Og -gdwarf-2 -DDEBUG
endif

ifeq ($(TARGET),rel)
  CPPFLAGS+=-O3 -DNDEBUG
endif
