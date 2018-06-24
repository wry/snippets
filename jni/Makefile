# JNI example with jar
# note: OSX specific, so uses .dylib instead of .so

ODIR := obj
OBJ := Bla.jar
CC := gcc
CXX := g++
LD := ld
CXXFLAGS := -std=c++11 -I./src/jni -I/System/Library/Frameworks/JavaVM.framework/Versions/Current/Headers/
JAVAC := javac
JAVACFLAGS := -cp $(ODIR)
JAVAH := javah
JAVA := java
JAR := jar
LDFLAGS :=
TARGET :=

all: $(ODIR)/$(OBJ)

run: $(ODIR)/$(OBJ)
	$(JAVA) -Djava.library.path=./$(ODIR) -jar $<

$(ODIR)/$(OBJ): $(ODIR)/libBla.dylib $(ODIR)/IBla.class $(ODIR)/Bla.class
	$(JAR) cfe $@ Bla -C $(ODIR) IBla.class -C $(ODIR) Bla.class -C $(ODIR) libBla.dylib

$(ODIR)/libBla.dylib: src/jni/Bla.cpp src/jni/Bla.h
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

$(ODIR)/Bla.class: src/java/Bla.java 
	$(JAVAC) $(JAVACFLAGS) -d $(ODIR) $<

$(ODIR)/IBla.class: src/java/IBla.java
	$(JAVAC) $(JAVACFLAGS) -d $(ODIR) $<

src/jni/Bla.h: $(ODIR)/IBla.class $(ODIR)/Bla.class
	$(JAVAH) -o $@ -cp $(ODIR) Bla

.PHONY: clean
clean:
	$(RM) src/jni/Bla.h $(ODIR)/*

