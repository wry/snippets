CXX := clang++
CXXFLAGS := -std=c++11 -I/opt/homebrew/opt/llvm/include/
LDFLAGS := -L/opt/homebrew/opt/llvm/lib/ -lclang

SOURCES := $(wildcard main*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
BINS := $(SOURCES:.cpp=)

all: $(BINS)

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(BINS)

