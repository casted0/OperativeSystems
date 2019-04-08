CPP = gcc
CFLAGS = -g -Wall -pedantic 
LDLIBS = -lrt -pthread

BOLD=\e[1m
NC=\e[0m

PROGS = ejercicio2 ejercicio2_solved ejercicio3_productor ejercicio3_consumidor

all: $(PROGS)

clean: 
	@rm -f $(PROGS)

$(PROGS):
	$(CPP) $(CFLAGS) $@.c -o $@ $(LDLIBS)
