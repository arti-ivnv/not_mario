# to complile and run in one command type:
# make run

# determine compiler
CXX := g++ 

# output file name
OUTPUT := Not_Mario

# determine operating system 
# uname is a terminal comand to see which os we got
OS := $(shell uname)

# determine source folder
SRC_DIR := ./src
OBJ_DIR := ./obj
INC_DIR := ./include
BIN_DIR := ./bin

# Mac-specific deployment Paths
APP_BUNDLE := $(OUTPUT).app
CONTENTS := $(BIN_DIR)/$(APP_BUNDLE)/Contents
MACOS := $(CONTENTS)/MacOS
FRAMEWORKS := $(CONTENTS)/Frameworks
RESOURCES := $(CONTENTS)/Resources

CONFIG_FILES := .bin/data/assets.txt

# Mac comiler / linker flags
ifeq ($(OS), Darwin)
# 	SFML_DIR := /opt/homebrew/Cellar/sfml@2/2.6.2_1
	SFML_DIR := bin/graphics/2.6.2_1
	CXX_FLAGS := -O3 -std=c++17 -Wno-unused-result -Wno-deprecated-declarations
	INCLUDES := -I$(SRC_DIR) -I$(SFML_DIR)/include
# 	Linker flags
	LDFLAGS := -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -L$(SFML_DIR)/lib -framework OpenGL -framework CoreFoundation
endif


SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
DEP_FILES := $(OBJ_FILES:.o=.d)

# include dependency files
-include $(DEP_FILES)

# all of these targets will be made if you just type make
all: $(OUTPUT)

# define the main executable requirments / command
# Link the main executable
$(OUTPUT) : $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

# specifies how the object files are compiled from cpp files
# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -MMD -MP -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

deploy: $(OUTPUT)
		@echo "Creating App Bundle: $(APP_NAME)..."
		mkdir -p $(MACOS) $(RESOURCES) $(FRAMEWORKS)

# 		Step 1 - copy binary
		cp ./bin/$(OUTPUT) $(MACOS)/

# 		Step 2 - Copy SFML libraries
		cp $(SFML_DIR)/lib/*.dylib $(FRAMEWORKS)/

# 		Step 3 - Copy assetes and configs
		@echo "Copying game data and assets..."
			if [ -d "bin/data" ]; then cp -r bin/data $(RESOURCES)/; fi
			if [ -d "bin/assets" ]; then cp -r bin/assets $(RESOURCES)/; fi

# 		Step 4 - Generate Info.plist
		@echo '<?xml version="1.0" encoding="UTF-8"?>' > $(CONTENTS)/Info.plist
		@echo '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com">' >> $(CONTENTS)/Info.plist
		@echo '<plist version="1.0"><dict>' >> $(CONTENTS)/Info.plist
		@echo '    <key>CFBundleExecutable</key><string>$(OUTPUT)</string>' >> $(CONTENTS)/Info.plist
		@echo '    <key>CFBundleIdentifier</key><string>com.yourname.$(OUTPUT)</string>' >> $(CONTENTS)/Info.plist
		@echo '    <key>CFBundlePackageType</key><string>APPL</string>' >> $(CONTENTS)/Info.plist
		@echo '    <key>CFBundleSignature</key><string>????</string>' >> $(CONTENTS)/Info.plist
		@echo '</dict></plist>' >> $(CONTENTS)/Info.plist

# 		Step 5 - Fix Library RPaths for Portability
		@for lib in $(shell ls $(FRAMEWORKS) | grep dylib); do \
			install_name_tool -change @rpath/$$lib @executable_path/../Frameworks/$$lib $(MACOS)/$(OUTPUT); \
		done
		@echo "Deployment build complete at $(BIN_DIR)/$(APP_NAME)"


# typing 'make clean' will remove all intermidiate build files
clean:
	rm -rf $(OBJ_FILES) $(DEP_FILES) ./bin/$(OUTPUT) ./bin/$(OUTPUT).app

# typing 'make run' will compile and run the program
run: $(OUTPUT)
	cd bin && ./$(OUTPUT) $(CONFIG_FILE) && cd ..




