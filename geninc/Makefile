TARGETS := geninc test bytes.i
CC := gcc
CCFLAGS := 
CXX := g++
CXXFLAGS := -std=c++11
LDFLAGS:= 
STRIP := strip

all: $(TARGETS)

test: test.c bytes.i
	$(CC) $(CCFLAGS) -o $@ $<  $(LDFLAGS)

geninc: geninc.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<  $(LDFLAGS)

bytes.i: geninc
	./geninc README.md > bytes.i

.PHONY: clean
clean:
	$(RM) $(TARGETS)
