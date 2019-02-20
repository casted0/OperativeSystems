#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 6


int main(void)
{
    pid_t pid;
	int i;

	for(i = 0; i < NUM_PROC; i++)
	{
		if(fork()){
			if(i == 0)
				printf("PADRE %d\n", getpid());
			break;
		}else{
			printf("HIJO %d de %d\n", getpid(), getppid());
		}
	} 

	if(i<5)
		wait(NULL);

<<<<<<< HEAD
	wait(NULL);
    sleep(10000);
=======
>>>>>>> 2dc737aea2cd63e7d699c52a8402def8b86e7136
	exit(EXIT_SUCCESS);
}

/*

a) El motivo de que el código del ejercicio 3 genere hijos que se quedan huérfanos, es que el padre no dispone de un wait() para 
   cada hijo, por lo tanto se puede dar el caso de que el proceso padre finalize antes de que lo haga el hijo

b) Introducido wait(NULL en el p1_exercise_3.c)

c) Hecho

*/
