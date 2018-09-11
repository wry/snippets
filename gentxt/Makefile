TARGET := gentxt
CPPFLAGS := -std=c++11

# font.i is the uint8_t list generated from ascii chars 0x20 to 0x7e ; each char represented as 8 uint8_t-s (8x8 bits per char)
# png : convert -size 300x300 -depth 32 -define quantum:format=floating-point -endian lsb rgba:bla.rgba bla.png

all: font.i $(TARGET)

$(TARGET): main.o
	$(CC) -o $@ $<

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(TARGET) *.o *.i
