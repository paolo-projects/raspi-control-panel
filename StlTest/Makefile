CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g -MMD
LDFLAGS = -pthread
LBLIBS = -lTouchCPLib -lwiringPi
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