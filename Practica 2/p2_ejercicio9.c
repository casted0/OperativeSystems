#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define N_PROC 3
#define SEM "/sem_fichero"

void escribir(FILE * f, int id){
    fprintf(f, "%d\n", id);
    usleep(1);
}

int main(void){

    FILE * fwr = fopen("ejercicio9.txt", "w");
    FILE * frd = fopen("ejercicio9.txt", "r");
    int pid, i, j, id_hijo, count[N_PROC], leido;
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

        }

    }

    if(pid == 0){

        while(1){

            sem_wait(sem);
            escribir(fwr, id_hijo);
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
                fclose(fwr);
                fclose(frd);
                sem_close(sem);
                sem_unlink(SEM); 
                exit(EXIT_SUCCESS);
            }
            sem_post(sem);
        
        }


    }

}