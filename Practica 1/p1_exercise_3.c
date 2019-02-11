#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 3

int main(void)
{
	pid_t pid;
	int i;
	for(i = 0; i < NUM_PROC; i++)
	{
		pid = fork();
		if(pid <  0)
		{
			printf("Error al emplear fork\n");
			exit(EXIT_FAILURE);
		}
		else if(pid ==  0)
		{
			printf("HIJO pid: %d, ppid: %d\n", getpid(), getppid());
			exit(EXIT_SUCCESS);
		}
		else if(pid > 0)
		{
			printf("PADRE %d\n", i);
			wait(NULL);
		}
	}
	wait(NULL);
	exit(EXIT_SUCCESS);
}

/*
	PREGUNTAS TEORICAS:

	- a) Para responder a esta pregunta solo hace falta ejecutar el programa multiples veces, veremos que no siempre el resultado va a ser el mismo
		 si realizamos el diagrama de forks para ver como se va a desarrollar el programa, es apreciable habiendo un unico wait al final, el proceso padre
		 va a seguir creando procesos hijos sin que necesariamente acaben todos ellos, por lo tanto no podemos predecir el orden en el que estos procesos hijo
		 van a finalizar.

	- b) Añadidos getpid() y getppid()

	- c) En la memoria
*/
