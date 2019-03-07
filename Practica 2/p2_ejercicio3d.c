#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

/* manejador: rutina de tratamiento de la señal SIGINT. */
void manejador(int sig){
    printf("He conseguido capturar SIGKILL \n");
    fflush(stdout);
    exit(EXIT_SUCCESS);
}


int main(void){

    struct sigaction act;
    act.sa_handler = manejador;
    sigemptyset(&(act.sa_mask));
    act.sa_flags =0;
    
    if(sigaction(SIGKILL,NULL,NULL)<0){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
        
    while(1){
        printf("En espera de SIGKILL (PID = %d)\n", getpid());
        sleep(9999);
    }
}


/* PREGUNTAS TEÓRICAS

    a) ¿La llamada a sigaction supone que se ejecute la función manejador? :
            Si pues, el handler del sigaction act, se ha establecido en la funcion manejador
    b) ¿Cuándo aparece el printf en pantalla? : 
            Aparece inmediatamente a continuac´ión de haber pulsaco Ctrl+C
    c) ¿Qué ocurre por defecto cuando un programa recibe una señal y no la tiene capturada? :
            Se llama al manejador por defecto, en este programa si comentamos la linea que define que el handler sea nuestro manejador
            Ctrl+C hará su tarea predeterminada, SIGKILL (Interrupt).
    d) ¿Por qué nuncasale por pantalla "He conseguido capturar SIGKILL"? : 
            Porque SIGKILL es una señal reservada para SO, para que este mate procesos.

*/