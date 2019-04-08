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

	int fd_shm,error,eleccion;
	sem_t *sem = NULL;
	ColaPC * cola;
	char caracter;

	//Por si acaso hay problemas, de ejecucion y no se liberan correctamente el semaforo o la memoria 
	//shm_unlink(SHM_NAME);
    //sem_unlink(SEM);

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

	sem_wait(sem); //Bloqueamos al consumidor ya que aun no hay nada en la cola y tenemos que ser los primeros

	printf("\nPor favor introduzca caracter: ");

	while(scanf("\n%c",&caracter)!=EOF){ 

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
		printf("\nHay %d caracteres en la cola",colaGetTamanio(cola));

		if(colaIsFull(cola)==TRUE){

			printf("\nCola llena, esperando a que el consumidor extraiga...");
			fflush(stdout);
			sem_post(sem);	//CEDEMOS EL TURNO AL CONSUMIDOR
			sleep(1);	    //ESPERAMOS 1 SEGUNDO PARA ASEGURAR QUE EL CONSUMIDOR ENTRA AL SEMAFORO
			sem_wait(sem);  //ESPERAMOS A QUE EL CONSUMIDOR NOS DEVUELVA EL TURNO

		}

		else{

			printf("\nSi quiere introducir otro caracter introduzca el numero 1");
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

				printf("\nCediendo el turno al consumidor...");
				fflush(stdout);
				sem_post(sem); 	//CEDEMOS EL TURNO AL CONSUMIDOR
				sleep(1);	    //ESPERAMOS 1 SEGUNDO PARA ASEGURAR QUE EL CONSUMIDOR ENTRA AL SEMAFORO
				sem_wait(sem);  //ESPERAMOS A QUE EL CONSUMIDOR NOS DEVUELVA EL TURNO
			}	
		}

		printf("\nEs tu turno, hay %d caracteres en la cola\n"
			"Por favor introduzca nuevo caracter: ",colaGetTamanio(cola));
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
