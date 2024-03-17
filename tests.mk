################################################################################
#    Include master makefile
#

#.................................................
#    Paths

TESTSMK_FILEPATH := $(abspath $(lastword $(MAKEFILE_LIST)))
TESTSMK_DIRPATH  := $(dir $(TESTSMK_FILEPATH))

MAKEFILE_FILEPATH := $(TESTSMK_DIRPATH)Makefile


include $(MAKEFILE_FILEPATH)


################################################################################
#    Functionality
#

#.................................................
#    Path

CPPUTEST_DIR :=  $(HOME)/opt/cpputest/

#.................................................
#    Check if tests are available

TESTS_EXIST := $(shell find -maxdepth 1 -type d -name "tests" )

#.................................................
#    Gather of objects

TEST_AS_SRCs  = $(shell find "src/" -name "*.[s|S]")
TEST_C_SRCs   = $(filter-out %main.c,$(shell find "src/" -name "*.[c|C]"))
TEST_CXX_SRCs = $(filter-out %main.cpp,$(shell find "src/" -name "*.cpp"))

ifdef PORT_NAME
  TEST_AS_SRCs  += $(shell find "port/posix/" -name "*.[s|S]")
  TEST_C_SRCs   += $(shell find "port/posix/" -name "*.[c|C]")
  TEST_CXX_SRCs += $(shell find "port/posix/" -name "*.cpp")
endif

ifdef TESTS_EXIST
  TEST_AS_SRCs  += $(shell find "tests/" -name "*.[s|S]")
  TEST_C_SRCs   += $(shell find "tests/" -name "*.[c|C]")
  TEST_CXX_SRCs += $(shell find "tests/" -name "*.cpp")
endif

TEST_OBJS       = $(sort $(TEST_AS_SRCs:%.s=$(TEST_OUTDIR)%.o))
TEST_OBJS      += $(sort $(TEST_C_SRCs:%.c=$(TEST_OUTDIR)%.o))
TEST_OBJS      += $(sort $(TEST_CXX_SRCs:%.cpp=$(TEST_OUTDIR)%.o))

#.................................................
#    Flags

TEST_CPPFLAGS   =
TEST_ASFLAGS    =
TEST_CFLAGS     =
TEST_CXXFLAGS   =
TEST_LDFLAGS    =

TEST_CPPFLAGS  += -I"$(CPPUTEST_DIR)include/"\
                  -I"$(CPPUTEST_DIR)include/CppUTest/MemoryLeakDetectorNewMacros.h"

TEST_LDFLAGS   += -L"$(CPPUTEST_DIR)cpputest_build/lib/"\
                  -lCppUTest\
                  -lCppUTestExt

TEST_CPPFLAGS   += $(CPPFLAGS)
TEST_ASFLAGS    += $(ASFLAGS)
TEST_CFLAGS     += $(CFLAGS)
TEST_CXXFLAGS   += $(CXXFLAGS)
TEST_LDFLAGS    += $(LDFLAGS)

#.................................................
#    Toolchain

TEST_AS  := gcc -x assembler-with-cpp
TEST_CC  := gcc
TEST_CXX := g++
TEST_LD  := g++

TEST_COMPILE.AS  ?= $(TEST_AS)  -c $< -o $@ $(TEST_CPPFLAGS) $(TEST_ASFLAGS)
TEST_COMPILE.CC  ?= $(TEST_CC)  -c $< -o $@ $(TEST_CPPFLAGS) $(TEST_CFLAGS)
TEST_COMPILE.CXX ?= $(TEST_CXX) -c $< -o $@ $(TEST_CPPFLAGS) $(TEST_CXXFLAGS)
TEST_LINK        ?= $(TEST_LD)     $^ -o $@ $(TEST_CPPFLAGS) $(TEST_LDFLAGS)


################################################################################
#    Rules
#

ifdef TESTS_EXIST
.PHONY: runCppUtest
runCppUtest: $(BIN_OUTDIR)runTests
	@$(ECHO_E) $(BLACK)"[TEST] "$(BLUE)"CppUTest"$(RESET)
	@./$< -c
else
.PHONY: runCppUtest
runCppUtest:
	@$(ECHO_NE) $(BLACK)"[TEST] "$(BLUE)"CppUTest "$(RESET)
	@$(ECHO_E) $(YELLOW)"Disabled"$(RESET)
endif

