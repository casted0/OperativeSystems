 
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


#define SEM "/example_sem"

#define N_READ 5
#define SECS 3

	Lectura() {
		Down(sem_lectura);
		lectores++;
		if (lectores == 1)
			Down(sem_escritura);
		Up(sem_lectura);
		
		Leer();

		Down(sem_lectura);
		lectores--;
		if (lectores == 0)
			Up(sem_escritura);
		Up(sem_lectura);

	}


	Escritura() {
		Down(sem_escritura);

		Escribir();

		Up(sem_escritura);
	}

	int main(){

		int i,pid;

		for(i=0;i<N_READ;i++){

			if(!pid=fork()){
				break;
			}
		}

		if(!pid){
			
			while(true){
			
				printf("\nR-INI %d",getpid());
				sleep(1);
				printf("\nR-FIN %d",getpid());

				sleep(SECS);
			}

		}
		else{

			while(true){

				printf("W-INI %d",getpid());
				sleep(1);
				printf("W-FIN %d",getpid());

				sleep(SECS);
			}
		}

	}
