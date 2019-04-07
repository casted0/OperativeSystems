
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#define N_PROC 3

typedef struct { 
	char text[2048];
} Mensaje;

int main(int argc, char ** argv){

    char nombre_fichero[256];
    char nombre_mq_0[256];
    char nombre_mq_1[256];
    int i, pid;
    int pids_hijos[N_PROC];
    mqd_t queue0, queue1;
    struct mq_attr attributes;
	Mensaje msg;

    // Argumentos para los execv

    char * args_procA[] = {argv[1], argv[2]}; // Proceso A nombre del fichero a crear y nombre de la cola de escritura
    char * args_procB[] = {argv[2], argv[3]}; // Proceso B nombre de la cola de escritura y lectura
    char * args_procC[] = {argv[3]}; // Proceso C nombre de la cola de lectura

    if(argc != 4){

        printf("Error, deben pasarse 3 argumentos: -nombre_fichero -nombre_mq_0 -nombre_mq_1.\n");
        return EXIT_FAILURE;

    }

    strcpy(nombre_fichero, argv[1]);
    strcpy(nombre_mq_0, argv[2]);
    strcpy(nombre_mq_1, argv[3]);

    printf("Creando colas de mensajes.\n");

    attributes.mq_flags = 0;
	attributes.mq_maxmsg = 10;
	attributes.mq_curmsgs = 0;
	attributes.mq_msgsize = sizeof(Mensaje);

    queue0 = mq_open(nombre_mq_0,
				O_CREAT | O_WRONLY, /* This process is only going to send messages */
				S_IRUSR | S_IWUSR, /* The user can read and write */
				&attributes); 

    queue1 = mq_open(nombre_mq_1,
				O_CREAT | O_RDONLY, /* This process is only going to send messages */
				S_IRUSR | S_IWUSR, /* The user can read and write */
				&attributes); 

	if(queue0 == (mqd_t)-1) {
		fprintf (stderr, "Error creando cola de mensajes 0.\n"); 
		return EXIT_FAILURE;
	}

	if(queue1 == (mqd_t)-1) {
		fprintf (stderr, "Error creando cola de mensajes 1.\n"); 
		return EXIT_FAILURE;
	}

    for(i = 0; i < N_PROC; i++){

        pid = fork();

        if(pid < 0){

            printf("Error creando proceso hijo.\n");
            return EXIT_FAILURE;

        }else if(pid == 0){

            if(i == 0){
                printf("Ejecutando proceso A.\n");
                execv("p3_ejercicio4_procesoA", args_procA);
            else if(i == 1){
                printf("Ejecutando proceso B.\n");
                execv("p3_ejercicio4_procesoB", args_procB);
            else{
                printf("Ejecutando proceso C.\n");
                execv("p3_ejercicio4_procesoC", args_procC);
            }

            break; // En caso de que se cambie N_PROC no estaría contemplado.

        }

        pids_hijos[i] = pid;

    }

    if(pid){

        printf("Soy el proceso padre, en cuanto acaben los hijos cerraré las colas de mensajes.\n");
        for(i = 0; i < N_PROC; i++){
            wait(NULL);
        }
        mq_close(queue0);
        mq_close(queue1);
	    mq_unlink(nombre_mq_0);
        mq_unlink(nombre_mq_1);
        return EXIT_SUCCESS;

    }


}