CPP = gcc
CFLAGS = -g -Wall -pedantic 
LDLIBS = -lrt -pthread

BOLD=\e[1m
NC=\e[0m

PROGS = p3_ejercicio2 p3_ejercicio2_solved

all: $(PROGS)

clean: 
	@rm -f $(PROGS)

$(PROGS):
	$(CPP) $(CFLAGS) $@.c -o $@ $(LDLIBS)
