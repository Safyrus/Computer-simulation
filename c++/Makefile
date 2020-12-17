CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -pthread -fpermissive

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

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
	-rm $(BIN)/*