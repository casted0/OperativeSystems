#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define SEM "/sem_p3_ejer2" 
#define SHM_NAME "/mq_p3_ejer2"
#define NAME_MAX 50	

int num_hijos;

void manejador(int sig){
	int i;

    printf("Soy %d, el padre y he recibido la senal SIGUSR1 \n",getpid());
    fflush(stdout);

    for(i=0;i<num_hijos;i++){
    	wait(NULL);
    }

    printf("Todos mis hijos han finalizado, terminando p3_ejercicio2.c \n");
    fflush(stdout);

    exit(EXIT_SUCCESS);
}


typedef struct{
	int previous_id; //!< Id of the previous client. Inicializar a -1
	int id; //!< Id of the current client.  Inicializar a 0
	char name[NAME_MAX]; //!< Name of the client.
} ClientInfo;


int main(int argc, char *argv[]){

	int i,num,fd_shm,error;
	pid_t pid;
	ClientInfo * clientePrueba;

	if(!atoi(argv[1])||argc!=2){
		printf("\nPor favor introduzca como parametro de entrada un numero, n>1\n");
		return -1;
	}

	num_hijos=atoi(argv[1]);

	//--------------MEMORY-----------------

	/* We create the shared memory */
	fd_shm = shm_open(SHM_NAME,
		O_RDWR | O_CREAT | O_EXCL, /* Create it and open for reading and writing */ 
		S_IRUSR | S_IWUSR); /* The current user can read and write */

	if(fd_shm == -1) {
		fprintf (stderr, "Error creating the shared memory segment \n");
		return EXIT_FAILURE;
	}
    
    /* Resize the memory segment */
	error = ftruncate(fd_shm, sizeof(ClientInfo));

	if(error == -1) {
		fprintf (stderr, "Error resizing the shared memory segment \n");
		shm_unlink(SHM_NAME);
		return EXIT_FAILURE;
	}
    
	/* Map the memory segment */
	clientePrueba = (ClientInfo *)mmap(NULL, sizeof(*clientePrueba), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);

	if(clientePrueba == MAP_FAILED) {
		fprintf (stderr, "Error mapping the shared memory segment \n");
		shm_unlink(SHM_NAME);
		return EXIT_FAILURE;
	}
	printf("\nPointer to shared memory segment: %p\n", (void*)clientePrueba);

	clientePrueba->id=0;
	clientePrueba->previous_id=-1;
	clientePrueba->name[0]=0;

	//--------------FIN MEMORY-----------------


	//--------------CARGAR SENIAL-----------------

	struct sigaction act;
    act.sa_handler = manejador;
    sigemptyset(&(act.sa_mask));
	act.sa_flags =0;

	if(sigaction(SIGUSR1,&act,NULL)<0){
	    perror("sigaction");
        exit(EXIT_FAILURE);
    }


    //--------------FIN CARGAR SENIAL-----------------


    //--------------CREACION DE HIJOS-----------------

    printf("Soy el padre con pid = %d, voy a crear %d hijos\n",getpid(),atoi(argv[1]));

    for(i=0;i<atoi(argv[1]);i++){

    	pid=fork();

    	if(!pid){
    		break;
    	}

    }

    //--------------FIN CREACION DE HIJOS-----------------

    if(!pid){ //HIJOS

    	srand(getpid());

    	num=1+(rand()%10); //CALCULANDO NUMERO ALEATORIO ENTRE 1 Y 10

    	sleep(num); //DURMIENDO 

    	
		clientePrueba->previous_id++;   //INCREMENTA EN 1 EL ID PREVIO
		printf("Por favor introduzca nombre para que le demos de alta:\n");  
		scanf("%s",clientePrueba->name);  //GUARDA EL NUEVO NOMBRE 
		clientePrueba->id++;  //INCREMENTA EN 1 EL ID DEL CLIENTE 
		


    	kill(getppid(),SIGUSR1);  //MANDA SIGUSR1 AL PADRE Y TERMINA

    	return 0;

    }
    else{     //PADRE

    	pause();   //ESPERANDO A RECIBIR SIGUSR1 DE ALGUN HIJO

    }

	return 0;
}