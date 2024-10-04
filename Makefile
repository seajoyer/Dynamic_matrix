CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
TEST_DIR := tests

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))

MAIN_OBJ := $(BUILD_DIR)/main.o

TARGET := $(BUILD_DIR)/dynamic_matrix
TEST_TARGET := $(BUILD_DIR)/run_tests

.PHONY: all clean help run test

all: $(TARGET)

test: $(TEST_TARGET)
	@$(TEST_TARGET)

$(TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ -I$(INC_DIR)

$(TEST_TARGET): $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -I$(INC_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I$(INC_DIR)

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I$(INC_DIR)

$(MAIN_OBJ): main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I$(INC_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	@$(TARGET)

help:
	@echo "Usage:"
	@echo "  make         # Build the project"
	@echo "  make run     # Build and run the project"
	@echo "  make test    # Build and run the tests"
	@echo "  make clean   # Remove build artifacts and temporary files"
	@echo "  make help    # Show this help message"
	@echo ""
	@echo "Current settings:"
	@echo "  CXX        = $(CXX)"
	@echo "  CXXFLAGS   = $(CXXFLAGS)"

print-%:
	@echo $* = $($*)
