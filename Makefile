SRC_DIR = src
ECS_DIR = $(SRC_DIR)/ECS
INCLUDE_DIR = include/game
SRC_DIR_O = build/obj
ECS_DIR_O = $(SRC_DIR_O)/ECS
DBG_DIR = build/debug

SRC = $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR_O)/%.o, $(SRC))

ECS = $(wildcard $(ECS_DIR)/*.cpp)
ECS_OBJS = $(patsubst $(ECS_DIR)/%.cpp, $(ECS_DIR_O)/%.o, $(ECS))

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -pedantic

LDFLAGS = -L./lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lUser32

EXECUTABLE = magic

debug: $(SRC_OBJS) $(ECS_OBJS) $(DBG_DIR)
	rm -rf $(DBG_DIR)/content
	mkdir -p $(DBG_DIR)/content
	cp -r content $(DBG_DIR)
	$(CC) $(SRC_OBJS) $(ECS_OBJS) $(LDFLAGS) -o $(DBG_DIR)/$(EXECUTABLE)

$(SRC_DIR_O)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(ECS_DIR_O)/%.o: $(ECS_DIR)/%.cpp $(INCLUDE_DIR)/ECS/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR_O) $(ECS_DIR_O) $(DBG_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -f $(DBG_DIR)/$(EXECUTABLE) $(SRC_OBJS) $(ECS_OBJS)