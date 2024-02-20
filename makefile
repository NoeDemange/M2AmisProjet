# project name (generate executable with this name)
TARGET   = graphCycleMol.exe

CFLAGS=-Wall -g
LDFLAGS=-lm
LDNAUTY = nauty/nauty.a

INCDIR=-Iinclude
INCNAUTY=-Inauty
OBJDIR=obj
SRCDIR=src
BINDIR=bin

CC=gcc -fopenmp -O3
EXEC=clean dir $(BINDIR)/$(TARGET)
SRC:=$(wildcard $(SRCDIR)/*.c)
OBJ:=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(EXEC)

$(BINDIR)/$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDNAUTY)

$(OBJ) : $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(INCDIR) $(INCNAUTY) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper dir run data

run:
	./$(BINDIR)/$(TARGET)

data:
	python3 chebi_molecule_loader.py

dir:
	mkdir $(OBJDIR)
	mkdir $(BINDIR)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	ls

mrproper: clean
	rm -rf data

LADIR=M2AmisProjet_GroupeB
zip:
	rm -rf ${LADIR}
	mkdir ${LADIR}
	cp -r $(SRCDIR) $(LADIR)
	cp -r include $(LADIR)
	cp makefile ${LADIR}
#	cp Compte_Rendu.pdf ${LADIR}
	rm -f ${LADIR}.zip
	zip -r ${LADIR}.zip ${LADIR}
	rm -rf ${LADIR}
	ls -l

test: mrproper all data run