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

	sem_wait(sem);

	sem_post(sem);

	//--------------INICIO CONSUMIDOR-----------------

	printf("\nBienvenido al consumidor, inicie este programa despues del productor para un correcto funcionamiento.");
	printf("\nLeyendo primer caracter de la cadena... ");

	caracter_leido=popColaPC(cola);

	while(caracter_leido!=0){

		printf("\nEl caracter leido es %c",caracter_leido);

		printf("\nLeyendo siguiente caracter de la cadena... ");

		caracter_leido=popColaPC(cola);
	}


	printf("\nFin de cadena detectado en caracter leido, terminando programa...\n");
	fflush(stdout);


	return 0;

}
