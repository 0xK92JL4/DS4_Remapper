BIN_DIR   := bin
SRC_DIR   := src
OBJ_DIR   := .obj

CXX      := c++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3 -Iinclude $(shell pkg-config --cflags libevdev)
LDFLAGS  := $(shell pkg-config --libs libevdev) #-lpthread

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

TARGET := $(BIN_DIR)/ds4-remapper

all: $(TARGET)


$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)


$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Linking $@"
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

re: clean all

run: all
	sudo ./$(TARGET)


.PHONY: all clean re run
