PROGRAM_NAME = tap
GUI_PROGRAM_NAME = tap_gui_client

ENTRY = main.cpp
GUI_ENTRY = gui/src/main.cpp

SRC_FOLDER = src
INC_FOLDER = inc
OBJ_FOLDER = obj

BATTLE_FOLDER = battle
BATTLE_SRC = $(addprefix $(SRC_FOLDER)/$(BATTLE_FOLDER)/, \
				Battle.cpp)

CHARACTERS_FOLDER = characters
CHARACTERS_SRC = $(addprefix $(SRC_FOLDER)/$(CHARACTERS_FOLDER)/, \
					Character.cpp Enchanter.cpp Enemy.cpp EnemyFactory.cpp \
					Fighter.cpp Goblin.cpp Merchant.cpp Narrator.cpp \
					NPC.cpp Player.cpp QuestGiver.cpp)

ENCHANTMENTS_FOLDER = enchantments
ENCHANTMENTS_SRC = $(addprefix $(SRC_FOLDER)/$(ENCHANTMENTS_FOLDER)/, \
						Enchantment.cpp EnchantmentFactory.cpp Flame.cpp Freeze.cpp)

ITEMS_FOLDER = items
ITEMS_SRC = $(addprefix $(SRC_FOLDER)/$(ITEMS_FOLDER)/, \
				Apple.cpp Armor.cpp Chest.cpp ChestKey.cpp Consumable.cpp \
				FlameSword.cpp Gear.cpp Inventory.cpp IronArmor.cpp \
				IronShield.cpp IronSword.cpp ItemFactory.cpp Item.cpp \
				Shield.cpp SpikeArmor.cpp Weapon.cpp)

# MODIFIED: esta carpeta entera faltaba en el Makefile -- CommandParser y
# CommandHandler no se estaban compilando en absoluto, de ahi los
# "undefined reference" al enlazar.
COMMANDS_FOLDER = commands
COMMANDS_SRC = $(addprefix $(SRC_FOLDER)/$(COMMANDS_FOLDER)/, \
					command.cpp CommandHandler.cpp commandparser.cpp)

PARSER_FOLDER = parser
PARSER_SRC = $(addprefix $(SRC_FOLDER)/$(PARSER_FOLDER)/, \
				parser.cpp)

QUESTS_FOLDER = quests
QUESTS_SRC = $(addprefix $(SRC_FOLDER)/$(QUESTS_FOLDER)/, \
				Quest.cpp)

SERVER_FOLDER = server
SERVER_SRC = $(addprefix $(SRC_FOLDER)/$(SERVER_FOLDER)/, \
				PlayerConnection.cpp Server.cpp ServerOwner.cpp)

UTILS_FOLDER = utils
UTILS_SRC = $(addprefix $(SRC_FOLDER)/$(UTILS_FOLDER)/, \
				colors.cpp globals.cpp log.cpp string_utils.cpp \
				types.cpp)

WORLD_FOLDER = world
WORLD_SRC = $(addprefix $(SRC_FOLDER)/$(WORLD_FOLDER)/, \
				Room.cpp World.cpp)

# MODIFIED: GuiClient es parte del motor compartido -- se compila con las
# mismas reglas/flags que el resto (no necesita SDL2/ImGui para compilar,
# solo el propio motor de juego), asi que entra en el mismo SRC/OBJS.
GUI_CLIENT_FOLDER = gui
GUI_CLIENT_SRC = $(addprefix $(SRC_FOLDER)/$(GUI_CLIENT_FOLDER)/, \
					GuiClient.cpp)

SRC = $(BATTLE_SRC) $(CHARACTERS_SRC) $(ENCHANTMENTS_SRC) $(ITEMS_SRC) $(QUESTS_SRC) $(SERVER_SRC) $(UTILS_SRC) $(WORLD_SRC) $(COMMANDS_SRC) $(GUI_CLIENT_SRC)

OBJS = $(SRC:$(SRC_FOLDER)/%.cpp=$(OBJ_FOLDER)/%.o)

FLAGS = $(VERSION_FLAG) -g -fsanitize=thread -I $(INC_FOLDER) -pthread
VERSION_FLAG = -std=c++17
COMPILATION_FLAGS = -Wall -Wextra -Werror -Wunused-parameter
CRAZY_FLAGS = -Wpedantic -Wshadow -Wconversion -Wsign-conversion \
				-Wcast-align -Wold-style-cast -Woverloaded-virtual \
				-Wnon-virtual-dtor -D_POSIX_C_SOURCE=200809L -O3 \
				-march=native -flto
# -MMD -MP (revisar)
# DEPS = $(OBJS:.o=.d)
# -include $(DEPS)
DEBUG_FLAG = -D WOW -D DEBUG_BUILD
CC = c++

# MODIFIED: carpeta de objetos aparte para el build SIN DEBUG_BUILD (el
# servidor real, normal_mode). Necesaria porque los .o de obj/ ya llevan
# DEBUG_BUILD horneado -- no se pueden reutilizar para esto.
SERVER_OBJ_FOLDER = obj_server
SERVER_OBJS = $(SRC:$(SRC_FOLDER)/%.cpp=$(SERVER_OBJ_FOLDER)/%.o)
SERVER_PROGRAM_NAME = tap_server

