CC=gcc

MAIN=main

SRC = $(wildcard src/*.c)
OBJ = $(addprefix obj/,$(notdir $(SRC:.c=.o)))

CFLAGS = -I ./include -Wall
LFLAGS = 

all: $(MAIN)

$(MAIN): $(OBJ) | bin
	$(CC) $(OBJ) -o $(addprefix bin/, $(MAIN))

obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) $(LFLAGS) -o $@
	
obj:
	mkdir obj

bin:
	mkdir bin

clean:
	rm $(addprefix bin/, $(MAIN))
	rm $(OBJ)
