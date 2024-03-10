CFLAGS := -Weverything
TARGET := genwav

$(TARGET): main.o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(TARGET) *.o
