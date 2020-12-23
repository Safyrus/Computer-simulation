CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -pthread

ifeq ($(OS), Windows_NT)
	CXX_FLAGS += -D _WIN32_WINNT=0x0A00
else
	CXX_FLAGS += -z execstack -fpermissive -m32
endif

BIN		:= bin
SRC		:= src
INCLUDE	:= include
ifeq ($(OS), Windows_NT)
LIB		:= lib/win
else
LIB		:= lib/linux
endif

LIBRARIES	:= -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
EXECUTABLE	:= main

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(call rwildcard,$(SRC),*.cpp)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/$(EXECUTABLE)*