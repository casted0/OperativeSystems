/*
 * Ejemplo de codigo que genera un numero aleatorio y lo muestra por pantalla
 */
 #include <sys/types.h>
 #include <sys/wait.h>
 #include <time.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <unistd.h>

int main(int argc, char *argv[]) {
  /* Inicializa el generador con una semilla cualquiera, OJO! este metodo solo
   se llama una vez */
  int pid1,pid2,pipe_status,fd[2],fd2[2];	

  srand(time(NULL));
  /* Devuelve un numero aleatorio en 0 y MAX_RAND(un número alto que varia
   segun el sistema) */
  int numero,numero_leido,numero_hijo2;
  


  pipe_status=pipe(fd);

  if(pipe_status == -1) {
  	perror("Error creando la tuberia\n");
  	exit(EXIT_FAILURE);
  }

  if((pid1 = fork()) == -1) {
 	perror("fork");
 	exit(EXIT_FAILURE);
  }

  if(pid1 == 0) {
  	/* Cierre del descriptor de entrada en el hijo */
  	close(fd[0]);
  	/* Enviar el saludo vía descriptor de salida */
  	numero=rand();
  	printf("\nSoy el hijo 1 y %d es el numero aleatorio generado\n", numero);

  	write(fd[1], &numero, sizeof(int));
  	exit(EXIT_SUCCESS);
  } else {
  	/* Cierre del descriptor de salida en el padre */
  	close(fd[1]);
  	/* Leer algo de la tubería... el saludo! */
  	read(fd[0], &numero_leido, sizeof(int));
  	printf("\nSoy el padre y he recibido el numero: %d de mi hijo 1\n", numero_leido);
  	wait(NULL);
  }

  /*----------------------------------*/


  pipe_status=pipe(fd2);

  if(pipe_status == -1) {
  	perror("Error creando la tuberia\n");
  	exit(EXIT_FAILURE);
  }

  if((pid2 = fork()) == -1) {
 	perror("fork");
 	exit(EXIT_FAILURE);
  }

  if(pid2 == 0) {
  	/* Cierre del descriptor de salida en el hijo */
  	close(fd2[1]);
  	/* Enviar el saludo vía descriptor de salida */

  	read(fd2[0], &numero_hijo2, sizeof(int));

  	printf("\nSoy el hijo 2 y el numero que me ha pasado el padre es = %d\n", numero_hijo2);
  	exit(EXIT_SUCCESS);
  } else {
  	/* Cierre del descriptor de entrada en el padre */
  	close(fd2[0]);
  	/* Enviar el numero leido */
  	write(fd2[1], &numero_leido, sizeof(int));

  	wait(NULL);
  	exit(EXIT_SUCCESS);
  }



  exit(EXIT_SUCCESS);
}
