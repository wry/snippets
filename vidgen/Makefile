ODIR := obj
CCFLAGS += -O0 -nostdlib -ffreestanding

ifeq ($(shell uname -s),Darwin)
  ifeq ($(shell uname -m),arm64)
    # on M1 if libSystem isn't linked, it will crash on launch
    LDFLAGS += -e _start -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem -lm
  else
    # x86
    LDFLAGS += -e _start -static
  endif
else ifeq ($(shell uname -s),Linux)
  ifeq ($(shell uname -m),aarch64)
    LDFLAGS += -e start -lm
  else
    # x86
    LDFLAGS += -e start -static
  endif
endif

all: $(ODIR)/vidgen

$(ODIR)/vidgen: $(ODIR)/vidgen.o
	$(LD) $< $(LDFLAGS) -o $@

$(ODIR)/vidgen.o: src/main.c
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(ODIR)/*

