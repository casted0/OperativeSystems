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

a) El motivo de que el código del ejercicio 3 genere hijos que se quedan huérfanos, es que el padre no dispone de un wait() para 
   cada hijo, por lo tanto se puede dar el caso de que el proceso padre finalize antes de que lo haga el hijo

b) 



*/