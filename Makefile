###################################################
#
# file: Makefile
#
# HY-255 Assignment 4: Sudoku
#
####################################################

CC = gcc
CFLAGS = -Wall -ansi -pedantic -O2
SRCDIR = src
OBJS = $(SRCDIR)/grid.o $(SRCDIR)/sudoku.o

.PHONY: all clean

all: sudoku

sudoku: $(OBJS)
	$(CC) $(CFLAGS) -o sudoku $(OBJS)

$(SRCDIR)/grid.o: $(SRCDIR)/grid.c $(SRCDIR)/grid.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/grid.c -o $(SRCDIR)/grid.o

$(SRCDIR)/sudoku.o: $(SRCDIR)/sudoku.c $(SRCDIR)/sudoku.h $(SRCDIR)/grid.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/sudoku.c -o $(SRCDIR)/sudoku.o

clean:
	-$(RM) $(OBJS) sudoku sudoku.exe
