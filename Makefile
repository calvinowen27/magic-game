SRC_DIR = src
DBG_DIR = build/debug
ECS_DIR = $(SRC_DIR)/ECS
UI_DIR = $(SRC_DIR)/UI
SPELLS_DIR = $(SRC_DIR)/Spells
INPUT_DIR = $(SRC_DIR)/Input
ANIMATION_DIR = $(SRC_DIR)/Animation

INCLUDE_DIR = include
INCLUDE_GAME = $(INCLUDE_DIR)/game
INCLUDE_ECS = $(INCLUDE_GAME)/ECS
INCLUDE_UI = $(INCLUDE_GAME)/UI
INCLUDE_JSON = $(INCLUDE_DIR)/nlohmann_json
INCLUDE_SPELLS = $(INCLUDE_GAME)/Spells
INCLUDE_INPUT = $(INCLUDE_GAME)/Input
INCLUDE_ANIMATION = $(INCLUDE_GAME)/Animation

SRC_DIR_O = build/obj
ECS_DIR_O = $(SRC_DIR_O)/ECS
UI_DIR_O = $(SRC_DIR_O)/UI
SPELLS_DIR_O = $(SRC_DIR_O)/Spells
INPUT_DIR_O = $(SRC_DIR_O)/Input
ANIMATION_DIR_O = $(SRC_DIR_O)/Animation

BUILD_DIR = build/debug

SRC_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR_O)/%.o, $(wildcard $(SRC_DIR)/*.cpp))
ECS_OBJS = $(patsubst $(ECS_DIR)/%.cpp, $(ECS_DIR_O)/%.o, $(wildcard $(ECS_DIR)/*.cpp))
UI_OBJS = $(patsubst $(UI_DIR)/%.cpp, $(UI_DIR_O)/%.o, $(wildcard $(UI_DIR)/*.cpp))
SPELLS_OBJS = $(patsubst $(SPELLS_DIR)/%.cpp, $(SPELLS_DIR_O)/%.o, $(wildcard $(SPELLS_DIR)/*.cpp))
INPUT_OBJS = $(patsubst $(INPUT_DIR)/%.cpp, $(INPUT_DIR_O)/%.o, $(wildcard $(INPUT_DIR)/*.cpp))
ANIMATION_OBJS = $(patsubst $(ANIMATION_DIR)/%.cpp, $(ANIMATION_DIR_O)/%.o, $(wildcard $(ANIMATION_DIR)/*.cpp))

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -pedantic

LDFLAGS = -L./lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lUser32 -lpthread

EXECUTABLE = magic

debug: make_dirs refresh_content refresh_data refresh_world $(SRC_OBJS) $(ECS_OBJS) $(UI_OBJS) $(SPELLS_OBJS) $(INPUT_OBJS) $(ANIMATION_OBJS)
	$(CC) $(SRC_OBJS) $(ECS_OBJS) $(UI_OBJS) $(SPELLS_OBJS) $(INPUT_OBJS) $(ANIMATION_OBJS) $(LDFLAGS) -o $(BUILD_DIR)/$(EXECUTABLE) -static-libgcc -static-libstdc++

### CREATE OBJECTS ###

$(SRC_DIR_O)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_GAME)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(ECS_DIR_O)/%.o: $(ECS_DIR)/%.cpp $(INCLUDE_ECS)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(UI_DIR_O)/%.o: $(UI_DIR)/%.cpp $(INCLUDE_UI)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(SPELLS_DIR_O)/%.o: $(SPELLS_DIR)/%.cpp $(INCLUDE_SPELLS)/&.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(INPUT_DIR_O)/%.o: $(INPUT_DIR)/%.cpp $(INCLUDE_INPUT)/&.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(ANIMATION_DIR_O)/%.o: $(ANIMATION_DIR)/%.cpp $(INCLUDE_ANIMATION)/&.hpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(DBG_DIR)/$(EXECUTABLE) $(SRC_OBJS) $(ECS_OBJS) $(UI_OBJS) $(SPELLS_OBJS) $(INPUT_OBJS) $(ANIMATION_OBJS)

make_dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(SRC_DIR_O)
	mkdir -p $(ECS_DIR_O)
	mkdir -p $(UI_DIR_O)
	mkdir -p $(SPELLS_DIR_O)
	mkdir -p $(INPUT_DIR_O)
	mkdir -p $(ANIMATION_DIR_O)

refresh_content:
	rm -rf $(BUILD_DIR)/content
	mkdir -p $(BUILD_DIR)/content
	cp -r content $(BUILD_DIR)

refresh_world:
	rm -rf $(BUILD_DIR)/world
	mkdir -p $(BUILD_DIR)/world
	cp -r world $(BUILD_DIR)

refresh_data:
	rm -rf $(BUILD_DIR)/data
	mkdir -p $(BUILD_DIR)/data
	cp -r data $(BUILD_DIR)