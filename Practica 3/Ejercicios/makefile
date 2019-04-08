CPP = gcc
CFLAGS = -g -Wall -pedantic 
LDLIBS = -lrt -pthread

BOLD=\e[1m
NC=\e[0m

PROG2 = ejercicio2 ejercicio2_solved 
PROG3 = ejercicio3_productor ejercicio3_consumidor
OBJECTSP3C = ColaPC.o ejercicio3_consumidor.o
OBJECTSP3P = ColaPC.o ejercicio3_productor.o

all: $(PROG2) $(PROG3)

clean: 
	@rm -f $(PROG2) $(PROG3) *.o

$(PROG2):
	$(CPP) $(CFLAGS) $@.c -o $@ $(LDLIBS)

ejercicio3_productor: $(OBJECTSP3P)
	$(CPP) $(CFLAGS) -o $@ $(LDLIBS) $(OBJECTSP3P)


ejercicio3_consumidor: $(OBJECTSP3C)
	$(CPP) $(CFLAGS) -o $@ $(LDLIBS) $(OBJECTSP3C)


ejercicio3_productor.o: ejercicio3_productor.c ColaPC.h
	$(CPP) $(CFLAGS) -c ejercicio3_productor.c $(LDLIBS)

ejercicio3_consumidor.o: ejercicio3_consumidor.c ColaPC.h
	$(CPP) $(CFLAGS) -c ejercicio3_consumidor.c $(LDLIBS)

ColaPC.o: ColaPC.c ColaPC.h
	$(CPP) $(CFLAGS) -c ColaPC.c