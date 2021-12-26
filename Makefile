# Makefile from:
# https://stackoverflow.com/questions/30573481/how-to-write-a-makefile-with-separate-source-and-header-directories/30602701#30602701

CC := g++

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/fhx_manager
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(info $$SRC is [${SRC}])

$(info $$OBJ is [${OBJ}])

CPPFLAGS := -Iinclude -I../../../build/lirc-0.9.4c/debian/tmp/usr/include -MMD -MP
CFLAGS   := -Wall
LDFLAGS  := -Llib
LDLIBS   := -lm -lstdc++fs -lwiringPi -lpthread -lm -lcurl ./../../../build/lirc-0.9.4c/debian/tmp/usr/lib/arm-linux-gnueabihf/liblirc_client.so

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)