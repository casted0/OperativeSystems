#-----------------------
# Makefile - Practica 1 - Sistemas Operativos 2018/19
#-----------------------

CC = gcc -pedantic
CFLAGS = -Wall -pthread
EXE = p1_exercise_3 p1_exercise_4 p1_exercise_5 p1_exercise_7 p1_exercise_8 p1_exercise_9 p1_exercise_11

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

p1_exercise_3 : p1_exercise_3.o
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_3 "
	@echo "# Depende de p1_exercise_3"
	@echo "# Ha cambiado p1_exercise_3"
	$(CC) $(CFLAGS) -o p1_exercise_3 p1_exercise_3.o

p1_exercise_4 : p1_exercise_4.o
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_4"
	@echo "# Depende de p1_exercise_4"
	@echo "# Ha cambiado p1_exercise_4"
	$(CC) $(CFLAGS) -o p1_exercise_4 p1_exercise_4.o

p1_exercise_5 : p1_exercise_5.o
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_5 "
	@echo "# Depende de p1_exercise_5"
	@echo "# Ha cambiado p1_exercise_5"
	$(CC) $(CFLAGS) -o p1_exercise_5 p1_exercise_5.o

p1_exercise_7 : p1_exercise_7.o
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_7"
	@echo "# Depende de p1_exercise_7"
	@echo "# Ha cambiado p1_exercise_7"
	$(CC) $(CFLAGS) -o p1_exercise_7 p1_exercise_7.o

p1_exercise_8 : p1_exercise_8.o
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_8 "
	@echo "# Depende de p1_exercise_8"
	@echo "# Ha cambiado p1_exercise_8"
	$(CC) $(CFLAGS) -o p1_exercise_8 p1_exercise_8.o

p1_exercise_9 : p1_exercise_9.o
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_9 "
	@echo "# Depende de p1_exercise_9"
	@echo "# Ha cambiado p1_exercise_9"
	$(CC) $(CFLAGS) -o p1_exercise_9 p1_exercise_9.o

p1_exercise_11 : p1_exercise_11.o
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_11 "
	@echo "# Depende de p1_exercise_11"
	@echo "# Ha cambiado p1_exercise_11"
	$(CC) $(CFLAGS) -o p1_exercise_11 p1_exercise_11.o
	rm -rf *.o

p1_exercise_3.o : p1_exercise_3.c
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_3.o"
	@echo "# Depende de p1_exercise_3.o"
	@echo "# Ha cambiado p1_exercise_3.o"
	$(CC) $(CFLAGS) -c p1_exercise_3.c

p1_exercise_4.o : p1_exercise_4.c
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_4.o"
	@echo "# Depende de p1_exercise_4.o"
	@echo "# Ha cambiado p1_exercise_4.o"
	$(CC) $(CFLAGS) -c p1_exercise_4.c

p1_exercise_5.o : p1_exercise_5.c
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_5.o"
	@echo "# Depende de p1_exercise_5.o"
	@echo "# Ha cambiado p1_exercise_5.o"
	$(CC) $(CFLAGS) -c p1_exercise_5.c

p1_exercise_7.o : p1_exercise_7.c
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_7.o"
	@echo "# Depende de p1_exercise_7.o"
	@echo "# Ha cambiado p1_exercise_7.o"
	$(CC) $(CFLAGS) -c p1_exercise_7.c

p1_exercise_8.o : p1_exercise_8.c
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_8.o"
	@echo "# Depende de p1_exercise_8.o"
	@echo "# Ha cambiado p1_exercise_8.o"
	$(CC) $(CFLAGS) -c p1_exercise_8.c

p1_exercise_9.o : p1_exercise_9.c
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_9.o"
	@echo "# Depende de p1_exercise_9.o"
	@echo "# Ha cambiado p1_exercise_9.o"
	$(CC) $(CFLAGS) -c p1_exercise_9.c

p1_exercise_11.o : p1_exercise_11.c
	@echo "#---------------------------"
	@echo "# Generando p1_exercise_11.o"
	@echo "# Depende de p1_exercise_11.o"
	@echo "# Ha cambiado p1_exercise_11.o"
	$(CC) $(CFLAGS) -c p1_exercise_11.c

ejercicio3_test:
	@echo Ejecutando ejercicio 3
	@./p1_exercise_3

ejercicio3_test_v:
	@echo Ejecutando ejercicio 3
	@valgrind --leak-check=full ./p1_exercise_3

ejercicio4_test:
	@echo Ejecutando ejercicio 4
	@./p1_exercise_4

ejercicio4_test_v:
	@echo Ejecutando ejercicio 4
	@valgrind --leak-check=full ./p1_exercise_4

ejercicio5_test:
	@echo Ejecutando ejercicio 5
	@./p1_exercise_5

ejercicio5_test_v:
	@echo Ejecutando ejercicio 5
	@valgrind --leak-check=full ./p1_exercise_5

ejercicio7_test:
	@echo Ejecutando ejercicio 7
	@./p1_exercise_7

ejercicio7_test_v:
	@echo Ejecutando ejercicio 7
	@valgrind --leak-check=full ./p1_exercise_7

ejercicio8_test:
	@echo Ejecutando ejercicio1
	@./p1_exercise_3

ejercicio8_test_v:
	@echo Ejecutando ejercicio1
	@valgrind --leak-check=full ./p1_exercise_3

ejercicio9_test:
	@echo Ejecutando ejercicio 9
	@./p1_exercise_9

ejercicio9_test_v:
	@echo Ejecutando ejercicio 9
	@valgrind --leak-check=full ./p1_exercise_9

ejercicio11_test:
	@echo Ejecutando ejercicio 11
	@./p1_exercise_11

ejercicio11_test_v:
	@echo Ejecutando ejercicio 11
	@valgrind --leak-check=full ./p1_exercise_11


