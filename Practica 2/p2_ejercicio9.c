#include <stdio.h>
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

void escribir(FILE * f, int id){
    fprintf(f, "%d\n", id);
}

int main(void){

    FILE * fwr = fopen("ejercicio9.txt", "w");
    FILE * frd = fopen("ejercicio9.txt", "r");
    int pid, i, j, id_hijo, count[N_PROC], leido, pids[N_PROC];
    int condicion = 0;
    sem_t *sem = NULL;

    if((sem = sem_open(SEM , O_CREAT | O_EXCL, S_IRUSR | S_IWUSR , 1))== SEM_FAILED){
        perror("sem_open");
        exit(EXIT_FAILURE);
    }  

    if(!fwr){
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

        while(1){

            sem_wait(sem);
            escribir(fwr, id_hijo);
            usleep(1);
            sem_post(sem);

        }
    
    }else{

        while(1){

            sleep(1);
            sem_wait(sem);
            while(fscanf(frd, "%d", &leido) == 1){
                count[leido] = count[leido] + 1;
            }

            for(j = 0; j < N_PROC; j++){
                if(count[j] >= 20)
                    condicion = 1;
                printf("Proceso %d, ha escrito: %d veces.\n", j, count[j]);
            }

            if(condicion){
                printf("La carrera ha acabado.\n");
                for(j=0;j<N_PROC;j++){
                    kill(pids[j], SIGTERM);
                    wait(NULL);
                }
                fclose(fwr);
                fclose(frd);
                sem_close(sem);
                sem_unlink(SEM);
                exit(EXIT_SUCCESS);
            }else{
                memset(count, 0, N_PROC*sizeof(int));
            }
            sem_post(sem);
        
        }


    }

}