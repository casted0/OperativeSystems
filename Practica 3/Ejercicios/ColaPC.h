
#ifndef COLAPC_H
#define COLAPC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAM_COLA 10  

typedef struct _ColaPC{

    int head,tail;
    char colaCircular[TAM_COLA];
}ColaPC;

typedef enum {
    ERROR=0, OK=1
} Status;

typedef enum {
    FALSE=0, TRUE=1
} Bool;


ColaPC * newColaPC();
void destroyColaPC(ColaPC * cola);
Status pushColaPC(ColaPC * cola, char x);
char popColaPC(ColaPC * cola);
Bool colaIsFull(ColaPC * cola);
Bool colaIsEmpty(ColaPC * cola);

#endif /* COLAPC_H */