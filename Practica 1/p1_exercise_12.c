#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#define NUM_HILOS 10

typedef struct {
	int entrada;
	int salida;
}Parametros;

void * potencia (void *arg) {
   Parametros * parametro = arg;
   int i,resultado=1;

   for ( i = 0 ; i < parametro->entrada ; i++ ) {
    	resultado = resultado * 2;
   }

   if(parametro->entrada == 0)
   {
      parametro->salida=1;
   }else{
      parametro->salida=resultado;
   }

   pthread_exit(NULL);
}

int main(int argc , char *argv[]) {
   pthread_t thread[NUM_HILOS];
   Parametros * parametro;
   int i;

   parametro=(Parametros*)malloc(NUM_HILOS*sizeof(Parametros));

   for(i=0;i<NUM_HILOS;i++){
   		parametro[i].entrada=i;
   }
   
   for(i=0;i<NUM_HILOS;i++){
   		pthread_create(&thread[i], NULL , potencia , (void *)&parametro[i]);
   }

   i--;

   for(;i>=0;i--){
   		pthread_join(thread[i],NULL);
   }

   for(i=0;i<NUM_HILOS;i++){
   		printf("\nEl hilo %d ha calculado que 2 elevado a %d es = %d\n",i,i,parametro[i].salida);

   }

   printf("El programa %s termino correctamente \n", argv[0]);

   free(parametro);
   exit(EXIT_SUCCESS);
} 
