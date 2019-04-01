#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define N_READ 4
#define SECS 2


int pid_hijos[N_READ];
int pid_padre;

    void manejador(int sig){
        int i;

        if(getpid()!=pid_padre){
            sleep(3); //LOS HIJOS ESPERAN A QUE EL PADRE LES MANDE LA SENAL SIGTERM
            return;
        }

        printf("\nHe conseguido capturar SIGINT soy el pid = %d\n",getpid());


        fflush(stdout);

        sem_unlink("/SEM_ESCRIT");
        sem_unlink("/SEM_LECT");
        sem_unlink("/SEM_LECTORES");

        for(i=0;i<N_READ;i++){
            kill(pid_hijos[i],SIGTERM);        
        }
        
        for(i=0;i<N_READ;i++){
            wait(NULL);        
        }

        printf("\nHijos liberados, terminando la ejecucion\n");
        
        fflush(stdout);
        
        exit(EXIT_SUCCESS);
    }



    void Lectura(sem_t * sem_lectura, sem_t * sem_escritura, sem_t * sem_lectores) {

        int sval;

        sem_wait(sem_lectura); // HACEMOS DOWN DE LEER
        
        sem_post(sem_lectores); // HACEMOS +1 EN LECTORES
   
        if (sem_getvalue(sem_lectores, &sval) == -1) {
            perror("sem_getvalue");
            sem_unlink("/SEM_ESCRIT");
            sem_unlink("/SEM_LECT");
            sem_unlink("/SEM_LECTORES");
            exit(EXIT_FAILURE);
        }
             
        
        if (sval == 1){

            sem_wait(sem_escritura);  // HACEMOS DOWN DE ESCRIBIR SI SOMOS EL UNICO LECTOR

        }
        
        sem_post(sem_lectura); // HACEMOS UP DE LEER
        
        /*--------LEER------------*/
        printf("\nR-INI %d",getpid());
        sleep(1);
        printf("\nR-FIN %d\n",getpid());
        fflush(stdout);
        /*--------FIN LEER------------*/


        sem_wait(sem_lectura); // HACEMOS DOWN DE LEER

        sem_wait(sem_lectores); // HACEMOS -1 LECTORES
        
        if (sem_getvalue(sem_lectores, &sval) == -1) {
            perror("sem_getvalue");
            sem_unlink("/SEM_ESCRIT");
            sem_unlink("/SEM_LECT");
            sem_unlink("/SEM_LECTORES");
            exit(EXIT_FAILURE);
        }
            

        if (sval == 0){
            sem_post(sem_escritura); // HACEMOS UP DE ESCRIBIR SI NO QUEDAN LECTORES
        }

        sem_post(sem_lectura); // HACEMOS UP DE LEER    

    }


    void Escritura(sem_t * sem_escritura) {


        sem_wait(sem_escritura); // HACEMOS DOWN DE ESCRIBIR


        /*--------ESCRIBIR------------*/
        printf("\nW-INI %d",getpid());
        sleep(1);
        printf("\nW-FIN %d\n",getpid());
        fflush(stdout);

        /*--------FIN ESCRIBIR-----------*/

        sem_post(sem_escritura); // HACEMOS UP DE ESCRIBIR

        
    }

    int main(){

        int i,pid;
        sem_t * sem_lectura=NULL;
        sem_t * sem_escritura=NULL;
        sem_t * sem_lectores=NULL;

        //Por si hay problemas de que el semaforo ya existe
        //sem_unlink("/SEM_ESCRIT");
        //sem_unlink("/SEM_LECT");
        //sem_unlink("/SEM_LECTORES");

        pid_padre=getpid();


        printf("\nSoy el padre con pid = %d\n",pid_padre);

        if ((sem_lectura = sem_open("/SEM_LECT", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }

        if ((sem_escritura = sem_open("/SEM_ESCRIT", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }

        if ((sem_lectores = sem_open("/SEM_LECTORES", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }



        struct sigaction act;
        act.sa_handler = manejador;
        sigemptyset(&(act.sa_mask));
        act.sa_flags =0;

        if(sigaction(SIGINT,&act,NULL)<0){
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        for(i=0;i<N_READ;i++){

            pid=fork();
            if(pid==0){
                break;
            }
            else{
                pid_hijos[i]=pid;
            }
        }

        if(pid==0){
            //hijos lectores
                        
            while(1){
                Lectura(sem_lectura,sem_escritura,sem_lectores);
                sleep(SECS);
                
            }


        }
        else{
            //padre escritor
            while(1){
                Escritura(sem_escritura);
                sleep(SECS);

            }
        }


        return 0;

}