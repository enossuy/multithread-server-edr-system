# Makefile ablette
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = ablette
LDFLAGS =  -L../libs/IPC -lpartage -L../libs/Reseau -lserveur -lpcap

default:
	gcc -c $(SOURCES) 
	gcc -g $(OBJECTS) -o $(TARGET) $(LDFLAGS)


clean:
	rm -f *.o $(TARGET)
