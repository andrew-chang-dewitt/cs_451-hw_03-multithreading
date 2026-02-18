CC         = gcc

# from https://stackoverflow.com/questions/154630/recommended-gcc-warning-options-for-c
CFLAGS    += -Wall -Wextra -Wformat=2 -Wswitch-default -Wcast-align \
	     -Wpointer-arith -Wbad-function-cast -Wstrict-prototypes -Winline \
	     -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wwrite-strings \
	     -Wconversion -Wunreachable-code -Wstrict-aliasing=2 -fno-common \
	     -fstrict-aliasing -std=c99 -pedantic
	     
DBG_FLAGS  = -O0 -g -include stdbool.h
REL_FLAGS  = -O3
PRF_FLAGS  = -pg

LOC       := $(shell readlink -f .)
PRJ_ROOT  := $(LOC)
TGT_DIR   := $(PRJ_ROOT)/target

REL_DIR   := /release
DBG_DIR   := /debug


ifeq ($(DBG),true)
	CFLAGS  += $(DBG_FLAGS)
	OUT_DIR  = $(TGT_DIR)$(DBG_DIR)
else ifeq ($(PRF),true)
	CFLAGS  += $(PRF_FLAGS)
	OUT_DIR  = $(TGT_DIR)$(DBG_DIR)
else
	CFLAGS  += $(REL_FLAGS)
	OUT_DIR  = $(TGT_DIR)$(REL_DIR)
endif

BIN_DIR   := $(OUT_DIR)/bin
OBJ_DIR   := $(OUT_DIR)/obj
DEP_DIR   := $(OUT_DIR)/dep
SRC_DIR   := $(LOC)

# gcc flags to automatically generate dependencies, from
# https://make.mad-scientist.net/papers/advanced-auto-dependency-generation/#tldr
DEP_FLAGS  = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d

NAMES     := life multithreaded_life granular_multithreaded_life
SRCS      := $(NAMES:%=$(SRC_DIR)/%.c)
BINS      := $(NAMES:%=$(BIN_DIR)/%)
DEPS := $(NAMES:%=$(DEP_DIR)/%.d)


.PHONY: clean all life multi gran list_bins

all: life multi gran


life: $(BIN_DIR)/life


multi: $(BIN_DIR)/multithreaded_life


gran: $(BIN_DIR)/granular_multithreaded_life


list:
	@echo $(TGT_DIR)
	@echo $(OUT_DIR)
	@echo $(BIN_DIR)
	@echo $(OBJ_DIR)
	@echo $(DEP_DIR)
	@echo $(NAMES)
	@echo $(SRCS)
	@echo $(BINS)
	@echo $(DEPS)


# automatically generate dependencies, from
# https://make.mad-scientist.net/papers/advanced-auto-dependency-generation/#tldr
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(DEP_DIR)/%.d | $(DEP_DIR)
	$(CC) $(DEP_FLAGS) $(CFLAGS) $<


$(DEPS):


include $(wildcard $(DEPS))


# uses secondary expansion to allow grabbing executable name
# from target in $(BINS) see
# https://www.gnu.org/software/make/manual/html_node/Secondary-Expansion.html
# .SECONDEXPANSION:
$(BIN_DIR)/%: $(SRC_DIR)/%.c $(DEP_DIR)/%.d | $(BIN_DIR) $(DEP_DIR) $(OBJ_DIR)
	@echo "building binary $@ from $^..."
	$(CC) $(CFLAGS) $(CMPT_FLAGS) $< $(OBJS) -o $@
	@echo "... $@ built."

$(BIN_DIR) $(OBJ_DIR) $(DEP_DIR): %:
	@mkdir -p $@

clean:
	rm -rf $(TGT_DIR)
