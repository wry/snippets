TARGETS := getaverror
CXX := g++
CXXFLAGS := -std=c++11 -D__STDC_CONSTANT_MACROS
LDFLAGS:= -lavutil
STRIP := strip

all:
	@echo targets : $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<  $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) $(TARGETS)
