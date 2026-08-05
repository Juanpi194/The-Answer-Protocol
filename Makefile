PROGRAM_NAME = tap
TUI_PROGRAM_NAME = tap_tui_client
GUI_PROGRAM_NAME = tap_gui_client

ENTRY = main.cpp
TUI_ENTRY = ui/tui/main.cpp
GUI_ENTRY = ui/gui/main.cpp

SRC_FOLDER = src
INC_FOLDER = inc
OBJ_FOLDER = obj

BATTLE_SRC = $(addprefix $(SRC_FOLDER)/battle/, \
				Battle.cpp)

CHARACTERS_SRC = $(addprefix $(SRC_FOLDER)/characters/, \
					Character.cpp Enchanter.cpp Enemy.cpp Fighter.cpp \
					Merchant.cpp NPC.cpp Narrator.cpp Player.cpp \
					QuestGiver.cpp enemies/Goblin.cpp enemies/Ogre.cpp \
					enemies/Shade.cpp enemies/Shadow.cpp enemies/Wizard.cpp \
					enemies/Dragon.cpp)

ENCHANTMENTS_SRC = $(addprefix $(SRC_FOLDER)/enchantments/, \
						Enchantment.cpp Flame.cpp Freeze.cpp)

ITEMS_SRC = $(addprefix $(SRC_FOLDER)/items/, \
				Armor.cpp Consumable.cpp Gear.cpp Inventory.cpp Item.cpp \
				Shield.cpp Weapon.cpp \
				armor/BronzeArmor.cpp armor/EmberArmor.cpp \
				armor/FrostArmor.cpp armor/IronArmor.cpp \
				armor/SpikeArmor.cpp armor/SteelArmor.cpp \
				chest/Chest.cpp chest/ChestKey.cpp \
				consumables/Apple.cpp consumables/FirePotion.cpp \
				consumables/HastePotion.cpp consumables/HealingPotion.cpp \
				consumables/IcePotion.cpp consumables/PoisonPotion.cpp \
				shields/BronzeShield.cpp shields/IronShield.cpp \
				shields/SpikeShield.cpp shields/SteelShield.cpp \
				weapons/BronzeSword.cpp weapons/FlameSword.cpp \
				weapons/FrostSword.cpp weapons/IronSword.cpp \
				weapons/SerratedSword.cpp weapons/SteelSword.cpp)

FACTORIES_SRC = $(addprefix $(SRC_FOLDER)/factories/, \
					EnchantmentFactory.cpp EnemyFactory.cpp ItemFactory.cpp \
					NpcPacificFactory.cpp)

COMMANDS_SRC = $(addprefix $(SRC_FOLDER)/commands/, \
					command.cpp CommandHandler.cpp commandparser.cpp)

PROTOCOL_SRC = $(addprefix $(SRC_FOLDER)/protocol/, \
					events.cpp responses.cpp)

GROUP_SRC = $(addprefix $(SRC_FOLDER)/group/, \
				Group.cpp)

QUESTS_SRC = $(addprefix $(SRC_FOLDER)/quests/, \
				Quest.cpp)

SERVER_SRC = $(addprefix $(SRC_FOLDER)/server/, \
				PlayerConnection.cpp Server.cpp ServerOwner.cpp)

UTILS_SRC = $(addprefix $(SRC_FOLDER)/utils/, \
				colors.cpp globals.cpp log.cpp string_utils.cpp types.cpp)

WORLD_SRC = $(addprefix $(SRC_FOLDER)/world/, \
				Room.cpp World.cpp)

UI_CLIENT_SRC = $(addprefix $(SRC_FOLDER)/ui/, \
					CLI.cpp)

PARSER_SRC = $(addprefix $(SRC_FOLDER)/parser/, \
				jsonconfig.cpp itemparser.cpp npcparser.cpp roomparser.cpp)

SRC = $(BATTLE_SRC) $(CHARACTERS_SRC) $(ENCHANTMENTS_SRC) $(ITEMS_SRC) $(FACTORIES_SRC) $(QUESTS_SRC) $(SERVER_SRC) $(UTILS_SRC) $(WORLD_SRC) $(COMMANDS_SRC) $(PROTOCOL_SRC) $(GROUP_SRC) $(PARSER_SRC) $(UI_CLIENT_SRC)

OBJS = $(SRC:$(SRC_FOLDER)/%.cpp=$(OBJ_FOLDER)/%.o)

# MODIFIED: sin -fsanitize=thread -- lo probamos con la GUI y ThreadSanitizer
# instrumenta TODOS los hilos del proceso, incluidos los que crea el driver
# grafico de Mesa/AMD al usar OpenGL/ImGui. Eso ralentiza tanto la GUI que
# el sistema operativo la marca como "no responde", aunque el bucle siga
# corriendo por dentro (lo confirmamos con trazas de debug -- no era ningun
# deadlock nuestro, era sobrecarga de TSan sobre codigo que no es nuestro).
# Un solo arbol de objetos otra vez, para todo -- server, GUI y TUI.
FLAGS = $(VERSION_FLAG) -g -I $(INC_FOLDER) -pthread
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

