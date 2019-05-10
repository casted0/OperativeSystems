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

int fd_shm, fd_shm_aux;

void mapa_print(tipo_mapa * mapa)
{
	int i, j, k;
	tipo_nave nave_print;

	for(j=0;j<MAPA_MAXY;j++) {
		for(i=0;i<MAPA_MAXX;i++) {
			tipo_casilla cas=mapa_get_casilla(mapa,j, i);
			//printf("%c",cas.simbolo);
			screen_addch(j, i, cas.simbolo);
		}
		//printf("\n");
	}

	for(i = 0; i < (2 * MAPA_MAXX); i++){
		screen_addch((j + 4), i, '-');
	}

	k = j + 8;

	for(j = 0; j<MAPA_MAXY; j++, k++) {
		for(i = 0; i<MAPA_MAXX; i++) {
			tipo_casilla cas=mapa_get_casilla(mapa,j, i);
			if(cas.simbolo == SYMB_VACIO){
				screen_addch(k, i, cas.simbolo);
			}else{
				nave_print = mapa_get_nave(mapa, cas.equipo, cas.numNave);
				screen_addch_int(k, i, (nave_print.numNave + '0'));
			}
			
		}
	}

	screen_refresh();
}


int main() {

	tipo_mapa * mapa = NULL;
	sem_t * sem_simulador = NULL;

	if ((sem_simulador = sem_open(SEM, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {

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

	// COMPROBAR QUE SE HA DISPUESTO TODO EN EL MAPA

	screen_init();

	sem_post(sem_simulador);

	while(mapa->terminado==false){
		
		
		mapa_print(mapa);
		/*	
		fd_shm_aux = shm_open(SHM_MAP_NAME, O_RDONLY, 0);
		if(fd_shm_aux == -1){
			mapa = NULL;
		}*/
		usleep(SCREEN_REFRESH);

	}

	//quitar luego
	//sleep(100);

	screen_end();


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

	if(mapa != NULL){

        munmap(mapa, sizeof(*mapa));
        shm_unlink(SHM_MAP_NAME);

    }

}