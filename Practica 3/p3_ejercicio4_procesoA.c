
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
    El primer proceso A abre con mmap un fichero cuyo nombre recibe como primer argumento de
    main y escribe en la primera cola de mensajes, cuyo nombre recibe como segundo argumento
    trozos del fichero de longitud máxima 2KB
*/

typedef struct { 
	char text[2048];
} Mensaje;

int main(int argc, char ** argv){

    char nombre_fichero[256];
    char nombre_mq[256];
    int fd_shm, error;
    Mensaje * msg;
    mqd_t queueWRITE;

    printf("He entrado en el proceso A.\n"); // Mensaje de debug

    if(argc != 3){
        printf("Error en los argumentos, debería haber recibido -nombre_fichero -nombre_mq.\n");
        return EXIT_FAILURE;
    }
    
    strcpy(nombre_fichero, argv[1]);
    strcpy(nombre_mq, argv[2]);

    fd_shm = shm_open(nombre_fichero, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);  // Abrimos la shared memory

	if(fd_shm == -1) {
		printf("Error en shm_open.\n");
		return EXIT_FAILURE;
    }

    error = ftruncate(fd_shm, sizeof(Mensaje));  // Acotamos la memoria de la shared memory

	if(error == -1) {
		printf("Error en ftruncate.\n");
		shm_unlink(nombre_fichero);
		return EXIT_FAILURE;
    }

    msg = (Mensaje*)mmap(NULL, sizeof(*msg), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0); // Creamos el mensaje dentro de la shared memory

    if(msg == MAP_FAILED) {
		fprintf (stderr, "Error mapeando la memoria para el mensaje.\n");
		shm_unlink(nombre_fichero);
		return EXIT_FAILURE;
	}

    queueWRITE = mq_open(nombre_mq, O_WRONLY); // Abrimos la cola unicamente para escribir

    if(queueWRITE == (mqd_t)-1) {
		fprintf (stderr, "Error abriendo cola de mensajes de escritura en A.\n"); 
		return EXIT_FAILURE;
	}

    strcpy(msg->text, "Mensaje mandado desde A.");   // Guardamos en el mensaje un texto 

    if(mq_send(queueWRITE, (char *)&msg, sizeof(msg), 1) == -1) {   // Mandamos el mensaje a la cola de escritura
		fprintf (stderr, "Error mandando el mensaje en A.\n");
		return EXIT_FAILURE; 
	}

    mq_close(queueWRITE);   // Cerramos la cola de escritura
    return EXIT_SUCCESS;


}

