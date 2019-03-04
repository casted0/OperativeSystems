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
			printf("PID DEL HIJO = %d | PID de su padre = %d\n", getpid(), getppid()); /*Modificacion del ejer 3*/
			exit(EXIT_SUCCESS);
		}
		else if(pid >  0)
		{
			printf("PADRE %d\n", i);
			wait(NULL); /*Modificacion del ejer 4*/
		}
	}
	wait(NULL);
	exit(EXIT_SUCCESS);
}

