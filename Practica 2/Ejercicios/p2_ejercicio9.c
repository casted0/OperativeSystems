#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define N_PROC 3
#define SEM "/sem_fichero"



void manejador(int sig){
    printf("Soy %d y he recibido la senal SIGTERM \n",getpid());
    fflush(stdout);
    exit(EXIT_SUCCESS);
}

void escribir(int id){
    FILE * f = fopen("ejercicio9.txt", "a");
    fprintf(f, "%d ", id);
    fclose(f);
}

int main(void){



    
    
    int pid, i, j, id_hijo, count[N_PROC], leido, pids[N_PROC],num;
    int condicion = 0;
    sem_t *sem = NULL;

    

    struct sigaction act;
    act.sa_handler = manejador;


    //Por si hay problemas con el semaforo
    //sem_unlink(SEM);

    if((sem = sem_open(SEM , O_CREAT | O_EXCL, S_IRUSR | S_IWUSR , 1))== SEM_FAILED){
        perror("sem_open");
        exit(EXIT_FAILURE);
    }  


    for(i = 0; i < N_PROC; count[i] = 0, i++){

        pid = fork();

        if(pid < 0){
            printf("Error en fork().\n");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){

            id_hijo = i;
            break;

        }else{
            pids[i] = pid;
        }

    }

    if(pid == 0){

        srand(getpid());

        sigemptyset(&(act.sa_mask));
        act.sa_flags = 0;
    
        if(sigaction(SIGTERM,&act,NULL)<0){
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        while(1){

            sem_wait(sem);

            //printf(" {semaforo cerrado por pid %d} ",getpid());
            escribir(id_hijo);
            //printf(" {semaforo abierto por pid %d} ",getpid());
            num=rand();
            num=num%(100000-1);
            //printf("\nValor de num = %d del pid = %d\n", num, getpid());
            sem_post(sem);

            usleep(num);
            

        }
    
    }else{

        while(1){

            sleep(1);
            sem_wait(sem);
            printf("\nSemaforo cerrado por padre\n");

            FILE * f = fopen("ejercicio9.txt", "r");
            while(fscanf(f, "%d", &leido) == 1){
                count[leido] = count[leido] + 1;
            }

            for(j = 0; j < N_PROC; j++){
                if(count[j] >= 20){
                    condicion = 1;
                }
                printf("Proceso %d,con PID = %d ha escrito: %d veces.\n", j,pids[j] ,count[j]);
            }

            if(condicion){
                printf("La carrera ha acabado.\n");
                for(j=0;j<N_PROC;j++){
                    kill(pids[j], SIGTERM);
                    wait(NULL);
                }
                fclose(f);
                
                f=fopen("ejercicio9.txt", "w");
                fprintf(f," ");
                fclose(f);

                printf("Soy el padre y todos mis hijos han acabado\n");

                sem_close(sem);
                sem_unlink(SEM);
                exit(EXIT_SUCCESS);
            }else{
                fclose(f);
                f=fopen("ejercicio9.txt", "w");
                fprintf(f," ");
                fclose(f);

                memset(count, 0, N_PROC*sizeof(int));
            }
            
            sem_post(sem);
        
        }


    }

}