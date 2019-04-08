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

	int fd_shm,eleccion;
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

	printf("\nExtrayendo primer caracter de la cadena... ");

	fflush(stdout);

	caracter_leido=popColaPC(cola);

	while(caracter_leido!=0&&caracter_leido!=24){

		printf("\nEl caracter leido es %c",caracter_leido);
		printf("\nHay %d caracteres en la cola",colaGetTamanio(cola));

		if(colaIsEmpty(cola)==TRUE){

			printf("\nCola vacia, esperando a que el productor inserte caracter en la cola...");
			fflush(stdout);
			sem_post(sem);   //CEDEMOS EL TURNO AL PRODUCTOR
			sleep(1);        //ESPERAMOS 1 SEGUNDO PARA ASEGURAR QUE EL PRODUCTOR ENTRA AL SEMAFORO
			sem_wait(sem);	 //ESPERAMOS A QUE EL PRODUCTOR NOS DEVUELVA EL TURNO

		}

		else{

			printf("\nSi quiere extraer otro caracter introduzca el numero 1");
			printf("\nSi quiere ceder el turno al consumido introduzca el numero 0");
			printf("\nIntroduzca eleccion: ");
			scanf("\n%d",&eleccion);

			while(eleccion!=1&&eleccion!=0){

				printf("\nEleccion invalida");
				printf("\nSi quiere introducir otro caracter introduzca el numero 1");
				printf("\nSi quiere ceder el turno al consumido introduzca el numero 0");
				printf("\nIntroduzca eleccion: ");
				scanf("\n%d",&eleccion);

			}


			if(!eleccion){

				printf("\nCediendo el turno al productor...");
				fflush(stdout);
				sem_post(sem);	//CEDEMOS EL TURNO AL PRODUCTOR
				sleep(1);		//ESPERAMOS 1 SEGUNDO PARA ASEGURAR QUE EL PRODUCTOR ENTRA AL SEMAFORO
				sem_wait(sem); //ESPERAMOS A QUE EL PRODUCTOR NOS DEVUELVA EL TURNO
			}

		}

		printf("\nEs tu turno, hay %d caracteres en la cola\n"
		 "Extrayendo siguiente caracter de la cadena... ",colaGetTamanio(cola));

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
