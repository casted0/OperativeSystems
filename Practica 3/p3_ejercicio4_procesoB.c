
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
    El proceso intermedio B lee de la cola de mensajes cada trozo del fichero y realiza una
    simple función de conversión, consistente en reemplazar los caracteres en el rango a-z
    por su siguiente en el abecedario, convirtiendo la "z" en "a". Una vez realizada esta
    transformación, escribe el contenido en la segunda cola de mensajes. Este programa
    recibe como argumentos de main los nombres de ambas colas.
*/

typedef struct { 
	char text[2048];
} Mensaje;

int main(int argc, char ** argv){

    int i;
    char nombre_mq0[256];
    char nombre_mq1[256];
    mqd_t queueWRITE, queueREAD;
    Mensaje msg;

    printf("He entrado en el proceso B.\n"); // Mensaje de debug

    if(argc != 3){
        printf("Error en los argumentos, deberia haber recibido -nombre_mq0 -nombre_mq1");
        return EXIT_FAILURE;
    }

    strcpy(nombre_mq0, argv[1]);
    strcpy(nombre_mq1, argv[2]);

    queueWRITE = mq_open(nombre_mq0, O_WRONLY); // Abrimos la cola 0 unicamente para escribir

    if(queueWRITE == (mqd_t)-1) {
		fprintf (stderr, "Error creando cola de mensajes de lectura.\n"); 
		return EXIT_FAILURE;
	}

    queueREAD = mq_open(nombre_mq1, O_RDONLY); // Abrimos la cola 1 unicamente para leer

    if(queueREAD == (mqd_t)-1) {
		fprintf (stderr, "Error creando cola de mensajes de lectura.\n"); 
		return EXIT_FAILURE;
	}

    if(mq_receive(queueREAD, (char *)&msg, sizeof(msg), NULL) == -1) {  // Recivimos el mensaje y si no da error lo procesamos como se pide.
		fprintf (stderr, "Error recibiendo el mensaje en B.\n");
		return EXIT_FAILURE;
	}

    for(i = 0; i < strlen(msg.text); i++){
        msg.text[i] = msg.text[i] + 1;
    }

    if(mq_send(queueWRITE, (char *)&msg, sizeof(msg), 1) == -1) {   // Mandamos el mensaje a la cola de escritura
		fprintf (stderr, "Error mandando el mensaje desde B.\n");
		return EXIT_FAILURE; 
	}

    mq_close(queueREAD);    // Cerramos la cola de lectura
    mq_close(queueWRITE);   // Cerramos la cola de escritura
    return EXIT_SUCCESS;


}