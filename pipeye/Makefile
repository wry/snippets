TARGET := pipeye
CXX := g++
CXXFLAGS := -std=c++11
STRIP := strip

ifdef PLATFORM
include $(PLATFORM).makefile
endif

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<  $(LDFLAGS)
	$(STRIP) $@

.PHONY: clean
clean:
	$(RM) $(TARGET)
