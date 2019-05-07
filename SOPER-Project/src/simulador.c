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

// Función utilizada para reservar memoria para 1 nave e inicializar sus recursos.

tipo_nave * iniciar_nave(int equipo){

    tipo_nave * nave_aux = NULL;

    nave_aux = (tipo_nave*)malloc(sizeof(nave_aux));

    if(nave_aux == NULL){
        printf("Error iniciando una nave, Finalizando el programa.\n");
        return NULL;
    }

    // Iniciamos los recursos con los MACROS definidos en 'simulador.h'

    nave_aux->vida = VIDA_MAX;
    nave_aux->posx = 0;
    nave_aux->posy = 0;
    nave_aux->viva = true;

    if(equipo == 1){
        nave_aux->equipo = 1;
        nave_aux->numNave = equipo1;
        equipo1++;
    }else if(equipo == 2){
        nave_aux->equipo = 2;
        nave_aux->numNave = equipo2;
        equipo2++;
    }else{
        nave_aux->equipo = 3;
        nave_aux->numNave = equipo3;
        equipo3++;
    }

    return nave_aux;   
}

// Función utilizada para reservar memoria para 1 casilla e inicializar sus recursos.

tipo_casilla * iniciar_casilla(){

    tipo_casilla * casilla_aux = NULL;

    casilla_aux = (tipo_nave*)malloc(sizeof(casilla_aux));

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

tipo_mapa * iniciar_mapa(){

    tipo_mapa * mapa_aux = NULL;
    int i, j;

    mapa_aux = (tipo_nave*)malloc(sizeof(mapa_aux));

    if(mapa_aux == NULL){
        printf("Error iniciando una casilla, Finalizando el programa.\n");
        return NULL;
    }

    // Iniciar la memoria de todo el mapa (Casillas y naves)

    for(i = 0; i < N_EQUIPOS; i++){
        for(j = 0; j < N_NAVES; j++){
            mapa_aux->info_naves[i][j] = *(iniciar_nave(i));
            if(&(mapa_aux->info_naves[i][j]) == NULL){
                printf("Funcion iniciar_nave ERROR.\n");
                return NULL;
            }
        }
    }

    for(i = 0; i < MAPA_MAXX; i++){
        for(j = 0; j < MAPA_MAXY; j++){
            mapa_aux->casillas[i][j] = *(iniciar_casilla());
        }
    }

    mapa_aux->num_naves[0] = equipo1;
    mapa_aux->num_naves[1] = equipo2;
    mapa_aux->num_naves[2] = equipo3;

    return mapa_aux;   
}


void main() {

    tipo_mapa * Mapa_partida = NULL;

	printf("Simulador iniciado...\n");

    Mapa_partida = iniciar_mapa();

    if(Mapa_partida == NULL){
        printf("No se ha iniciado la partida correctamente\n");
    }

    printf("El mapa y las naves han sido creados correctamente.\n Es probable que las posiciones iniciales de las naves esten mal.\n");

    exit(EXIT_SUCCESS);

}



