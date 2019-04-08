#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

#include "ColaPC.h"

struct _ColaPC{

    int head,tail;
    char colaCircular[TAM_COLA];
};


ColaPC * newColaPC(){

	ColaPC * nuevaCola=NULL;

	nuevaCola = (ColaPC*)malloc(sizeof(ColaPC));

	if(!nuevaCola){
		return NULL;
	}

	nuevaCola->head=0;
	nuevaCola->tail=0;

	return nuevaCola;

}


void destroyColaPC(ColaPC * cola){

	if(!cola){
		return;
	}

	free(cola);

	return;
}


Status pushColaPC(ColaPC * cola, char x){

	if(!cola||!x){
		return ERROR;
	}

	if(colaIsFull(cola)==TRUE){
		return ERROR;
	}


	cola->colaCircular[cola->tail]=x;

	cola->tail=(cola->tail+1)%TAM_COLA;	

	return OK;

}

char popColaPC(ColaPC * cola){

	char x;

	if(!cola){
		return NULL;
	}

	if(colaIsEmpty(cola)==TRUE){
		return NULL;
	}

	x=cola->colaCircular[(cola->head)]

	cola->head=(cola->head+1)%TAM_COLA;

	return x;

}

Bool colaIsFull(ColaPC * cola){

	if(cola->head==((cola->tail)+1)%TAM_COLA){
		return TRUE;
	}
	else{
		return FALSE;
	}

}

Bool colaIsEmty(ColaPC * cola){

	if(cola->head==cola->tail){
		return TRUE;
	}
	else{
		return FALSE;
	}
	
}