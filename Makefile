OS = $(shell uname -s)

CC = gcc
CFLAGS = -Wall -g
LINKER_FLAGS = -lSDL2 -lSDL2_ttf -lm
BUILD_DIR = build
TARGET = sdl_bezier_curve.out
SRCS = $(wildcard */*.c)
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))
INC=Inc/
INCLUDES=$(foreach d, $(INC), -I$d)

ifeq ($(OS), Darwin) 
	INCLUDES += -I/opt/homebrew/Cellar/sdl2/2.30.10/include/SDL2
	INCLUDES += -I/opt/homebrew/Cellar/sdl2_ttf/2.24.0/include/SDL2
	LINKER_FLAGS += -L/opt/homebrew/Cellar/sdl2/2.30.10/lib
	LINKER_FLAGS += -L/opt/homebrew/Cellar/sdl2_ttf/2.24.0/lib
endif

vpath %.c Src/

all: $(TARGET)
	@echo "Target up to date"

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(INCLUDES) -o $(TARGET) $(LINKER_FLAGS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR) 
	$(CC) -c $< -o $@ $(INCLUDES)

$(BUILD_DIR):
	mkdir $@

run: $(TARGET)
	./$(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET)
	rm -rf $(BUILD_DIR)
