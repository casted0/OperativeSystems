#-----------------------
# Makefile - Practica 2 - Sistemas Operativos 2018/19
#-----------------------

CC = gcc -pedantic
CFLAGS = -Wall -pthread
EXE = p2_ejercicio2 p2_ejercicio3 p2_ejercicio3d p2_ejercicio4 p2_ejercicio6a p2_ejercicio6b p2_ejercicio8 p2_ejercicio9

all : $(EXE)

clean : 
	rm -f $(EXE)

###########  EJECUTABLES ############


p2_ejercicio2 : p2_ejercicio2.c
	$(CC) $(CFLAGS) -o p2_ejercicio2 p2_ejercicio2.c

p2_ejercicio3 : p2_ejercicio3.c
	$(CC) $(CFLAGS) -o p2_ejercicio3 p2_ejercicio3.c

p2_ejercicio3d : p2_ejercicio3d.c
	$(CC) $(CFLAGS) -o p2_ejercicio3d p2_ejercicio3d.c

p2_ejercicio4 : p2_ejercicio4.c
	$(CC) $(CFLAGS) -o p2_ejercicio4 p2_ejercicio4.c

p2_ejercicio6a : p2_ejercicio6a.c
	$(CC) $(CFLAGS) -o p2_ejercicio6a p2_ejercicio6a.c	

p2_ejercicio6b : p2_ejercicio6b.c
	$(CC) $(CFLAGS) -o p2_ejercicio6b p2_ejercicio6b.c	

p2_ejercicio8 : p2_ejercicio8.c
	$(CC) $(CFLAGS) -o p2_ejercicio8 p2_ejercicio8.c

p2_ejercicio9 : p2_ejercicio9.c
	$(CC) $(CFLAGS) -o p2_ejercicio9 p2_ejercicio9.c


###########  EJERCICIOS ############

test_ejercicio2:
	@echo Ejecutando ejercicio 2
	@./p2_ejercicio2

test_ejercicio2_v:
	@echo Ejecutando ejercicio 2 con valgrind
	@valgrind --leak-check=full ./p2_ejercicio2

test_ejercicio3:
	@echo Ejecutando ejercicio 3
	@./p2_ejercicio3

test_ejercicio3_v:
	@echo Ejecutando ejercicio 3 con valgrind
	@valgrind --leak-check=full ./p2_ejercicio3

test_ejercicio3d:
	@echo Ejecutando ejercicio 3d
	@./p2_ejercicio3d

test_ejercicio3d_v:
	@echo Ejecutando ejercicio 3d con valgrind
	@valgrind --leak-check=full ./p2_ejercicio3d

test_ejercicio4:
	@echo Ejecutando ejercicio 4
	@./p2_ejercicio4

test_ejercicio4_v:
	@echo Ejecutando ejercicio 4 con valgrind
	@valgrind --leak-check=full ./p2_ejercicio4	

test_ejercicio6a:
	@echo Ejecutando ejercicio 6a
	@./p2_ejercicio6a

test_ejercicio6a_v:
	@echo Ejecutando ejercicio 6a con valgrind
	@valgrind --leak-check=full ./p2_ejercicio6a	

test_ejercicio6b:
	@echo Ejecutando ejercicio 6b
	@./p2_ejercicio6b

test_ejercicio6b_v:
	@echo Ejecutando ejercicio 6b con valgrind
	@valgrind --leak-check=full ./p2_ejercicio6b	

test_ejercicio8:
	@echo Ejecutando ejercicio 8
	@./p2_ejercicio8

test_ejercicio8_v:
	@echo Ejecutando ejercicio 8 con valgrind
	@valgrind --leak-check=full ./p2_ejercicio8

test_ejercicio9:
	@echo Ejecutando ejercicio 9
	@./p2_ejercicio9

test_ejercicio9_v:
	@echo Ejecutando ejercicio 9 con valgrind
	@valgrind --leak-check=full ./p2_ejercicio9


