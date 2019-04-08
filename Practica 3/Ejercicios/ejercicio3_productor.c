#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "ColaPC.h"

#define SEM "/sem_p3_ejer3" 
#define SHM_NAME "/mq_p3_ejer3" 


int main(){

	int fd_shm,error;
	sem_t *sem = NULL;
	ColaPC * cola;
	char caracter;

	if((sem = sem_open(SEM , O_CREAT | O_EXCL, S_IRUSR | S_IWUSR , 1))== SEM_FAILED){
        perror("sem_open");
        exit(EXIT_FAILURE);
    } 


    //--------------MEMORY-----------------

	/* We create the shared memory */
	fd_shm = shm_open(SHM_NAME,
		O_RDWR | O_CREAT | O_EXCL, /* Create it and open for reading and writing */ 
		S_IRUSR | S_IWUSR); /* The current user can read and write */

	if(fd_shm == -1) {
		fprintf (stderr, "Error creating the shared memory segment \n");
		sem_close(sem);
		sem_unlink(SEM);
		return EXIT_FAILURE;
	}
    
    /* Resize the memory segment */
	error = ftruncate(fd_shm, sizeof(ColaPC));

	if(error == -1) {
		fprintf (stderr, "Error resizing the shared memory segment \n");
		sem_close(sem);
		shm_unlink(SHM_NAME);
		sem_unlink(SEM);
		return EXIT_FAILURE;
	}
    
	/* Map the memory segment */
	cola = (ColaPC *)mmap(NULL, sizeof(*cola), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);

	if(cola == MAP_FAILED) {
		fprintf (stderr, "Error mapping the shared memory segment \n");
		sem_close(sem);
		shm_unlink(SHM_NAME);
		sem_unlink(SEM);
		return EXIT_FAILURE;
	}


	//--------------FIN MEMORY-----------------


	//--------------INICIO PRODUCTOR-----------------

	printf("\nBienvenido al productor, inicie este programa antes del consumidor para un correcto funcionamiento.");

	sem_wait(sem);

	printf("\nPor favor introduzca caracter: ");

	while(scanf("%c",&caracter)!=EOF && caracter!=48){

		if(pushColaPC(cola, caracter)==ERROR){
			printf("Error en productor al introducir caracter en la colaPC\n");
			fflush(stdout);

			munmap(cola, sizeof(*cola));
			sem_close(sem);
			shm_unlink(SHM_NAME);
		    sem_unlink(SEM);
			return -1;
		}

		printf("\nCaracter introducido a la cola correctamente");

		sem_post(sem);

		sleep(1);

		sem_wait(sem);

		printf("\nPor favor introduzca nuevo caracter: ");
	}

	printf("\nEOF detectado, terminando programa...\n");
	fflush(stdout);

	caracter=0;

	if(pushColaPC(cola, caracter)==ERROR){
		printf("Error en productor al introducir caracter 'Fin de Cadena' en la colaPC\n");
		fflush(stdout);

		sem_post(sem);
		sleep(1);

		munmap(cola, sizeof(*cola));
		sem_close(sem);
		shm_unlink(SHM_NAME);
		sem_unlink(SEM);
		return -1;
	}

	sem_post(sem);

	sleep(1);	

	munmap(cola, sizeof(*cola));
	sem_close(sem);
	shm_unlink(SHM_NAME);
    sem_unlink(SEM);

	return 0;

}
