 
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define N_READ 5
#define SECS 3


int pid_hijos[N_READ];
int lectores=0;

	void manejador(int sig){
		int i;
    	printf("He conseguido capturar SIGKILL \n");
    	fflush(stdout);

    	for(i=0;i<N_READ;i++){
    		kill(pid_hijos[i],SIGTERM);
    	}

    	//sem_close(sem_escritura);
    	//sem_close(sem_lectura);

    	sem_unlink("/SEM_ESCRIT");
		sem_unlink("/SEM_LECT");
    	
    	exit(EXIT_SUCCESS);
	}



	void Lectura(sem_t * sem_lectura, sem_t * sem_escritura) {

		sem_wait(sem_lectura);
		lectores++;
		if (lectores == 1)
			sem_wait(sem_escritura);
		sem_post(sem_lectura);
		
		/*--------LEER------------*/
		printf("\nR-INI %d",getpid());
		sleep(1);
		printf("\nR-FIN %d",getpid());
		sleep(SECS);
		/*--------FIN LEER------------*/


		sem_wait(sem_lectura);
		lectores--;
		if (lectores == 0)
			sem_post(sem_escritura);
		sem_post(sem_lectura);

	}


	void Escritura(sem_t * sem_escritura) {
		sem_wait(sem_escritura);

		/*--------ESCRIBIR------------*/
		printf("W-INI %d",getpid());
		sleep(1);
		printf("W-FIN %d",getpid());

		sleep(SECS);

		/*--------FIN ESCRIBIR-----------*/

		sem_post(sem_escritura);
	}

	int main(){

		int i,pid;
		sem_t * sem_lectura=NULL;
		sem_t * sem_escritura=NULL;

		//sem_unlink("/SEM_ESCRIT");
		//sem_unlink("/SEM_LECT");


		if ((sem_lectura = sem_open("/SEM_LECT", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, N_READ)) == SEM_FAILED) {
			perror("sem_open");
			exit(EXIT_FAILURE);
		}

		if ((sem_escritura = sem_open("/SEM_ESCRIT", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
			perror("sem_open");
			exit(EXIT_FAILURE);
		}


		struct sigaction act;
    	act.sa_handler = manejador;
    	sigemptyset(&(act.sa_mask));
    	act.sa_flags =0;

 		if(sigaction(SIGINT,&act,NULL)<0){
        	perror("sigaction");
        	exit(EXIT_FAILURE);
    	}

		for(i=0;i<N_READ;i++){

			if(!(pid=fork())){
				pid_hijos[i]=getpid();
				break;
			}
		}

		if(!pid){
			//hijos lectores
			while(1){

				Lectura(sem_lectura,sem_escritura);
				
			}


		}
		else{
			//padre escritor
			while(1){


				Escritura(sem_escritura);

			}
		}

		//sem_unlink(sem_escritura);
		//sem_unlink(sem_lectura);

		return 0;

	}
