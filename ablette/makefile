# Makefile ablette
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = ablette
LDFLAGS = -lpcap -L../libs/Reseau -lserveur 

default:
	gcc -c $(SOURCES) 
	gcc $(OBJECTS) -o $(TARGET) $(LDFLAGS)  


clean:
	rm -f *.o $(TARGET)
