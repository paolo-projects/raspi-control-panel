CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -g -fsanitize=address -MMD
LDFLAGS =  -fsanitize=address
LBLIBS = -lSDL2 -lts -lSDL2_ttf -lSDL2_image
INCLUDES = -Iinclude

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)
EXEC = raspicp

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

-include $(DEP)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -rf $(OBJ) $(DEP) $(EXEC)