#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

/*
    Crea 4 procesos hijo, de forma paralela, cada uno duerme 30 segundos
*/

#define NUM_PROC 4

int main(void) {

    int i, pid;

    for(i = 0; i < NUM_PROC; i++){
        pid = fork();
        if(pid != 0){

            sleep(5);
            kill (pid, SIGTERM);

        }else if(pid == 0){

            //sleep(30);
            printf("Soy el proceso hijo %d y ya me toca terminar.\n", getpid());
            exit(EXIT_SUCCESS);

        }else{

            printf("Error haciendo fork().\n");
            exit(EXIT_FAILURE);

        }
    }

    exit(EXIT_SUCCESS);
}