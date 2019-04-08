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

	int fd_shm;
	sem_t *sem = NULL;
	char caracter_leido;
	ColaPC * cola;

	printf("\nBienvenido al consumidor, inicie este programa despues del productor para un correcto funcionamiento.");

	fflush(stdout);

	sem=sem_open(SEM, O_CREAT);


	//--------------INICIO MEMORY-----------------

	/* We open the shared memory */
	fd_shm = shm_open(SHM_NAME,
			O_RDWR, /* Obtain it and open for reading and writing*/
			0); /* Unused */ 

	if(fd_shm == -1) {
		fprintf (stderr, "Error opening the shared memory segment \n"); return EXIT_FAILURE;
	}
      
	/* Map the memory segment */
	cola = (ColaPC*)mmap(NULL, sizeof(*cola), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
	if(cola == MAP_FAILED) {
		fprintf (stderr, "Error mapping the shared memory segment \n");
		return EXIT_FAILURE;
	}

	//--------------FIN MEMORY-----------------

	//--------------INICIO CONSUMIDOR-----------------

	sem_wait(sem);

	printf("\nLeyendo primer caracter de la cadena... ");

	fflush(stdout);

	caracter_leido=popColaPC(cola);

	while(caracter_leido!=0&&caracter_leido!=24){

		printf("\nEl caracter leido es %c",caracter_leido);
		fflush(stdout);

		sem_post(sem);

		sleep(2);

		sem_wait(sem);

		printf("\nLeyendo siguiente caracter de la cadena... ");

		caracter_leido=popColaPC(cola);
	}

	if(caracter_leido==24){
		printf("\nHemos leido de la cola cuando esta estaba vacia...\n");
		printf("Terminando programa...\n");
		fflush(stdout);

		sem_post(sem);

		munmap(cola, sizeof(*cola));
		sem_close(sem);


		return -1;	
	}

	printf("\nFin de cadena detectado en caracter leido, terminando programa...\n");
	fflush(stdout);

	sem_post(sem);

	munmap(cola, sizeof(*cola));
	sem_close(sem);

	return 0;

}
