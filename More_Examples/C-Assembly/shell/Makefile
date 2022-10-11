# provide a makefile with a target called shell_jr that 
# creates the executable shell_jr

CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors \
-fstack-protector-all

all: shell_jr

shell_jr: shell_jr.o
	$(CC) shell_jr.o -o shell_jr

shell_jr.o: shell_jr.c
	$(CC) $(CFLAGS) -c shell_jr.c

clean:
	@echo "Cleaning my system"
	@rm -f *.io $(PROGS) a.out
