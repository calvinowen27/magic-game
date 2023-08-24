SRC_DIR = src
ECS_DIR = $(SRC_DIR)/ECS
UI_DIR = $(SRC_DIR)/UI

INCLUDE_DIR = include
INCLUDE_GAME = $(INCLUDE_DIR)/game
INCLUDE_ECS = $(INCLUDE_GAME)/ECS
INCLUDE_UI = $(INCLUDE_GAME)/UI
INCLUDE_JSON = $(INCLUDE_DIR)/nlohmann_json

SRC_DIR_O = build/obj
ECS_DIR_O = $(SRC_DIR_O)/ECS
UI_DIR_O = $(SRC_DIR_O)/UI

BUILD_DIR = build/debug

SRC_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR_O)/%.o, $(wildcard $(SRC_DIR)/*.cpp))
ECS_OBJS = $(patsubst $(ECS_DIR)/%.cpp, $(ECS_DIR_O)/%.o, $(wildcard $(ECS_DIR)/*.cpp))
UI_OBJS = $(patsubst $(UI_DIR)/%.cpp, $(UI_DIR_O)/%.o, $(wildcard $(UI_DIR)/*.cpp))

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -pedantic -I$(INCLUDE_DIR)

LDFLAGS = -L./lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lUser32

EXECUTABLE = magic

debug: make_dirs refresh_content refresh_data refresh_world $(SRC_OBJS) $(ECS_OBJS) $(UI_OBJS)
	$(CC) $(SRC_OBJS) $(ECS_OBJS) $(UI_OBJS) $(LDFLAGS) -o $(BUILD_DIR)/$(EXECUTABLE)

# create source objects
$(SRC_DIR_O)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_GAME)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

# create ecs objects
$(ECS_DIR_O)/%.o: $(ECS_DIR)/%.cpp $(INCLUDE_ECS)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(UI_DIR_O)/%.o: $(UI_DIR)/%.cpp $(INCLUDE_UI)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(DBG_DIR)/$(EXECUTABLE) $(SRC_OBJS) $(ECS_OBJS) $(UI_OBJS)

make_dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(SRC_DIR_O)
	mkdir -p $(ECS_DIR_O)
	mkdir -p $(UI_DIR_O)

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