TARGETS := udpmcast udpbcast

CC := gcc
CXX := g++
CFLAGS += 
CXXFLAGS += -std=c++11

ifdef PLATFORM
SYSNAME := $(PLATFORM)
include $(PLATFORM).makefile
else
SYSNAME := $(shell uname -s)
endif

ifeq ($(SYSNAME),Linux)
TARGETS += iptomac revshell
else ifeq ($(SYSNAME),Darwin)
TARGETS += revshell
endif

all:
	@echo "targets : $(TARGETS)"

%: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(TARGETS)
