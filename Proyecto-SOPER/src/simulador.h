#ifndef SRC_SIMULADOR_H_
#define SRC_SIMULADOR_H_

#include <stdbool.h>

#define N_EQUIPOS 3 // Número de equipos
#define N_NAVES 3 // Número de naves por equipo

/*** SCREEN ***/
extern char symbol_equipos[N_EQUIPOS]; // Símbolos de los diferentes equipos en el mapa (mirar mapa.c)
#define MAPA_MAXX 20 // Número de columnas del mapa
#define MAPA_MAXY 20 // Número de
#define SCREEN_REFRESH 10000 // Frequencia de refresco del mapa en el monitor
#define SYMB_VACIO '.' // Símbolo para casilla vacia
#define SYMB_TOCADO '%' // Símbolo para tocado
#define SYMB_DESTRUIDO 'X' // Símbolo para destruido
#define SYMB_AGUA 'w' // Símbolo para agua

/*** SIMULACION ***/
#define VIDA_MAX 50 // Vida inicial de una nave
#define ATAQUE_ALCANCE 20 // Distancia máxima de un ataque
#define ATAQUE_DANO 10 // Daño de un ataque
#define MOVER_ALCANCE 1 // Máximo de casillas a mover
#define TURNO_SECS 5 // Segundos que dura un turno


/*** MAPA ***/
// Información de nave
typedef struct {
	int vida; // Vida que le queda a la nave
	int posx; // Columna en el mapa
	int posy; // Fila en el mapa
	int equipo; // Equipo de la nave
	int numNave; // Numero de la nave en el equipo
	bool viva; // Si la nave está viva o ha sido destruida
} tipo_nave;

// Información de una casilla en el mapa
typedef struct {
	char simbolo; // Símbolo que se mostrará en la pantalla para esta casilla
	int equipo; // Si está vacia = -1. Si no, número de equipo de la nave que está en la casilla
	int numNave; // Número de nave en el equipo de la nave que está en la casilla
} tipo_casilla;


typedef struct {
	tipo_nave ** info_naves;
	tipo_casilla ** casillas;
	int * num_naves; // Número de naves vivas en un equipo
} tipo_mapa;

#define SHM_MAP_NAME "/shm_naves"

// Funciones auxiliares

tipo_nave * iniciar_nave(int equipo); 			// Funcion que inicia la memoria de una nave
tipo_casilla * iniciar_casilla();     			// Funcion que inicia la memoria de una casilla
tipo_mapa * iniciar_mapa();           			// Funcion que inicia la memoria de un mapa

void destruir_nave(tipo_nave * nave);			// Funcion que libera la memoria de una nave
void destruir_casilla(tipo_casilla * casilla);  // Funcion que libera la memoria de una casilla
void destruir_mapa(tipo_mapa * mapa); 			// Funcion que libera la memoria de un mapa

#endif /* SRC_SIMULADOR_H_ */
