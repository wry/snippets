TARGET := gentxt

# font.i is the uint8_t list generated from ascii chars 0x20 to 0x7e ; each char represented as 8 uint8_t-s (8x8 bits per char)
# float color components, conv to png : convert -size 300x300 -depth 32 -define quantum:format=floating-point -endian lsb rgba:out.rgba out.png
# uint8_t color components, show : ./gentxt | ffplay -f rawvideo -pixel_format rgba -video_size 300x300 -

all: font.i $(TARGET)

$(TARGET): main.o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(TARGET) *.o *.i
