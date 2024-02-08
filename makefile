# project name (generate executable with this name)
TARGET   = graphCycleMol.exe

CFLAGS=-Wall -g
LDFLAGS=-lm

INCDIR=-Iinclude
OBJDIR=obj
SRCDIR=src
BINDIR=bin

CC=gcc -fopenmp -O3
EXEC=clean dir $(BINDIR)/$(TARGET)
SRC:=$(wildcard $(SRCDIR)/*.c)
OBJ:=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(EXEC)

$(BINDIR)/$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ) : $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(INCDIR) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper dir run

run:
	./$(BINDIR)/$(TARGET)

dir:
	mkdir $(OBJDIR)
	mkdir $(BINDIR)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	ls

mrproper: clean
	rm -rf results