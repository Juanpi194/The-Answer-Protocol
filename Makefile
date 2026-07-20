PROGRAM_NAME = tap

ENTRY = main.cpp

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
						Enchantment.cpp EnchantmentFactory.cpp Flame.cpp)

ITEMS_FOLDER = items
ITEMS_SRC = $(addprefix $(SRC_FOLDER)/$(ITEMS_FOLDER)/, \
				Armor.cpp Chest.cpp ChestKey.cpp Consumable.cpp Gear.cpp \
				IronArmor.cpp IronShield.cpp IronSword.cpp \
				ItemFactory.cpp Item.cpp Shield.cpp Weapon.cpp)

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

SRC = $(BATTLE_SRC) $(CHARACTERS_SRC) $(ENCHANTMENTS_SRC) $(ITEMS_SRC) $(QUESTS_SRC) $(SERVER_SRC) $(UTILS_SRC) $(WORLD_SRC)

OBJS = $(SRC:$(SRC_FOLDER)/%.cpp=$(OBJ_FOLDER)/%.o)

FLAGS = $(VERSION_FLAG) -I $(INC_FOLDER)
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


# DE AQUÍ PARA ABAJO ESTÁ COMO LA MIERDA

help:
	# TODO: Explanation of each command

install:
	curl -o inc/libs/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp

run:
	./$(PROGRAM_NAME)

$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(DEBUG_FLAG) -c $< -o $@

all: $(OBJS)
	$(CC) $(FLAGS) $(ENTRY) $(OBJS) -o $(PROGRAM_NAME)

compile-debug: $(OBJS)
	$(CC) $(FLAGS) $(ENTRY) $(OBJS) $(DEBUG_FLAG) -o $(PROGRAM_NAME)

clean:
	rm $(OBJS)

fclean: clean
	rm $(PROGRAM_NAME)

re: clean all


.DEFAULT_GOAL= all
