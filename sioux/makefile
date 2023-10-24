# Makefile sioux
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = serveur_madz
LDFLAGS =  -L../libs/Reseau -lserveur -pthread

default:
	gcc -c $(SOURCES) 
	gcc $(OBJECTS) -o $(TARGET) $(LDFLAGS)  


clean:
	rm -f *.o $(TARGET)
