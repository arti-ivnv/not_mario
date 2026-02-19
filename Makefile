# to complile and run in one command type:
# make run

# determine compiler
CXX := g++ 

# output file name
OUTPUT := sfmlgame

# determine operating system 
# uname is a terminal comand to see which os we got
OS := $(shell uname)

# determine source folder
SRC_DIR := ./src

OBJ_DIR := ./obj

# determine include folder for 3d party libs
INC_DIR := ./include

BIN_DIR := ./bin

CONFIG_FILES := .bin/data/assets.txt

# Mac comiler / linker flags
ifeq ($(OS), Darwin)
	SFML_DIR := /opt/homebrew/Cellar/sfml@2/2.6.2_1
	CXX_FLAGS := -O3 -std=c++17 -Wno-unused-result -Wno-deprecated-declarations
	INCLUDES := -I$(SRC_DIR) -I$(SFML_DIR)/include
# 	Linker flags
	LDFLAGS := -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -L$(SFML_DIR)/lib -framework OpenGL 
endif

# the source files for the the ecs game engine
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
# OBJ_FILES := $(SRC_FILES:.cpp=.o)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

TARGET := $(BIN_DIR)/myprogram


# include dependency files
DEP_FILES := $(OBJ_FILES:.o=.d)
-include $(DEP_FILES)

# all of these targets will be made if you just type make
all: $(OUTPUT)

# define the main executable requirments / command
$(OUTPUT) : $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

# specifies how the object files are compiled from cpp files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -MMD -MP -c $(CXX_FLAGS) $(INCLUDES) $< -o $@


# typing 'make clean' will remove all intermidiate build files
clean:
	rm -f $(OBJ_FILES) $(DEP_FILES) ./bin/$(OUTPUT)

# typing 'make run' will compile and run the program
run: $(OUTPUT)
	cd bin && ./$(OUTPUT) $(CONFIG_FILE) && cd ..




