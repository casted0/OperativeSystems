CPP = gcc
CFLAGS = -g -Wall -pedantic
LDLIBS = -lrt

BOLD=\e[1m
NC=\e[0m

PROGS = p3_ejercicio4 p3_ejercicio4_procesoA p3_ejercicio4_procesoB p3_ejercicio4_procesoC limpiarMQ

all: clean $(PROGS)

clean: 
	@rm -f $(PROGS)

$(PROGS):
	$(CPP) $(CFLAGS) $@.c -o $@ $(LDLIBS)
