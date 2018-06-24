# to compile:
#   g++ -std=c++11 -I /usr/local/include -L /usr/local/lib -lsvg bla.cpp
# to compile and test:
#   g++ -std=c++11 -I /usr/local/include -L /usr/local/lib -lsvg bla.cpp && ./a.out /tmp/drawing-1.svg loan /Users/wry/wrk/liboverlay/tvloverlay/qt/liboverlay/assets/storyboard/mmarkt_media_set1.xml > /tmp/IPTV-2401/storyboard.xml 

TARGET := svg_test
CXXFLAGS += -std=c++11
LDFLAGS += -lsvg

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) $(TARGET)
