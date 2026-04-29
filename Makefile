CXX = clang
OPTIMIZATION = -O2
CPPLIB = -lstdc++ # what the fuck???
FMS_EXT = -fms-extensions # nested structs
CFLAGS = -std=c23 -Wall -ffast-math $(OPTIMIZATION) $(FMS_EXT)
LDFLAGS = -lcimgui -lglfw3dll -lglad $(CPPLIB)
PKG = pkg
INCLUDE = -I $(PKG)/include -I $(SRC_DIR)
LIB = -L $(PKG)/lib -L $(PKG)/lib/imgui

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
	$(CXX) $(LIB) $^ -o $@ $(LDFLAGS)
	@echo "✅ Build complete!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "📝 Compiling $<..."
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@ $(INCLUDE)

.PHONY:
clear:
	@echo "🧹 Cleanup"
	@rm -drf $(BIN_DIR)

.PHONY:
info:
	@echo "🧾 Inputs  $(SRCS)"
	@echo "📦 Outputs $(OBJS)"
	@echo "🎯 Target  $(TARGET)"
