TARGET := ooc
OBJ := main.o Color.o Log.o

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: src/%.c
	$(CC) -o $@ -c $<

clean:
	$(RM) $(TARGET)

.INTERMEDIATE: $(OBJ)
.PHONY: clean
