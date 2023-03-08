OBJECTS := $(patsubst %.c,%.o,$(wildcard src/*.c))

main: $(OBJECTS)	
	gcc $(OBJECTS) -lcurses -o main