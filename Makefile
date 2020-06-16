TARGET ?= back_end

BUILD ?= build
GEN_DIR ?= gen
SRC_DIR ?= src
BIN_DIR ?= bin
INC_DIR ?= include

SRCS := $(shell find $(SRC_DIR) -name '*.cpp') $(shell find $(GEN_DIR) -name '*.cpp')
OBJS := $(SRCS:%=$(BUILD)/%.o)
DEPS := $(OBJS:.o=.d)

INC_FLAGS := $(addprefix -I,$(INC_DIR)) $(addprefix -I,$(GEN_DIR))

CFLAGS  := -march=native -Og -ggdb $(INC_FLAGS) -MMD -MP -Wall -Werror -Wextra -Wunused -Wconversion
LD_FLAGS := -march=native  -L /usr/lib/ -lboost_system -lboost_thread -lpthread

#CFLAGS  := -march=native -Og -ggdb -Wall -Werror -Wextra -Wunused -Wconversion -Wunused-macros -fno-strict-aliasing $(INC_FLAGS) -MMD -MP -Wold-style-cast
#LD_FLAGS := -march=native --all-warnings -fwhole-program -Wl,-Map,bin/memory.map  -L /usr/lib/ -lboost_system -lboost_thread -lpthread
#-flto in CFLAGS can reduce size, but greatly increases compile time. -O2 for speed.

all:  build

build: $(BIN_DIR)/$(TARGET).exe

$(BIN_DIR)/$(TARGET).exe: $(OBJS)
	$(MKDIR_P) $(dir $@)
	g++ $^ $(LD_FLAGS) -o $@

# c++ source
$(BUILD)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	g++ $(CFLAGS) -c $< -o $@

.PHONY: clean all

clean:
	$(RM) -r $(BUILD)
	$(RM) -r $(BIN_DIR)
	$(RM) $(GEN_DIR)/*
	

-include $(DEPS)

MKDIR_P ?= mkdir -p