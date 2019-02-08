#-----------------------
# Makefile - Practica 1 - Sistemas Operativos 2018/19
#-----------------------

CC = gcc -pedantic
CFLAGS = -Wall
EXE = p1_exercise_3 p1_exercise_5 p1_exercise_7 p1_exercise_8 p1_exercise_9 p1_exercise_11

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o p1_exercise_3.o p1_exercise_5.o p1_exercise_7.o p1_exercise_8.o p1_exercise_9.o p1_exercise_11.o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -lpthread -lm -o $@ $@.o p1_exercise_3.o p1_exercise_5.o p1_exercise_7.o p1_exercise_8.o p1_exercise_9.o p1_exercise_11.o

p1_exercise_3.o : p1_exercise_3.c
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

 p1_exercise_5.o : p1_exercise_5.c 
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

 p1_exercise_7.o : p1_exercise_7.c 
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

 p1_exercise_8.o : p1_exercise_8.c 
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

 p1_exercise_9.o : p1_exercise_9.c 
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

 p1_exercise_11.o : p1_exercise_11.c 
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

ejercicio3_test:
	@echo Ejecutando ejercicio 3
	@./p1_exercise_3

ejercicio3_test_v:
	@echo Ejecutando ejercicio 3
	@valgrind --leak-check=full ./p1_exercise_3

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


