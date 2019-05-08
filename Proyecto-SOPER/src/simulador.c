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
#include <unistd.h>
#include <mapa.h>

// Variables globales usadas para contar el numero de naves

int equipo1 = 0;
int equipo2 = 0;
int equipo3 = 0;


int main() {

    tipo_mapa * mapa = NULL;
    int pid = 1, i;

	printf("Simulador iniciado...\n");

    mapa = iniciar_mapa();

    if(mapa == NULL){
        printf("No se ha iniciado la partida correctamente\n");
        exit(EXIT_FAILURE);
    }

    // INICIAR PROCESOS JEFE, 3 PORQUE HAY 3 EQUIPOS

    for(i = 0; i < N_EQUIPOS; i++){

        if(pid != 0){
            printf("Soy el simulador y estoy creando al jefe numero %d.\n", i);
            pid = fork();
        }else if(pid == -1){
            printf("Error creando procesos jefes.\n");
        }else{
            break;
        }

    }

    // ------ CODIGO DE LOS JEFES Y ESPERA DEL SIMULADOR A QUE ACABEN ------ //

    if(pid == 0){
        usleep(500);
        printf("Soy el jefe numero %d.\n", (i-1));
        sleep(15);
    }else{
        for(i = 0; i < N_EQUIPOS; i++){
            wait(NULL);
        }
    }




    // ------ LIBERAR Y ACABAR ------ //

    destruir_mapa(mapa);
    exit(EXIT_SUCCESS);

}


// ------ FUNCIONES AUXILIARES ------ //


//------------------------------------------------------------------------------------------



tipo_nave * iniciar_nave(int equipo){

    tipo_nave * nave_aux = NULL;

    nave_aux = (tipo_nave*)malloc(sizeof(tipo_nave));

    if(nave_aux == NULL){
        printf("Error iniciando una nave, Finalizando el programa.\n");
        return NULL;
    }

    // Iniciamos los recursos con los MACROS definidos en 'simulador.h'

    nave_aux->vida = VIDA_MAX;
    nave_aux->viva = true;

    // Le damos a cada nave un numero dependiendo de su equipo y el turno en el que ha entrado
    // ademas, dependiendo de este identificador, se le otorga una posicion en el mapa.

    if(equipo == 1){
        nave_aux->equipo = 1;
        nave_aux->numNave = equipo1;
        if(nave_aux->numNave == 0){
            nave_aux->posx = 0;
            nave_aux->posy = 0;
        }else if(nave_aux->numNave == 1){
            nave_aux->posx = 1;
            nave_aux->posy = 0;
        }else{
            nave_aux->posx = 0;
            nave_aux->posy = 1;
        }
        equipo1++;
    }else if(equipo == 2){
        nave_aux->equipo = 2;
        nave_aux->numNave = equipo2;
        if(nave_aux->numNave == 0){
            nave_aux->posx = 20;
            nave_aux->posy = 0;
        }else if(nave_aux->numNave == 1){
            nave_aux->posx = 19;
            nave_aux->posy = 0;
        }else{
            nave_aux->posx = 20;
            nave_aux->posy = 1;
        }
        equipo2++;
    }else{
        nave_aux->equipo = 3;
        nave_aux->numNave = equipo3;
        if(nave_aux->numNave == 0){
            nave_aux->posx = 20;
            nave_aux->posy = 20;
        }else if(nave_aux->numNave == 1){
            nave_aux->posx = 19;
            nave_aux->posy = 20;
        }else{
            nave_aux->posx = 20;
            nave_aux->posy = 19;
        }
        equipo3++;
    }

    return nave_aux;   
}



//------------------------------------------------------------------------------------------



tipo_casilla * iniciar_casilla(){

    tipo_casilla * casilla_aux = NULL;

    casilla_aux = (tipo_casilla*)malloc(sizeof(tipo_casilla));

    if(casilla_aux == NULL){
        printf("Error iniciando una casilla, Finalizando el programa.\n");
        return NULL;
    }

    // Iniciamos los recursos con los MACROS definidos en 'simulador.h'

    casilla_aux->simbolo = SYMB_VACIO;
    casilla_aux->equipo = -1;
    casilla_aux->numNave = -1; // No esta especificado pero supongo que si no hay nave el numero default es -1

    return casilla_aux;   
}



//------------------------------------------------------------------------------------------



tipo_mapa * iniciar_mapa(){

    tipo_mapa * mapa_aux = NULL;
    int i, j;

    mapa_aux = (tipo_mapa*)malloc(sizeof(tipo_mapa));

    if(mapa_aux == NULL){
        printf("Error iniciando una casilla, Finalizando el programa.\n");
        return NULL;
    }

    // Iniciar la memoria de todo el mapa (Casillas y naves)

    mapa_aux->info_naves = (tipo_nave**)malloc(N_EQUIPOS * sizeof(tipo_nave*));

    for(i = 0; i < N_EQUIPOS; i++){

       mapa_aux->info_naves[i] = (tipo_nave*)malloc(N_NAVES * sizeof(tipo_nave)); 

    }

    mapa_aux->casillas = (tipo_casilla**)malloc(MAPA_MAXX * sizeof(tipo_casilla*));

    for(i = 0; i < MAPA_MAXX; i++){

       mapa_aux->casillas[i] = (tipo_casilla*)malloc(MAPA_MAXY * sizeof(tipo_casilla)); 

    }

    mapa_aux->num_naves = (int*)malloc(N_EQUIPOS * sizeof(int));

    // Insertar en la memoria los recursos de las otras funciones de inicializacion


    for(i = 0; i < N_EQUIPOS; i++){
        for(j = 0; j < N_NAVES; j++){
            mapa_aux->info_naves[i][j] = *iniciar_nave(i);
        }
    }

    for(i = 0; i < MAPA_MAXX; i++){
        for(j = 0; j < MAPA_MAXY; j++){
            mapa_aux->casillas[i][j] = *iniciar_casilla();
        }
    }

    mapa_aux->num_naves[0] = equipo1;
    mapa_aux->num_naves[1] = equipo2;
    mapa_aux->num_naves[2] = equipo3;

    return mapa_aux;   
}



//------------------------------------------------------------------------------------------



void destruir_nave(tipo_nave * nave){

    if(nave != NULL){

        free(nave);

    }

}



//------------------------------------------------------------------------------------------



void destruir_casilla(tipo_casilla * casilla){

    if(casilla != NULL){

        free(casilla);

    }

}



//------------------------------------------------------------------------------------------



void destruir_mapa(tipo_mapa * mapa){

    int i;

    if(mapa){

        free(mapa->num_naves);

        for(i = 0; i < N_EQUIPOS; i++){
            destruir_nave(mapa->info_naves[i]);
        }

        free(mapa->info_naves);

        for(i = 0; i < MAPA_MAXX; i++){
            destruir_casilla(mapa->casillas[i]);
        }

        free(mapa->casillas);

        free(mapa);
    }

}



//------------------------------------------------------------------------------------------