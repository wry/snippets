TARGET := lwstest

CFLAGS := -I /tmp/libwebsockets/install/include/
LDFLAGS := -L /tmp/libwebsockets/install/lib/ -lwebsockets

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	$(RM) $(TARGET)

