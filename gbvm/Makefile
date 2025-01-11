CC = gcc
CFLAGS = -O2 -Wall -Wextra -Wswitch-enum -std=c2x -pedantic -I$(IDIR)
LIBS =
IDIR = ./headers
CDIR = ./src

_HEADERS = gbvm.h
HEADERS = $(patsubst %,$(IDIR)/%,$(_HEADERS))

_CODE = gbvm.c
CODE = $(patsubst %,$(CDIR)/%,$(_CODE))

gbvm: main.c $(CODE)
	  $(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(CDIR)/*.o main.o gbvm
