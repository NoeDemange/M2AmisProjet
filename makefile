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
DOT_FILES := $(wildcard graphs/*.dot)
PNG_FILES := $(patsubst %.dot,%.png,$(DOT_FILES))
SVG_FILES := $(patsubst %.dot,%.svg,$(DOT_FILES))

all: $(EXEC)
	ln -sf ./$(BINDIR)/$(TARGET) ./similarite

$(BINDIR)/$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDNAUTY)

$(OBJ) : $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(INCDIR) $(INCNAUTY) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper dir run data

run:
	./$(BINDIR)/$(TARGET)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BINDIR)/$(TARGET)

data:
	rm -rf data
	python3 chebi_molecule_loader.py

dir:
	mkdir $(OBJDIR)
	mkdir $(BINDIR)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -rf similarite
	rm -rf graphs
	ls

mrproper: clean
	rm -rf data
	rm matRes.csv

png: $(PNG_FILES) #need graphviz
$(PNG_FILES): graphs/%.png: graphs/%.dot 
#	neato -Gstart=5 -Tpng $< -o $@ 
	dot -Tpng $< -o $@

svg : $(SVG_FILES)
$(SVG_FILES): graphs/%.svg: graphs/%.dot 
#	neato -Gstart=5 -Tsvg $< -o $@ 
	dot -Tsvg $< -o $@

cleang :
	rm -f graphs/*.png
	rm -f graphs/*.svg

LADIR=M2AmisProjet_GroupeB
zip:
	rm -rf ${LADIR}
	mkdir ${LADIR}
	cp -r $(SRCDIR) $(LADIR)
	cp -r include $(LADIR)
	cp makefile ${LADIR}
	mkdir ${LADIR}/nauty
	cp nauty/A_lire.txt ${LADIR}/nauty/A_lire.txt
	cp README.md $(LADIR)
	cp chebi_molecule_loader.py ${LADIR}
	cp M2Projet_Rapport_GroupeB.pdf ${LADIR}
	rm -f ${LADIR}.zip
	zip -r ${LADIR}.zip ${LADIR}
	rm -rf ${LADIR}
	ls -l

test: mrproper all data run