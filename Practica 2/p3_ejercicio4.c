#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#define N_PROC 5

/* Manejador_SIGUSR1 */
void manejador_SIGUSR1(int sig){
    printf("Señal SIGUSR1 mandada\n");
    fflush(stdout);
}

int main(void){

    int pidGestor, pidRunner, i, j;
    struct sigaction act;
    act.sa_handler = manejador_SIGUSR1;

    pidGestor = fork();                      // Proceso padre genera proceso hijo que sera el gestor

    if(pidGestor < 0){                       // Si hay error en el fork() se para la ejecucion

        printf("Error en fork().\n");
        exit(EXIT_FAILURE);

    }else if(pidGestor == 0){                // Si estamos en el hijo del fork() hacemos de gestor

        for(i = 0; i < N_PROC; i++){

            pidRunner = fork();

            if(pidRunner < 0){

                printf("Error en fork().\n");
                exit(EXIT_FAILURE); 

            }else if(pidRunner == 0){

                /* Se arma la señal SIGUSR1. */    
                if(sigaction(SIGINT,&act,NULL)<0){
                    perror("sigaction");
                    exit(EXIT_FAILURE);
                }

            }else{

                for(j = 0; j < N_PROC; j++){
                    pause();                 // El hijo espera hasta recibir N * SIGUSR1 de los N nietos         
                }

            }

        }


    }else{




    }
    
}

