# Makefile sioux
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = exec
LDFLAGS =  -L../libs/Reseau -lserveur

default:
	gcc -c $(SOURCES) 
	gcc $(OBJECTS) -o $(TARGET) $(LDFLAGS)  


clean:
	rm *.o $(TARGET)
