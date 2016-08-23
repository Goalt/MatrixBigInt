CXX=g++
CXXFLAGS= -Wall -g -std=c++11

OBJ_FILES=$(patsubst %.cpp, %.o, $(wildcard *.cpp))


BIN_FILES= res

all: $(BIN_FILES)

res: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(OBJ_FILES) $(BIN_FILES)

