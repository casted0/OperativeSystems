/* wait and return process info */
#include <sys/types.h>
#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main ( void )
{
	pid_t pid;
	char * sentence = (char *)malloc(5 * sizeof (char));
	pid = fork();
	if (pid <  0 )
	{
		printf("Error al emplear fork\n");
		exit (EXIT_FAILURE);
	}
	else if(pid ==  0)
	{
		strcpy(sentence, "hola");
		free(sentence);
		exit(EXIT_SUCCESS);
	}
	else
	{
		wait(NULL);
		/* printf("Padre: %s\n", sentence);  ESTA LINEA ES INCORRECTA, GENERA UN ERROR DE MEMORIA POR LEER ALGO VACIO */ 
		free(sentence);
		exit(EXIT_SUCCESS);
	}
}


/*

a) Lo que ocurre es que por pantalla se nos muestra "Padre: " y nada mas, la 'sentence' que queremos ver no aparece, esto es debido a que
   como el enunciado dice, creamos la memoria en el padre, la inicializamos en el hijo con strcpy y intentamos imprimir en el padre de nuevo, 
   Esto es debido a que el proceso padre he hijo aun estando relacionados no son el mismo programa por tanto el padre no tiene acceso a los bytes de 
   memoria donde escribe el hijo, en teoria al ejecutar fork, el proceso hijo copia exactamente todo lo que tiene el padre, pero eso no significa que 
   compartan las direcciones de memoria si no su contenido.


b) Es necesario liberarlo en ambos procesos, pues, una vez que ocurre el fork, padre e hijo son dos procesos completamente distintos con su propia memoria





*/