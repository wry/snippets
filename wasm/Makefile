CC := emcc
CFLAGS := -s WASM=1 -Os
TARGET := main

$(TARGET).wasm: $(TARGET).js index.html
$(TARGET).js: $(TARGET).c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(TARGET).js $(TARGET).wasm $(TARGET).js.mem
