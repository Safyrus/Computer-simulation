CXX        := g++
CXX_FLAGS  := -Wall -Wextra -std=c++17 -pthread -O1

ifeq ($(OS), Windows_NT)
    CXX_FLAGS  += -D _WIN32_WINNT=0x0A00
    LIB        := lib/win
    BIN        := bin/Windows
else
    CXX_FLAGS  += -fpermissive
    LIB        := lib/linux
    BIN        := bin/Linux
endif

SRC        := src
INCLUDE    := include

SRCFILE    = $(call rwildcard,$(SRC),*.cpp)

LIBRARIES  := -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
EXECUTABLE := main

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))


all: $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(addprefix $(BIN)/obj/,$(SRCFILE:.cpp=.o))
	$(CXX) $(CXX_FLAGS) -L$(LIB) $^ -o $@ $(LIBRARIES)


$(BIN)/obj/%.o: %.cpp
ifeq ($(OS), Windows_NT)
	@-if not exist "$(@D)" ( mkdir "$(@D)" )
else
	@-if ! [ -d "$(@D)" ]; then mkdir -p "$(@D)"; fi
endif
	@$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $^ -o $@


ifeq ($(OS), Windows_NT)
BIN := $(subst /,\,$(BIN))
install:
	-copy ..\data\pix46.ttf $(BIN)
	-if exist "$(BIN)\prog\" ( Rmdir /Q /S "$(BIN)\prog" )
	-mkdir $(BIN)\prog
	-Xcopy /E ..\data\prog $(BIN)\prog
	-if exist "$(BIN)\data\img\" ( Rmdir /Q /S "$(BIN)\data\img" )
	-mkdir $(BIN)\data\img
	-Xcopy /E ..\data\img $(BIN)\data\img
	-if exist "$(BIN)\data\snd\" ( Rmdir /Q /S "$(BIN)\data\snd" )
	-mkdir $(BIN)\data\snd
	-Xcopy /E ..\data\snd $(BIN)\data\snd

clean:
	-del $(BIN)\$(EXECUTABLE).exe
	-Rmdir /Q /S "$(BIN)\obj"

else
install:
	-cp ../data/pix46.ttf $(BIN)
	if [ -d "$(BIN)/prog/" ]; then rm -r $(BIN)/prog/; fi
	-cp -r ../data/prog $(BIN)
	if [ -d "$(BIN)/data/img/" ]; then rm -r $(BIN)/data/img/; fi
	-cp -r ../data/img $(BIN)/data
	if [ -d "$(BIN)/data/snd/" ]; then rm -r $(BIN)/data/snd/; fi
	-cp -r ../data/snd $(BIN)/data

clean:
	-rm $(BIN)/$(EXECUTABLE)
	-rm -r $(BIN)/obj
endif