# Build test program
$(BIN_OUTDIR)runTests: $(TEST_OBJS)
	@$(ECHO_NE) $(BLACK)"[TEST] "$(BLUE)"LD  "$(RESET)"$@ "
	@$(MKDIR_P) $(dir $@)
	@$(TEST_LINK) 2>&1 | $(TEE) $(@:%.to=%.terr) | $(XARGS_R0) $(ECHO_E) $(RED)"FAIL\n\n"$(RESET)
	@$(ECHO_E) $(GREEN)"OK"$(RESET)

# Create test object from Assembly source code
$(TEST_OUTDIR)%.o: %.s $(TEST_OUTDIR)%.d $(AUX)
	@$(ECHO_NE) $(BLACK)"[TEST] "$(BLUE)"AS  "$(RESET)"$< "
	@$(MKDIR_P) $(dir $@)
	@$(TEST_COMPILE.AS) 2>&1 | $(TEE) $(@:%.to=%.terr) | $(XARGS_R0) $(ECHO_E) $(RED)"FAIL\n\n"$(RESET)
	@$(MV_F) $(@:%.o=%.Td) $(@:%.o=%.d) && $(TOUCH) $@ || { $(ECHO_E) $(RED)"FAIL\n\n"$(RESET); exit 1; }
	@$(ECHO_E) $(GREEN)"OK"$(RESET)

# Create test object from C source code
$(TEST_OUTDIR)%.o: %.c $(TEST_OUTDIR)%.d $(AUX)
	@$(ECHO_NE) $(BLACK)"[TEST] "$(BLUE)"CC  "$(RESET)"$< "
	@$(MKDIR_P) $(dir $@)
	@$(TEST_COMPILE.CC) 2>&1 | $(TEE) $(@:%.to=%.terr) | $(XARGS_R0) $(ECHO_E) $(RED)"FAIL\n\n"$(RESET)
	@$(MV_F) $(@:%.o=%.Td) $(@:%.o=%.d) && $(TOUCH) $@ || { $(ECHO_E) $(RED)"FAIL\n\n"$(RESET); exit 1; }
	@$(ECHO_E) $(GREEN)"OK"$(RESET)

# Create test object from C++ source code
$(TEST_OUTDIR)%.o: %.cpp $(TEST_OUTDIR)%.d $(AUX)
	@$(ECHO_NE) $(BLACK)"[TEST] "$(BLUE)"CXX "$(RESET)"$< "
	@$(MKDIR_P) $(dir $@)
	@$(TEST_COMPILE.CXX) 2>&1 | $(TEE) $(@:%.to=%.terr) | $(XARGS_R0) $(ECHO_E) $(RED)"FAIL\n\n"$(RESET)
	@$(MV_F) $(@:%.o=%.Td) $(@:%.o=%.d) && $(TOUCH) $@ || { $(ECHO_E) $(RED)"FAIL\n\n"$(RESET); exit 1; }
	@$(ECHO_E) $(GREEN)"OK"$(RESET)

# Show information for debugging purposes
.PHONY: testinfo
testinfo:
	@$(ECHO_E) $(BLUE)"TEST_AS_SRCs:"$(RESET)
	for i in $(sort $(TEST_AS_SRCs)) ; \
	  do \
	    echo "    $$i"; \
	  done
	@$(ECHO_E) $(BLUE)"TEST_C_SRCs:"$(RESET)
	for i in $(sort $(TEST_C_SRCs)) ; \
	  do \
	    echo "    $$i"; \
	  done
	@$(ECHO_E) $(BLUE)"TEST_CXX_SRCs:"$(RESET)
	for i in $(sort $(TEST_CXX_SRCs)) ; \
	  do \
	    echo "    $$i"; \
	  done
	@$(ECHO_E) $(BLUE)"TEST_OBJS:"$(RESET)
	for i in $(TEST_OBJS) ; \
	  do \
	    echo "    $$i"; \
	  done
	@$(ECHO) ""
	@$(ECHO_E) $(BLUE)"TEST_AS:  "$(RESET)$(TEST_COMPILE.AS)
	@$(ECHO_E) $(BLUE)"TEST_CC:  "$(RESET)$(TEST_COMPILE.CC)
	@$(ECHO_E) $(BLUE)"TEST_CXX: "$(RESET)$(TEST_COMPILE.CXX)
	@$(ECHO_E) $(BLUE)"TEST_LD:  "$(RESET)$(TEST_LINK)


################################################################################
#    Auto-depedencies
#

# Manage auto-depedencies( this must be at the end )
TEST_DEPS=$(TEST_OBJS:%.o=%.d)

.PRECIOUS: $(TEST_DEPS)
$(TEST_DEPS): $(AUX)
#	@$(ECHO_E) $(BLACK)"[TEST] "$(BLUE)"D   "$(RESET)"$@ "

-include $(TEST_DEPS)
