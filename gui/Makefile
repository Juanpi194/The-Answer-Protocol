# Makefile for the TAP GUI client.
# Meant to run on the school cluster, where SDL2 is already installed.
# ImGui is vendored under external/imgui; if it is missing, "deps" clones it.
#
# This links the REAL game engine (characters, items, world, quests,
# commands, battle, enchantments, utils, and PlayerConnection) directly --
# no duplicated types, no mock server. Deliberately excluded:
#   - src/parser/*.cpp   -- currently out of sync with Room/Quest/QuestGiver/
#                            Consumable's real constructors (does not compile
#                            as-is); not needed since World's default
#                            constructor does not go through them yet either.
#   - Server.cpp / ServerOwner.cpp -- full networked server, not needed here;
#                                      the GUI's Remote mode is its own small
#                                      TCP client in GuiClient.cpp instead.

NAME = tap_gui_client

IMGUI_DIR = external/imgui
IMGUI_TAG = v1.90.9

ROOT = ..

# All real game-logic sources the GUI needs, pulled in automatically so new
# files teammates add under these folders get picked up without editing this
# Makefile. See the exclusions explained above.
GAME_SRC_DIRS = characters items quests world commands enchantments battle utils
GAME_SRC = $(foreach d,$(GAME_SRC_DIRS),$(wildcard $(ROOT)/src/$(d)/*.cpp)) \
           $(ROOT)/src/server/PlayerConnection.cpp \
           $(ROOT)/src/gui/GuiClient.cpp

SRC = src/main.cpp \
      $(GAME_SRC) \
      $(IMGUI_DIR)/imgui.cpp \
      $(IMGUI_DIR)/imgui_draw.cpp \
      $(IMGUI_DIR)/imgui_tables.cpp \
      $(IMGUI_DIR)/imgui_widgets.cpp \
      $(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp \
      $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

OBJDIR = obj
# Object files mirror the full relative path of their source (obj/src/main.o,
# obj/../src/characters/Player.o, ...) -- no VPATH guessing, no ambiguity.
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))

INC = -I $(ROOT)/inc -I $(IMGUI_DIR) -I $(IMGUI_DIR)/backends

SDL_CFLAGS = $(shell pkg-config --cflags sdl2 2>/dev/null || sdl2-config --cflags)
SDL_LIBS = $(shell pkg-config --libs sdl2 2>/dev/null || sdl2-config --libs)
GL_FLAGS = -lGL
THREAD_FLAGS = -pthread

CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 $(INC) $(SDL_CFLAGS) $(THREAD_FLAGS)

all: $(NAME)

deps:
	@if [ ! -d "$(IMGUI_DIR)" ]; then \
		echo "Dear ImGui not found, cloning $(IMGUI_TAG)..."; \
		git clone --depth 1 --branch $(IMGUI_TAG) https://github.com/ocornut/imgui.git $(IMGUI_DIR); \
	fi

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) $(SDL_LIBS) $(GL_FLAGS) $(THREAD_FLAGS)

# % matches the full relative path (e.g. "src/main" or
# "../src/characters/Player"), so no VPATH search is needed.
$(OBJDIR)/%.o: %.cpp | deps
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all deps run clean fclean re
