ODIR := obj
CCFLAGS += -Os -nostdlib -ffreestanding
LDFLAGS += -e _start -static

all: $(ODIR)/vidgen

$(ODIR)/vidgen: $(ODIR)/vidgen.o
	$(LD) $< $(LDFLAGS) -o $@

$(ODIR)/vidgen.o: src/main.c
	$(CC) $(CCFLAGS) -c $< -o $@


.PHONY: clean
clean:
	$(RM) $(ODIR)/*

