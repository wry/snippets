TARGET := curl_test
CXXFLAGS += 
LDFLAGS += -lcurl

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) $(TARGET)
