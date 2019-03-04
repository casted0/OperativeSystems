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

	pid = fork();
	if(pid <  0)
	{
		printf("Error al emplear fork\n");
		exit(EXIT_FAILURE);
	}
	else if(pid >  0)
	{
		printf("\nPADRE ORIGINAL CON PID %d\n", getpid());
	}

	else if(pid ==  0)
	{
		for(i=0;i<NUM_PROC;i++){
			if(pid == 0){
				printf("Hijo %d con PID = %d y PADRE = %d\n",i,getpid(),getppid());
				pid = fork();
			}
			else{
				wait(NULL);
				exit(EXIT_SUCCESS);
			}
		}

	}

	wait(NULL);
	exit(EXIT_SUCCESS);
} 
