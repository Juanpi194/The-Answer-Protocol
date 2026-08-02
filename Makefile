PROGRAM_NAME = tap
TUI_PROGRAM_NAME = tap_tui_client
GUI_PROGRAM_NAME = tap_gui_client

ENTRY = main.cpp
TUI_ENTRY = ui/tui/main.cpp
GUI_ENTRY = ui/gui/main.cpp

SRC_FOLDER = src
INC_FOLDER = inc
OBJ_FOLDER = obj

# MODIFIED: rutas explícitas, sin $(wildcard ...) -- a petición expresa,
# nada de flexibilidad automática. Cualquier archivo nuevo que alguien
# añada hay que listarlo aquí a mano para que entre en la compilación.
#
# Cosas que existen en el repo pero NO se listan aquí a propósito (quedan
# fuera del build, sin tocarlas, hasta que estén terminadas):
#   - src/factories/EnchantmentFactory.cpp y src/enchantments/EnchantmentFactory.cpp
#     (dos versiones, ninguna conectada a nada todavía -- a revisar cuál
#     de las dos es la buena cuando se termine esa parte)
#   - src/factories/NpcPacificFactory.cpp (no compila tal cual: le falta
#     el include de Enchanter.hpp, y tiene un typo NpcFactory/NpcPacificFactory)
#   - src/protocol/command.cpp y src/protocol/commandparser.cpp (fork viejo,
#     ya superado por src/commands/, pedido explícitamente no tocarlos)
#   - src/parser/enemyparser.cpp (vacío, sin empezar)
#   - inc/characters/Vendor.hpp, inc/items/SpecialEffectGear.hpp (solo
#     cabecera, sin .cpp -- nada que compilar todavía)

BATTLE_SRC = $(addprefix $(SRC_FOLDER)/battle/, \
				Battle.cpp)

CHARACTERS_SRC = $(addprefix $(SRC_FOLDER)/characters/, \
					Character.cpp Enchanter.cpp Fighter.cpp Merchant.cpp \
					NPC.cpp Narrator.cpp Player.cpp QuestGiver.cpp \
					enemies/Enemy.cpp enemies/Goblin.cpp)

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
				shields/SteelShield.cpp \
				weapons/BronzeSword.cpp weapons/FlameSword.cpp \
				weapons/FrostSword.cpp weapons/IronSword.cpp \
				weapons/SerratedSword.cpp weapons/SteelSword.cpp)

FACTORIES_SRC = $(addprefix $(SRC_FOLDER)/factories/, \
					EnemyFactory.cpp ItemFactory.cpp)

COMMANDS_SRC = $(addprefix $(SRC_FOLDER)/commands/, \
					command.cpp CommandHandler.cpp commandparser.cpp)

# MODIFIED: solo events.cpp/responses.cpp -- command.cpp/commandparser.cpp
# de esta carpeta son el fork viejo, sin tocar, no entran en el build.
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

# DE AQUI PARA ABAJO ME HE FLIPADO INTENTANDO HACER INTERCONEXIONES.
# FUNCIONA, PERO RECOMPILA EN TODO MOMENTO. LO ARREGLARE.

# MODIFIED: make help ahora sí explica cada comando.
help:
	@echo "Available targets:"
	@echo "  make / make all   - Compile all 3 executables: server, gui, tui"
	@echo "  make server       - Compile only the server binary ($(PROGRAM_NAME))"
	@echo "  make gui          - Compile only the GUI client ($(GUI_PROGRAM_NAME))"
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

# MODIFIED: ademas de json.hpp, ahora tambien trae ImGui si no esta ya.
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

# MODIFIED: regla de compilacion para los .cpp de ImGui (A revisar)
$(OBJ_FOLDER)/$(IMGUI_DIR)/%.o: $(IMGUI_DIR)/%.cpp | install
	mkdir -p $(dir $@)
	$(CC) $(VERSION_FLAG) $(GUI_INC) $(SDL_CFLAGS) -O2 -c $< -o $@

# MODIFIED: all ahora compila los 3 ejecutables (antes solo el server).
all: server gui tui

compile-debug: $(OBJS)
	$(CC) $(FLAGS) $(ENTRY) $(OBJS) $(DEBUG_FLAG) -o $(PROGRAM_NAME)

# MODIFIED: "server" es ahora el nombre explícito de este target (antes era
# "all"). Sigue siendo el mismo binario de siempre: "tap" a secas es el
# server (normal_mode), "tap --debug" es debug_mode -- ver debug-mode abajo.
server: $(OBJS)
	$(CC) $(FLAGS) $(ENTRY) $(OBJS) -o $(PROGRAM_NAME)

run-server: server
	./$(PROGRAM_NAME)

debug-mode: server
	./$(PROGRAM_NAME) --debug

# MODIFIED: TUI (cliente CLI que pide el subject aparte del server y la
# GUI). Comparte $(OBJS) con todo lo demas -- no necesita SDL2 ni ImGui
# para nada, CLI no depende de esas librerias.
tui: $(OBJS)
	$(CC) $(FLAGS) $(TUI_ENTRY) $(OBJS) -o $(TUI_PROGRAM_NAME)

run-tui: tui
	./$(TUI_PROGRAM_NAME)

# MODIFIED: build de la GUI, unificado aqui (antes vivia en gui/Makefile).
# MODIFIED: "gui" ya no depende directamente de $(IMGUI_OBJS) -- eso obliga
# a Make a resolver esa regla ANTES de ejecutar "install", y en un repo
# nuevo external/imgui/ todavia no existe en ese momento (Make calcula todo
# el grafo de dependencias antes de ejecutar nada), asi que fallaba con
# "No rule to make target obj/external/imgui/imgui.o" aunque install
# fuese a crear la carpeta un segundo despues. La solucion: "install" se
# ejecuta primero de verdad (prerequisito normal, no de orden), y luego se
# lanza una sub-invocacion de make que vuelve a leer el Makefile con
# external/imgui/ ya en el sitio.
gui: install
	$(MAKE) gui-build

gui-build: $(OBJS) $(IMGUI_OBJS)
	$(CC) $(GUI_FLAGS) $(GUI_ENTRY) $(OBJS) $(IMGUI_OBJS) $(SDL_LIBS) $(GL_LIBS) -o $(GUI_PROGRAM_NAME)

run-gui: gui
	./$(GUI_PROGRAM_NAME)

# MODIFIED: clean ya no toca external/imgui -- eso son "importaciones" de
# install, no artefactos de compilación. Se queda solo con obj/, así un
# "make clean" normal no te obliga a re-descargar ImGui cada vez.
clean:
	rm -rf $(OBJ_FOLDER)

# MODIFIED: fclean ahora también deshace lo que trae "install" (json.hpp,
# external/imgui), tal y como pediste.
fclean: clean
	rm -f $(PROGRAM_NAME) $(TUI_PROGRAM_NAME) $(GUI_PROGRAM_NAME)
	rm -rf external/imgui
	rm -f inc/libs/json.hpp

re: fclean all

.PHONY: help install run-server run-gui run-tui valgrind-run debug-mode clean fclean re gui gui-build all compile-debug tui server

.DEFAULT_GOAL= all