COMPILER = g++
RMDIR = rm -rdf
RM = rm -f

DEP_FLAGS = -M -MT $@ -MT $(OBJ_PATH)/$(*F).o -MP -MF $@
LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm

INC_PATHS = -I$(INC_PATH) $(addprefix -I,$(SDL_INC_PATH))

FLAGS = -std=c++11 -Wall -pedantic -Wextra -Wno-unused-parameter -Werror=init-self -fsanitize=address -g

DFLAGS = -ggdb -O0 -DDEBUG

RFLAGS = -O3 -mtune=native

INC_PATH = include
SRC_PATH = src
BIN_PATH = bin
OBJ_PATH = obj
DEP_PATH = dep

CPP_FILES = $(shell find $(SRC_PATH) -name "*.cpp")
INC_FILES = $(shell find $(SRC_PATH) -name "*.hpp")
FILE_NAMES = $(sort $(notdir $(CPP_FILES:.cpp=)) $(notdir $(INC_FILES:.h=)))
DEP_FILES = $(patsubst $(SRC_PATH)/%.cpp,$(DEP_PATH)/%.d,$(CPP_FILES))
OBJ_FILES = $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(CPP_FILES))

EXEC = JOGO

# SE FOR WINDOWS
ifeq ($(OS),Windows_NT)
RMDIR = rd /s /q
RM = del /q

SDL_PATHS = C:/SDL2

SDL_INC_PATH += $(addsuffix /include,$(SDL_PATHS))
LINK_PATH = $(addprefix -L,$(addsuffix /lib,$(SDL_PATHS)))
FLAGS += -mwindows
DFLAGS += -mconsole
LIBS := -lmingw32 -lSDL2main $(LIBS)

EXEC := $(EXEC).exe

else

SDL_INC_PATH = /usr/include/SDL2
INC_PATHS = -I$(INC_PATH) $(addprefix -I,$(SDL_INC_PATH))
UNAME_S := $(shell uname -s)

# SE FOR MAC
ifeq ($(UNAME_S), Darwin)

LIBS = -lm -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf

endif
endif

.PRECIOUS: $(DEP_FILES)
.PHONY: release debug clean folders help

all: folders $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(COMPILER) -o $@ $^ $(LINK_PATH) $(LIBS) $(FLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp | folders
	@mkdir -p $(dir $@)
	$(COMPILER) $(INC_PATHS) $< -c $(FLAGS) -o $@


$(DEP_PATH)/%.d: $(SRC_PATH)/%.cpp | folders
	@mkdir -p $(dir $@)
	$(COMPILER) $(INC_PATHS) $< $(DEP_FLAGS) $(FLAGS)

clean:
	$(RMDIR) $(DEP_PATH)
	$(RMDIR) $(OBJ_PATH)
	$(RMDIR) $(BIN_PATH)
	$(RM) $(EXEC)

release: FLAGS += $(RFLAGS)
release: $(EXEC)

debug: FLAGS += $(DFLAGS)
debug: $(EXEC)

folders:
ifeq ($(OS), Windows_NT)
	@if NOT exist $(DEP_PATH) (mkdir $(DEP_PATH))
	@if NOT exist $(OBJ_PATH) (mkdir $(OBJ_PATH))
	@if NOT exist $(BIN_PATH) (mkdir $(BIN_PATH))
	@if NOT exist $(INC_PATH) (mkdir $(INC_PATH))
	@if NOT exist $(SRC_PATH) (mkdir $(SRC_PATH))
else
	@mkdir -p $(DEP_PATH) $(OBJ_PATH) $(BIN_PATH) $(INC_PATH) $(SRC_PATH)
endif

print-% : ; echo $* = $($*)

help:
ifeq ($(OS), Windows_NT)
	echo.
endif
	@echo Available targets:
	@echo - release: Builds the release version
	@echo - debug: Builds the debug version
	@echo - clean: Cleans generated files
	@echo - folders: Generates project directories
	@echo - help: Show help
ifeq ($(OS), Windows_NT)
	echo.
endif

.SECONDEXPANSION:
-include $$(DEP_FILES)