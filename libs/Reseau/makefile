# Makefile libs

SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
CIBLE = libserveur.a

#
# Nom de la cible principale
#

all: $(CIBLE)

#
# Cible de nettoyage
#

clean: 
	rm -f *.o $(CIBLE)

#
# Dependances pour la bibliotheque
#

$(CIBLE): $(OBJETS)
	$(AR) rs $@ $?

$(CIBLE:.a=).o: $(CIBLE:.a=).c $(CIBLE:.a=).h
