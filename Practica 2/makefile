#-----------------------
# Makefile - Practica 1 - Sistemas Operativos 2018/19
#-----------------------

CC = gcc -pedantic
CFLAGS = -Wall -pthread
EXE = p2_example_pause p2_example_sem p2_example_signal p2_example_sigprocmask p2_example_alarm p2_ejercicio2 p2_ejercicio3 p2_ejercicio3d clean

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core

.PHONY : remove
remove : 
	rm -f $(EXE)

###########  EJECUTABLES ############

p2_example_pause : p2_example_pause.o
	@echo "#---------------------------"
	@echo "# Generando p2_example_pause "
	@echo "# Depende de p2_example_pause.o"
	@echo "# Ha cambiado p2_example_pause"
	$(CC) $(CFLAGS) -o p2_example_pause p2_example_pause.o

p2_example_sem : p2_example_sem.o
	@echo "#---------------------------"
	@echo "# Generando p2_example_sem"
	@echo "# Depende de p2_example_sem.o"
	@echo "# Ha cambiado p2_example_sem"
	$(CC) $(CFLAGS) -o p2_example_sem p2_example_sem.o

p2_example_signal : p2_example_signal.o
	@echo "#---------------------------"
	@echo "# Generando p2_example_signal "
	@echo "# Depende de p2_example_signal.o"
	@echo "# Ha cambiado p2_example_signal"
	$(CC) $(CFLAGS) -o p2_example_signal p2_example_signal.o

p2_example_sigproc : p2_example_sigproc.o
	@echo "#---------------------------"
	@echo "# Generando p2_example_sigproc"
	@echo "# Depende de p2_example_sigproc.o"
	@echo "# Ha cambiado p2_example_sigproc"
	$(CC) $(CFLAGS) -o p2_example_sigproc p2_example_sigproc.o

p2_example_alarm : p2_example_alarm.o
	@echo "#---------------------------"
	@echo "# Generando p2_example_alarm "
	@echo "# Depende de p2_example_alarm.o"
	@echo "# Ha cambiado p2_example_alarm"
	$(CC) $(CFLAGS) -o p2_example_alarm p2_example_alarm.o

p2_ejercicio2 : p2_ejercicio2.o
	@echo "#---------------------------"
	@echo "# Generando p2_ejercicio2 "
	@echo "# Depende de p2_ejercicio2"
	@echo "# Ha cambiado p2_ejercicio2"
	$(CC) $(CFLAGS) -o p2_ejercicio2 p2_ejercicio2.o

p2_ejercicio3 : p2_ejercicio3.o
	@echo "#---------------------------"
	@echo "# Generando p2_ejercicio3"
	@echo "# Depende de p2_ejercicio3"
	@echo "# Ha cambiado p2_ejercicio3"
	$(CC) $(CFLAGS) -o p2_ejercicio3 p2_ejercicio3.o

p2_ejercicio3d : p2_ejercicio3d.o
	@echo "#---------------------------"
	@echo "# Generando p2_ejercicio3d"
	@echo "# Depende de p2_ejercicio3d"
	@echo "# Ha cambiado p2_ejercicio3d"
	$(CC) $(CFLAGS) -o p2_ejercicio3d p2_ejercicio3d.o

p2_ejercicio4 : p2_ejercicio4.o
	@echo "#---------------------------"
	@echo "# Generando p2_ejercicio4 "
	@echo "# Depende de p2_ejercicio4.o"
	@echo "# Ha cambiado p2_ejercicio4"
	$(CC) $(CFLAGS) -o p2_ejercicio4 p2_ejercicio4.o

###########  OBJETOS ############

p2_example_pause.o : p2_example_pause.c
	@echo "#---------------------------"
	@echo "# Generando p2_example_pause.o "
	@echo "# Depende de p2_example_pause.c"
	@echo "# Ha cambiado p2_example_pause.o"
	$(CC) $(CFLAGS) -c p2_example_pause.c

p2_example_sem.o : p2_example_sem.c
	@echo "#---------------------------"
	@echo "# Generando p2_example_sem.o "
	@echo "# Depende de p2_example_sem.c"
	@echo "# Ha cambiado p2_example_sem.o"
	$(CC) $(CFLAGS) -c p2_example_sem.c

p2_example_signal.o : p2_example_signal.c
	@echo "#---------------------------"
	@echo "# Generando p2_example_signal.o "
	@echo "# Depende de p2_example_signal.c"
	@echo "# Ha cambiado p2_example_signal.o"
	$(CC) $(CFLAGS) -c p2_example_signal.c

p2_example_sigproc.o : p2_example_sigprocmask.c
	@echo "#---------------------------"
	@echo "# Generando p2_example_sigproc.o "
	@echo "# Depende de p2_example_sigproc.c"
	@echo "# Ha cambiado p2_example_sigproc.o"
	$(CC) $(CFLAGS) -c p2_example_sigprocmask.c

p2_example_alarm.o : p2_example_alarm.c
	@echo "#---------------------------"
	@echo "# Generando p2_example_alarm.o "
	@echo "# Depende de p2_example_alarm.c"
	@echo "# Ha cambiado p2_example_alarm.o"
	$(CC) $(CFLAGS) -c p2_example_alarm.c

p2_ejercicio2.o : p2_ejercicio2.c
	@echo "#---------------------------"
	@echo "# Generando p2_ejercicio2.o "
	@echo "# Depende de p2_ejercicio2.o"
	@echo "# Ha cambiado p2_ejercicio2.o"
	$(CC) $(CFLAGS) -c p2_ejercicio2.c

p2_ejercicio3.o : p2_ejercicio3.c
	@echo "#---------------------------"
	@echo "# Generando p2_ejercicio3.o "
	@echo "# Depende de p2_ejercicio3.o"
	@echo "# Ha cambiado p2_ejercicio3.o"
	$(CC) $(CFLAGS) -c p2_ejercicio3.c

p2_ejercicio3d.o : p2_ejercicio3d.c
	@echo "#---------------------------"
	@echo "# Generando p2_ejercicio3d.o "
	@echo "# Depende de p2_ejercicio3d.o"
	@echo "# Ha cambiado p2_ejercicio3d.o"
	$(CC) $(CFLAGS) -c p2_ejercicio3d.c

p2_ejercicio4.o : p2_ejercicio4.c
	@echo "#---------------------------"
	@echo "# Generando p2_ejercicio4.o "
	@echo "# Depende de p2_ejercicio4.o"
	@echo "# Ha cambiado p2_ejercicio4.o"
	$(CC) $(CFLAGS) -c p2_ejercicio4.c

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

###########   EJEMPLOS  ############

test_example_alarm:
	@echo Ejecutando ejemplo de alarma
	@./p2_example_alarm

test_example_alarm_v:
	@echo Ejecutando ejemplo de alarma con valgrind
	@valgrind --leak-check=full ./p2_example_alarm

test_example_sem:
	@echo Ejecutando ejemplo de sem
	@./p2_example_sem

test_example_sem_v:
	@echo Ejecutando ejemplo de sem con valgrind
	@valgrind --leak-check=full ./p2_example_sem

test_example_sigprocmask:
	@echo Ejecutando ejemplo de sigprogmask
	@./p2_example_sigprocmask

test_example_sigprocmask_v:
	@echo Ejecutando ejemplo de sigprocmask con valgrind
	@valgrind --leak-check=full ./p2_example_sigprocmask

test_example_pause:
	@echo Ejecutando ejemplo de pause
	@./p2_example_pause

test_example_pause_v:
	@echo Ejecutando ejemplo de pause con valgrind
	@valgrind --leak-check=full ./p2_example_pause


