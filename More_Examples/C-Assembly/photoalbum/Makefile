CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
-pedantic-errors -fstack-protector-all 
PROGS = public01 public02 public03 public04

.PHONY: all clean

all: $(PROGS)

clean:
	rm -f *.o $(PROGS) a.out

$(PROGS): photo_album.o my_memory_checker_216.o 
public%: public%.o

photo_album.o: photo_album.h
my_memory_checker_216.o: my_memory_checker_216.h
public%.o: photo_album.h my_memory_checker_216.h 

sponsor:
	@echo "This exercise brought to you by pepsi"