IMGUI_DIR = external/imgui
IMGUI_TAG = v1.90.9
IMGUI_SRC = $(addprefix $(IMGUI_DIR)/, imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp) \
			$(addprefix $(IMGUI_DIR)/backends/, imgui_impl_sdl2.cpp imgui_impl_opengl3.cpp)
IMGUI_OBJS = $(IMGUI_SRC:%.cpp=$(OBJ_FOLDER)/%.o)

SDL_CFLAGS = $(shell pkg-config --cflags sdl2 2>/dev/null || sdl2-config --cflags)
SDL_LIBS = $(shell pkg-config --libs sdl2 2>/dev/null || sdl2-config --libs)
GUI_INC = -I $(IMGUI_DIR) -I $(IMGUI_DIR)/backends
# MODIFIED: sin -fsanitize=thread aqui -- TSan instrumenta TODO el proceso,
# incluidos los hilos internos del driver grafico de Mesa (shader compiler,
# hilo de GL...), y eso ralentiza tanto la GUI que el SO la marca como "no
# responde" aunque el bucle siga corriendo (confirmado con las trazas de
# debug: los 4 mensajes se repetian sin parar, no habia ningun deadlock).
# El hilo de CLI (lo que de verdad nos interesa vigilar con TSan) se puede
# probar via tap_tui, que usa la misma clase sin el coste del driver.
GUI_FLAGS = $(VERSION_FLAG) -g -I $(INC_FOLDER) $(GUI_INC) $(SDL_CFLAGS) -pthread
GL_LIBS = -lGL

help:
	@echo "Available targets:"
	@echo "  make / make all   - Compile all 3 executables: server, gui, tui"
	@echo "  make server       - Compile only the server binary ($(PROGRAM_NAME))"
	@echo "  make gui          - Compile only the GUI client ($(GUI_PROGRAM_NAME)) -- run 'make install' first on a fresh checkout"
	@echo "  make tui          - Compile only the TUI client ($(TUI_PROGRAM_NAME))"
	@echo "  make run-server   - Compile (if needed) and launch the server"
	@echo "  make run-gui      - Compile (if needed) and launch the GUI client"
	@echo "  make run-tui      - Compile (if needed) and launch the TUI client"
	@echo "  make debug-mode   - Compile (if needed) and launch the server in debug_mode (--debug)"
	@echo "  make valgrind-run - Compile (if needed) and run the server under valgrind"
	@echo "  make install      - Fetch external dependencies (json.hpp, Dear ImGui)"
	@echo "  make clean        - Remove compiled object files"
	@echo "  make fclean       - clean + remove binaries + remove installed dependencies (json.hpp, external/imgui)"
	@echo "  make re           - fclean + all"

install:
	curl -o inc/libs/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
	@if [ ! -d "$(IMGUI_DIR)" ]; then \
		echo "Cloning Dear ImGui $(IMGUI_TAG)..."; \
		git clone --depth 1 --branch $(IMGUI_TAG) https://github.com/ocornut/imgui.git $(IMGUI_DIR); \
	else \
		echo "Dear ImGui already present at $(IMGUI_DIR), skipping."; \
	fi

valgrind-run: server
	valgrind ./$(PROGRAM_NAME) 2>result.txt

$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp | install
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(DEBUG_FLAG) -c $< -o $@

$(OBJ_FOLDER)/$(IMGUI_DIR)/%.o: $(IMGUI_DIR)/%.cpp | install
	mkdir -p $(dir $@)
	$(CC) $(VERSION_FLAG) $(GUI_INC) $(SDL_CFLAGS) -O2 -c $< -o $@

all: server gui tui

compile-debug: $(OBJS)
	$(CC) $(FLAGS) $(ENTRY) $(OBJS) $(DEBUG_FLAG) -o $(PROGRAM_NAME)

server: $(PROGRAM_NAME)

$(PROGRAM_NAME) : $(OBJS)
	$(CC) $(FLAGS) $(ENTRY) $(OBJS) -o $(PROGRAM_NAME)

run-server:
	./$(PROGRAM_NAME)

debug-mode:
	./$(PROGRAM_NAME) --debug

tui: $(TUI_PROGRAM_NAME)

$(TUI_PROGRAM_NAME) : $(OBJS)
	$(CC) $(FLAGS) $(TUI_ENTRY) $(OBJS) -o $(TUI_PROGRAM_NAME)

run-tui:
	./$(TUI_PROGRAM_NAME)

gui: $(GUI_PROGRAM_NAME)

$(GUI_PROGRAM_NAME) : $(OBJS) $(IMGUI_OBJS)
	$(CC) $(GUI_FLAGS) $(GUI_ENTRY) $(OBJS) $(IMGUI_OBJS) $(SDL_LIBS) $(GL_LIBS) -o $(GUI_PROGRAM_NAME)

run-gui:
	./$(GUI_PROGRAM_NAME)

clean:
	rm -rf $(OBJ_FOLDER)

fclean: clean
	rm -f $(PROGRAM_NAME) $(TUI_PROGRAM_NAME) $(GUI_PROGRAM_NAME)
	rm -rf external
	rm -f inc/libs/json.hpp

re: fclean all

.PHONY: help install run-server run-gui run-tui valgrind-run debug-mode clean fclean re gui all compile-debug tui server

.DEFAULT_GOAL= all