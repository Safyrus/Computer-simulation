CXX        := g++
CXX_FLAGS  := -Wall -Wextra -std=c++17 -ggdb -pthread

ifeq ($(OS), Windows_NT)
    CXX_FLAGS  += -D _WIN32_WINNT=0x0A00
    LIB        := lib/win
    BIN        := bin/Windows
else
    CXX_FLAGS  += -z execstack -fpermissive
    LIB        := lib/linux
    BIN        := bin/Linux
endif

SRC        := src
INCLUDE    := include

LIBRARIES  := -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
EXECUTABLE := main

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))


all: $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(call rwildcard,$(SRC),*.cpp)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)


ifeq ($(OS), Windows_NT)
BIN := $(subst /,\,$(BIN))
install:
	-copy ..\data\pix46.ttf $(BIN)
	-if exist "$(BIN)\prog\" ( Rmdir /Q /S $(BIN)\prog )
	-mkdir $(BIN)\prog
	-Xcopy /E ..\data\prog $(BIN)\prog\

clean:
	-del $(BIN)\$(EXECUTABLE).exe
else
install:
	-cp ../data/pix46.ttf $(BIN)
	-cp -r ../data/prog $(BIN)

clean:
	-rm $(BIN)/$(EXECUTABLE)
endif
