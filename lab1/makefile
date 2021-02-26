PROGRAMM = lab1.elf
BUILD_DIR = bin
SRC_DIR = src
SRC = $(shell find $(SRC_DIR) -name '*.cpp' -not -path '$(SRC_DIR)/tests/*')
INCLUDES = $(shell find $(SRC_DIR) -name '*.h' -not -path '$(SRC_DIR)/tests/*')
DIRS = $(shell find $(SRC_DIR)/ -type d)
TEST_SRC = $(shell find $(SRC_DIR) -name '*.cpp' -not -path '$(SRC_DIR)/main.cpp')
TEST_INCLUDES = $(shell find $(SRC_DIR) -name '*.h')

CC = g++
CFLAGS = -std=gnu++17 -Wall -Werror -pedantic
LDFLAGS = -lpthread

OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(patsubst %.cpp,%.o,$(SRC)))
TEST_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(patsubst %.cpp,%.o,$(TEST_SRC)))
DIRFLAGS = $(addprefix -I,$(DIRS))
PROG = $(BUILD_DIR)/$(PROGRAMM)
TEST_PROG = $(BUILD_DIR)/test_$(PROGRAMM)

ifdef DEBUG
CFLAGS += -g
endif

ifdef OPT
CFLAGS += -O0
endif

all: clean

.PHONY: unix
unix: $(PROG)

.PHONY: tests
tests: unit_tests

.PHONY: unit_tests
unit_tests: $(TEST_PROG)
	$(TEST_PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(TEST_PROG): $(TEST_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDES)
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(DIRFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf bin