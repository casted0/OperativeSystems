#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <mapa.h>

// Variables globales usadas para contar el numero de naves

int equipo0 = 0;
int equipo1 = 0;
int equipo2 = 0;


int main() {

    tipo_mapa * mapa = NULL;    // Mapa del simulador
    int pid = 1, pid_nave = 1;  // Pids para generar jefes y naves de cada jefe
    int i, j;                   // Indices
    sem_t * sem_simulador = NULL;

	printf("SIMULADOR: Simulador iniciado...\n");

    if ((sem_simulador = sem_open(SEM, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {

        printf("SIMULADOR: Error creando el semaforo.\n");
        exit(EXIT_FAILURE);

    }

    mapa = iniciar_mapa();

    if(mapa == NULL){
        printf("SIMULADOR: No se ha iniciado la partida correctamente.\n");
        exit(EXIT_FAILURE);
    }

    // FUNCION PARA DISPONER LAS NAVES PARA EL COMIENZO (Y SETTEAR EL RESTO DE LA MEMORIA)

    estado_inicial_mapa(mapa);

    // COMPROBAR QUE SE HA DISPUESTO TODO EN EL MAPA

    printf("Simbolo de la casilla (5, 5): [%c]\n", mapa_get_symbol(mapa, 5, 5));


    // ABRIMOS EL SEMAFORO PARA QUE ENTRE EL MONITOR AHORA QUE HAY MAPA

    sem_post(sem_simulador);
    
    // INICIAR PROCESOS JEFE, 3 PORQUE HAY 3 EQUIPOS

    for(i = 0; i < N_EQUIPOS; i++){

        if(pid != 0){
            pid = fork();
        }else if(pid == -1){
            printf("SIMULADOR: Error creando procesos jefes.\n");
            exit(EXIT_FAILURE);
        }else{
            break;
        }

    }

    // ------ CODIGO DE LOS JEFES Y ESPERA DEL SIMULADOR A QUE ACABEN ------ //

    if(pid == 0){

        usleep(500);
        printf("JEFE [%d]: Jefe creado.\n", (i-1));
        
        for(j = 0; j < N_NAVES; j++){

            if(pid_nave != 0){
                pid_nave = fork();
            }else if(pid_nave == -1){
                printf("JEFE [%d]: Error creando procesos nave.\n", (i-1));
            }else{
                break;
            }

        }

    }
        
    if(pid_nave == 0){

        usleep(500);
        printf("NAVE [%d]: Nave creada, pertenezco al jefe %d.\n", (j-1), (i-1));
        sleep(2);

    }

    // ------ LIBERAR Y ACABAR ------ //

    if(pid_nave != 0)
        for(i = 0; i < N_NAVES; i++){ wait(NULL); }

    if(pid != 0)
        for(i = 0; i < N_EQUIPOS; i++){ wait(NULL); }

    sleep(4);

    destruir_mapa(mapa);
    sem_close(sem_simulador);
 	sem_unlink(SEM);
    exit(EXIT_SUCCESS);

}


// ------ FUNCIONES AUXILIARES ------ //


//------------------------------------------------------------------------------------------



tipo_mapa * iniciar_mapa(){

    // ------ CREAMOS MEMORIA COMPARTIDA CON PERMISOS DE LECTURA Y ESCRITURA ------ //

    int fd_shm = shm_open(SHM_MAP_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR); 
    
    if(fd_shm == -1){
        fprintf (stderr, "SIMULADOR: Error creando la memoria compartida del mapa.\n");
        return NULL;
    }
    
    // ------ AJUSTAMOS EL TAMAÑO DE LA MEMORIA PARA QUE SEA EL DE TIPO_MAPA ------ //
    
    int error = ftruncate(fd_shm, sizeof(tipo_mapa));

    if(error == -1){
        fprintf (stderr, "SIMULADOR: Error ajustando el tamaño de la memoria compartida al tipo_mapa. \n");
        shm_unlink(SHM_MAP_NAME);
        return NULL;
    }
    
    /* MAPEAMOS LA MEMORIA COMPARTIDA */
    
    tipo_mapa * mapa_aux = mmap(NULL, sizeof(*mapa_aux), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    
    if(mapa_aux == MAP_FAILED){
        fprintf (stderr, "SIMULADOR: Error mapeando la memoria creada. \n");
        shm_unlink(SHM_MAP_NAME);
        return NULL;
    }
    
    return mapa_aux;

}


//------------------------------------------------------------------------------------------



void estado_inicial_mapa(tipo_mapa * mapa){

    int i;

    if(mapa == NULL){
        printf("SIMULADOR: No se puede inicializar, mapa vacio.\n");
        return;
    }

    estado_inicial_naves(mapa);                 // RELLENAMOS LOS VALORES DE LAS NAVES
    estado_inicial_casillas(mapa);                // RELLENAMOS LOS VALORES DE LAS CASILLAS
    for(i = 0; i < N_EQUIPOS; mapa->num_naves[i] = 3, i++); // INDICAMOS QUE HAY 3 NAVES POR EQUIPO

}



//------------------------------------------------------------------------------------------



void estado_inicial_naves(tipo_mapa * mapa){

    int i, j;

    if(mapa == NULL){
        printf("SIMULADOR: No se puede inicializar, mapa vacio.\n");
        return;
    }

    for(i = 0; i < N_EQUIPOS; i++){
        for(j = 0; j < N_NAVES; j++){

            mapa->info_naves[i][j].equipo = i;
            mapa->info_naves[i][j].vida = VIDA_MAX;
            mapa->info_naves[i][j].viva = true;

            if(i == 0){

                mapa->info_naves[i][j].numNave = equipo0;

                if(mapa->info_naves[i][j].numNave == 0){

                    mapa->info_naves[i][j].posx = 0;
                    mapa->info_naves[i][j].posy = 0;

                }else if(mapa->info_naves[i][j].numNave == 1){

                    mapa->info_naves[i][j].posx = 0;
                    mapa->info_naves[i][j].posy = 1;

                }else{

                    mapa->info_naves[i][j].posx = 1;
                    mapa->info_naves[i][j].posy = 0;

                }

                equipo0++;

            }else if(i == 1){

                mapa->info_naves[i][j].numNave = equipo1;

                if(mapa->info_naves[i][j].numNave == 0){

                    mapa->info_naves[i][j].posx = 19;
                    mapa->info_naves[i][j].posy = 0;

                }else if(mapa->info_naves[i][j].numNave == 1){

                    mapa->info_naves[i][j].posx = 18;
                    mapa->info_naves[i][j].posy = 0;

                }else{

                    mapa->info_naves[i][j].posx = 19;
                    mapa->info_naves[i][j].posy = 1;
                    
                }

                equipo1++;
                

            }else{

                mapa->info_naves[i][j].numNave = equipo2;

                if(mapa->info_naves[i][j].numNave == 0){

                    mapa->info_naves[i][j].posx = 19;
                    mapa->info_naves[i][j].posy = 19;

                }else if(mapa->info_naves[i][j].numNave == 1){

                    mapa->info_naves[i][j].posx = 19;
                    mapa->info_naves[i][j].posy = 18;

                }else{

                    mapa->info_naves[i][j].posx = 18;
                    mapa->info_naves[i][j].posy = 19;
                    
                }

                equipo2++;
                

            }

        }
    }

}



//------------------------------------------------------------------------------------------



void estado_inicial_casillas(tipo_mapa * mapa){

    int i, j;

    if(mapa == NULL){
        printf("SIMULADOR: No se puede inicializar, mapa vacio.\n");
        return;
    }

    for(i = 0; i < MAPA_MAXX; i++){
        for(j = 0; j < MAPA_MAXY; j++){

            mapa->casillas[i][j].equipo = -1;
            mapa->casillas[i][j].numNave = -1;
            mapa->casillas[i][j].simbolo = SYMB_VACIO;

        }
    }

    // Establecemos las naves donde les corresponde al inicio de la partida en cada casilla - Funcion de mapa.h
    // COORDENADAS: 
    // Equipo 0: (0, 0) - (0, 1) - (1, 0)
    // Equipo 1: (20, 0) - (19, 0) - (20, 1)
    // Equipo 2: (19, 20) - (20, 19) - (20, 20)

    for(i = 0; i < N_EQUIPOS; i++){
        for(j = 0; j < N_NAVES; j++){

            if( mapa_set_nave(mapa, mapa->info_naves[i][j]) == -1){
                printf("No se ha puesto la nave del equipo %d numero %d.\n", i, j);
            }

        }
    }

}



//------------------------------------------------------------------------------------------



void destruir_mapa(tipo_mapa * mapa){

    if(mapa){

        munmap(mapa, sizeof(*mapa));
        shm_unlink(SHM_MAP_NAME);

    }

}



//------------------------------------------------------------------------------------------