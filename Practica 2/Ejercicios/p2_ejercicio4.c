#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define N_PROC 3

/* Manejador_SIGUSR1 */
void manejador_SIGUSR2(int sig){
    printf("\nSeñal SIGUSR2 recibida por padre con PID padre = %d\n",getpid());
    fflush(stdout);
}


void manejador_SIGUSR1_gestor(int sig){

    printf("\nSeñal SIGUSR1 recibida por gestor con PID del gestor = %d\n",getpid());
    fflush(stdout);

}

void manejador_SIGUSR1_gestor_terminar(int sig){
    int i;

    printf("\nSeñal SIGUSR1 recibida por gestor con PID del gestor = %d\n",getpid());
    fflush(stdout);

    for(i=0;i<N_PROC;i++){
        wait(NULL);
    }

    exit(EXIT_SUCCESS);
}

void manejador_SIGUSR1_runner(int sig){
    printf("\nSeñal SIGUSR1 recibida por runner con PID del runner = %d\n",getpid());
    fflush(stdout);
    exit(EXIT_SUCCESS);
}


int main(void){

    int pidPadre,pidGestor, pidRunner[N_PROC], i;

    int pipe_status,fd[2];  //tuberia

    struct sigaction act;
    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

    act.sa_handler = manejador_SIGUSR2;
    if(sigaction(SIGUSR2,&act,NULL)<0){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    pipe_status=pipe(fd);

    if(pipe_status == -1) {
        perror("Error creando la tuberia\n");
        exit(EXIT_FAILURE);
    }

    
    

    pidPadre=getpid();

    printf("\nSoy el padre con PID = %d\n",getpid());

    pidGestor = fork();                      // Proceso padre genera proceso hijo que sera el gestor

    if(pidGestor < 0){                       // Si hay error en el fork() se para la ejecucion

        printf("Error en fork().\n");
        exit(EXIT_FAILURE);

    }else if(pidGestor == 0){    // Si estamos en el hijo del fork() hacemos de gestor

        close(fd[0]);

        pidGestor=getpid();

        printf("Soy el gestor con PID = %d\n",getpid());

        act.sa_handler = manejador_SIGUSR1_gestor;
        if(sigaction(SIGUSR1,&act,NULL)<0){
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        for(i = 0; i < N_PROC; i++){

            pidRunner[i] = fork();

            if(pidRunner[i] < 0){

                printf("Error en fork().\n");
                exit(EXIT_FAILURE); 

            }else if(pidRunner[i] == 0){


                //ARMAR LA SENAL
                act.sa_handler = manejador_SIGUSR1_runner;
                if(sigaction(SIGUSR1,&act,NULL)<0){
                    perror("sigaction");
                    exit(EXIT_FAILURE);
                }


                /* Se envia SIGUSR1 AL GESTOR*/
                kill (pidGestor, SIGUSR1);   

                pause(); //ESPERA A RECIBIR SIGUSR1 DEL PADRE

            }else{

                pause();                 // El manejador espera hasta recibir SIGUSR1 del runner                         
            }

        }

        
        act.sa_handler = manejador_SIGUSR1_gestor_terminar;
        if(sigaction(SIGUSR1,&act,NULL)<0){
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        
        //ENTONCES MANDA SIGUSR2 AL PADRE
        kill (pidPadre, SIGUSR2);

        write(fd[1], pidRunner, sizeof(pidRunner));//manda el array de pid de los runners

        pause();//ESPERA A RECIBIR SIGUSR1 DEL PADRE


    }else{
        //PADRE

        close(fd[1]);

        pause(); //ESPERA A RECIBIR SIGUSR2 DEL GESTOR

        read(fd[0], pidRunner, sizeof(pidRunner)); //lee el array de pid de los runners
        
        kill (pidGestor, SIGUSR1);  //MANDA SIGUSR1 A SU HIJO GESTOR Y A LOS CORREDORES

        for(i=0;i<N_PROC;i++){
            kill(pidRunner[i],SIGUSR1);
        }


        wait(NULL);

        return 0;
    }
    
}