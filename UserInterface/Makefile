CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
         -pedantic-errors -fstack-protector-all 
PROGS = user_interface public01 public02 public03 public04 public05 \
	public06 public07 public08 public09

.PHONY: all clean

all: 
$(PROGS)
user_interface: user_interface.o document.o
    $(CC) user_interface.o -o user_interface

user_interface.o: document.h user_interface.c
    $(CC) $(CFLAGS) -c user_interface.c
    
    
document.o: document.h document.c
    $(CC) $(CFLAGS) -c document.c
    
clean:
	rm -f *.o $(PROGS) *.tmp