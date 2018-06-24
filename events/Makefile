TARGET := evsend
CXX := g++
CXXFLAGS := -std=c++11

ifdef PLATFORM
include $(PLATFORM).makefile
endif

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(STATIC_LIBS) $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) $(TARGET)
