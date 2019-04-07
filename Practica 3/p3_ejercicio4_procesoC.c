
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

/*
    El último proceso lee de la cola el trozo de memoria y lo vuelca por pantalla.
    Recibe como único parámetro de main el nombre de la segunda cola.
*/

typedef struct { 
	char text[2048];
} Mensaje;

int main(int argc, char ** argv){

    char nombre_mq[256];
    mqd_t queueREAD;
    Mensaje * msg;

    printf("He entrado en el proceso C.\n"); // Mensaje de debug

    if(argc != 2){
        printf("Error en los argumentos, debería haber recibido -nombre_mq.\n");
        return EXIT_FAILURE;
    }

    strcpy(nombre_mq, argv[1]);

    queueREAD = mq_open(nombre_mq, O_RDONLY); // Abrimos la cola unicamente para leer

    if(queueREAD == (mqd_t)-1) {
		fprintf (stderr, "Error abriendo cola de mensajes de lectura en C.\n"); 
		return EXIT_FAILURE;
	}

    if(mq_receive(queueREAD, (char *)&msg, sizeof(msg), NULL) == -1) {  // Recivimos el mensaje y si no da error lo imprimimos
		fprintf (stderr, "Error recibiendo el mensaje en C.\n");
		return EXIT_FAILURE;
	}

    printf("Mensaje leido: %s", msg.text);

    mq_close(queueREAD);   // Cerramos la cola de lectura
    return EXIT_SUCCESS;

}