TARGET := gentxt

all: font.i $(TARGET)

$(TARGET): main.o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(TARGET) *.o *.i
