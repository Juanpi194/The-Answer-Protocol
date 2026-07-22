NAME = tap_gui_client

SRC = src/main.cpp \
      src/GameClient.cpp \
      external/imgui/imgui.cpp \
      external/imgui/imgui_draw.cpp \
      external/imgui/imgui_tables.cpp \
      external/imgui/imgui_widgets.cpp \
      external/imgui/backends/imgui_impl_sdl2.cpp \
      external/imgui/backends/imgui_impl_opengl3.cpp

OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.cpp=.o)))
VPATH = src external/imgui external/imgui/backends

INC = -I inc -I external/imgui -I external/imgui/backends

SDL_FLAGS = $(shell pkg-config --cflags --libs sdl2 2>/dev/null || sdl2-config --cflags --libs)
GL_FLAGS = -lGL

CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 $(INC)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) $(SDL_FLAGS) $(GL_FLAGS)

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all run clean fclean re