CC = gcc
CFLAGS = -Wall -g -lSDL2 -lSDL2_ttf -lm
BUILD_DIR = build
TARGET = sdl_bezier_curve
SRCS = $(wildcard */*.c)
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))
INC=Inc/
INCLUDES=$(foreach d, $(INC), -I$d)

vpath %.c Src/

all: $(TARGET)
	@echo "Target up to date"

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(INCLUDES) -o $(TARGET)

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