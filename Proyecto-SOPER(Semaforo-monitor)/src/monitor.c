#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>
#include <simulador.h>
#include <gamescreen.h>
#include <mapa.h>


void mapa_print(tipo_mapa * mapa)
{
	int i,j;

	printf("MONITOR: Mostrando el mapa: (debug)\n");

	for(j=0;j<MAPA_MAXY;j++) {
		for(i=0;i<MAPA_MAXX;i++) {
			tipo_casilla cas=mapa_get_casilla(mapa,j, i);
			printf("%c",cas.simbolo);
			screen_addch(j, i, cas.simbolo);
		}
		printf("\n");
	}
	screen_refresh();
}


int main() {

	tipo_mapa * mapa = NULL;
	sem_t * sem_simulador = NULL;

	if ((sem_simulador = sem_open(SEM, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {

        printf("MONITOR: Error abriendo el semaforo.\n");
        exit(EXIT_FAILURE);

	}

	sem_wait(sem_simulador);

	mapa = abrir_mapa();

	if(mapa == NULL){

		printf("El mapa no se ha abierto correctamente, finalizando...\n");
		exit(EXIT_FAILURE);

	}

	printf("MONITOR: Monitor iniciado... \n");

	screen_init();

	mapa_print(mapa);

	screen_end();
	sleep(5);
	printf("MONITOR: Cerrando el monitor...\n");

	sem_post(sem_simulador);

	cerrar_mapa(mapa);
	sem_close(sem_simulador);
	exit(EXIT_SUCCESS);
}



// ------ FUNCIONES AUXILIARES ------ //



// -----------------------------------------------------------------------


tipo_mapa * abrir_mapa(){

	tipo_mapa * mapa_aux = NULL;
	int fd_shm;

    // ------ ABRIMOS MEMORIA COMPARTIDA CON PERMISOS DE LECTURA ------ //

	fd_shm = shm_open(SHM_MAP_NAME, O_RDONLY, 0); 
	
	if(fd_shm == -1){
		fprintf (stderr, "MONITOR: Error abriendo la memoria compartida \n");
		return NULL;
	}

	/* MAPEAMOS LA MEMORIA COMPARTIDA */

	mapa_aux = mmap(NULL, sizeof(*mapa_aux), PROT_READ, MAP_SHARED, fd_shm, 0);

	if(mapa_aux == MAP_FAILED){

		fprintf (stderr, "MONITOR: Error mapeando la memoria compartida \n");
		return NULL;

	}
    
    return mapa_aux;

}


// -----------------------------------------------------------------------


void cerrar_mapa(tipo_mapa * mapa){

	if(mapa){

        munmap(mapa, sizeof(*mapa));
        shm_unlink(SHM_MAP_NAME);

    }

}