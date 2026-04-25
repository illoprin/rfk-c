CC = clang
CFLAGS = -std=c23 -Wall -ffast-math
LDFLAGS = -lglfw3dll -lglad
PKG = pkg
INCLUDE = -I $(PKG)/include -I $(SRC_DIR)
LIB = -L $(PKG)/lib

SRC_DIR = src
BIN_DIR = build
OBJ_DIR = $(BIN_DIR)/obj
TARGET = $(BIN_DIR)/app.exe

SRCS = $(shell find $(SRC_DIR) -name "*.c")
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(TARGET)
	@cp $(PKG)/lib/*.dll $(BIN_DIR)
	./$<

build: $(TARGET)
	@echo ""

$(TARGET): $(OBJS)
	@echo "📦 Building..."
	$(CC) $(LIB) $^ -o $@ $(LDFLAGS)
	@echo "✅ Build complete!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "📝 Compiling $<..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

.PHONY:
clear:
	@echo "🧹 Cleanup"
	@rm -drf $(BIN_DIR)

.PHONY:
info:
	@echo "🧾 Inputs  $(SRCS)"
	@echo "📦 Outputs $(OBJS)"
	@echo "🎯 Target  $(TARGET)"
