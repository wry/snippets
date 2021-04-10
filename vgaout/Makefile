TARGET := vgaout
OBJ := main.o

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: src/%.c
	$(CC) -o $@ -c $<

clean:
	$(RM) $(TARGET)

.INTERMEDIATE: $(OBJ)
.PHONY: clean