COLOR_RESET			:= \033[0m
COLOR_GRAY			:= \033[0;30m
COLOR_LIGHT_GRAY	:= \033[1;30m
COLOR_RED			:= \033[0;31m
COLOR_GREEN			:= \033[0;32m
COLOR_LIGHT_GREEN	:= \033[1;32m
COLOR_YELLOW		:= \033[0;33m
COLOR_LIGHT_YELLOW	:= \033[1;33m
COLOR_BLUE			:= \033[0;34m
COLOR_LIGHT_BLUE	:= \033[1;34m
COLOR_MAGENTA		:= \033[0;35m
COLOR_CYAN			:= \033[0;36m
COLOR_WHITE			:= \033[0;37m
BOLD				:= \033[1m
BOLD_OFF			:= \033[22m
UNDERLINE			:= \033[4m
UNDERLINE_OFF		:= \033[24m

# --- MODIFIED: GUI-only build settings (ImGui + SDL2) ----------------------
# Discusion que tengo con Claudia sobre si deberiamos o no mantener el proyecto compilable
# sin tener ImGui accesible. Creo que es algo que requiere el subject, asi que como es mantener
# normas especificas que se pueden unificar, lo voy a dejar.
IMGUI_DIR = external/imgui
IMGUI_TAG = v1.90.9
IMGUI_SRC = $(addprefix $(IMGUI_DIR)/, imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp) \
			$(addprefix $(IMGUI_DIR)/backends/, imgui_impl_sdl2.cpp imgui_impl_opengl3.cpp)
IMGUI_OBJS = $(IMGUI_SRC:%.cpp=$(OBJ_FOLDER)/%.o)

SDL_CFLAGS = $(shell pkg-config --cflags sdl2 2>/dev/null || sdl2-config --cflags)
SDL_LIBS = $(shell pkg-config --libs sdl2 2>/dev/null || sdl2-config --libs)
GUI_INC = -I $(IMGUI_DIR) -I $(IMGUI_DIR)/backends
GUI_FLAGS = $(VERSION_FLAG) -g -fsanitize=thread -I $(INC_FOLDER) $(GUI_INC) $(SDL_CFLAGS) -pthread
GL_LIBS = -lGL

# DE AQUI PARA ABAJO ESTA COMO LA MIERDA // Ya no tanto

help:
	# TODO: Explanation of each command

# MODIFIED: ademas de json.hpp, ahora tambien trae ImGui si no esta ya.
install:
	curl -o inc/libs/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
	@if [ ! -d "$(IMGUI_DIR)" ]; then \
		echo "Cloning Dear ImGui $(IMGUI_TAG)..."; \
		git clone --depth 1 --branch $(IMGUI_TAG) https://github.com/ocornut/imgui.git $(IMGUI_DIR); \
	else \
		echo "Dear ImGui already present at $(IMGUI_DIR), skipping."; \
	fi

# MODIFIED: run ahora es la entrada base para el proyecto, antes lanzaba valgrind por alguna razon
run: gui
	./$(GUI_PROGRAM_NAME)

valgrind-run: all
	valgrind ./$(PROGRAM_NAME) 2>result.txt

$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(DEBUG_FLAG) -c $< -o $@

# MODIFIED: misma regla, pero SIN DEBUG_FLAG -- esta es la que de verdad
# produce current_level = WARNING, es decir, normal_mode() (el servidor
# real con sockets) en vez de debug_mode().
$(SERVER_OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

# MODIFIED: regla de compilacion para los .cpp de ImGui (A revisar)
$(OBJ_FOLDER)/$(IMGUI_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(VERSION_FLAG) $(GUI_INC) $(SDL_CFLAGS) -O2 -c $< -o $@

all: $(OBJS)
	$(CC) $(FLAGS) $(ENTRY) $(OBJS) -o $(PROGRAM_NAME)

compile-debug: $(OBJS)
	$(CC) $(FLAGS) $(ENTRY) $(OBJS) $(DEBUG_FLAG) -o $(PROGRAM_NAME)

# MODIFIED: el servidor real (normal_mode). "tap" y "tap_gui_client" siguen
# siendo siempre debug_mode/modo local -- este es el unico que abre el
# socket TCP de verdad.
server: $(SERVER_OBJS)
	$(CC) $(FLAGS) $(ENTRY) $(SERVER_OBJS) -o $(SERVER_PROGRAM_NAME)

run-server: server
	./$(SERVER_PROGRAM_NAME)

# MODIFIED: build de la GUI, unificado aqui (antes vivia en gui/Makefile).
gui: $(OBJS) $(IMGUI_OBJS)
	$(CC) $(GUI_FLAGS) $(GUI_ENTRY) $(OBJS) $(IMGUI_OBJS) $(SDL_LIBS) $(GL_LIBS) -o $(GUI_PROGRAM_NAME)

clean:
	rm -rf $(OBJ_FOLDER) $(SERVER_OBJ_FOLDER)
	rm -rf external/imgui

fclean: clean
	rm -f $(PROGRAM_NAME) $(GUI_PROGRAM_NAME) $(SERVER_PROGRAM_NAME)

re: clean all

.PHONY: help install run valgrind-run run-server clean fclean re gui all compile-debug server

.DEFAULT_GOAL= all
