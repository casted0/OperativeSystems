#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <mapa.h>

tipo_mapa * mapa = NULL;    // Mapa del simulador

// Variables globales usadas para contar el numero de naves

int equipo0 = 0;
int equipo1 = 0;
int equipo2 = 0;

int i, j;                   // Indices

int pid_hijos[3]={0,0,0};
int pid_padre = 1;

/* manejador: rutina de tratamiento de la señal SIGINT. */
void manejador_padre(int sig) {

    if(pid_padre){

        printf("\n\nTerminando la partida por haber recibido Ctrl+C...\n");
        fflush(stdout);


        kill(pid_hijos[0], SIGUSR1);
        kill(pid_hijos[1], SIGUSR1);
        kill(pid_hijos[2], SIGUSR1);
        sleep(1);

        wait(NULL);
        wait(NULL);
        wait(NULL);


        printf("\nTodos los jefes y naves finalizados, cerrando simulacion...\n");
        fflush(stdout);
        sleep(1);

        destruir_mapa(mapa);
        sem_unlink(SEM);
        exit(EXIT_SUCCESS);
    }
    else{
        sleep(20);
    }
}

void manejador_jefe(int sig){

    printf("\nJEFE [%d] ha recibido orden de terminar por Ctrl+C...", (i));
    fflush(stdout);

    
    kill(pid_hijos[0], SIGUSR2);
    kill(pid_hijos[1], SIGUSR2);
    kill(pid_hijos[2], SIGUSR2);

    wait(NULL);
    wait(NULL);
    wait(NULL);

    exit(EXIT_SUCCESS);
}


void manejador_nave(int sig){

    printf("\nNAVE [%d]: Nave ha recibido orden de terminar por Ctrl+C, pertenezco al jefe %d...", (j), (i));
    fflush(stdout);
    exit(EXIT_SUCCESS);

}



int main() {

    
    int pid = 1,pipe_status,fd_jefes[3][2],fd_naves[3][2];  
    int juego_terminado = 0,k,cont,fila,columna,turno=0,n,m; 
    char mensaje[20]; 
    int cas_columna,cas_fila;
    char symbol;
    bool encontrado;

    sem_t * sem_simulador = NULL;
    
    /*Para debuguear*/
    //shm_unlink(SHM_MAP_NAME);
    //sem_unlink(SEM);

    
    struct sigaction act;

    act.sa_handler = manejador_padre;
    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

    /*Recibir la senial Ctrl+C*/
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

	printf("SIMULADOR: Simulador iniciado...\n");

    if ((sem_simulador = sem_open(SEM, O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {

        printf("SIMULADOR: Error creando el semaforo.\n");
        exit(EXIT_FAILURE);

    }

    mapa = iniciar_mapa();

    if(mapa == NULL){
        printf("SIMULADOR: No se ha iniciado la partida correctamente.\n");
        exit(EXIT_FAILURE);
    }

    // FUNCION PARA DISPONER LAS NAVES PARA EL COMIENZO (Y SETTEAR EL RESTO DE LA MEMORIA)

    estado_inicial_mapa(mapa);

    


    // ABRIMOS EL SEMAFORO PARA QUE ENTRE EL MONITOR AHORA QUE HAY MAPA

    sem_post(sem_simulador);
    
    // INICIAR PROCESOS JEFE, 3 PORQUE HAY 3 EQUIPOS

    printf("\nSoy el padre con pid = %d\n",getpid());

    for(i = 0; i < N_EQUIPOS; i++){

        /*Crear la tuberia que conectara con el hijo que se crea debajo*/

        pipe_status=pipe(fd_jefes[i]);

        if(pipe_status == -1) {
            perror("Error creando la tuberia\n");
            exit(EXIT_FAILURE);
        }


        pid_hijos[i] = fork();
        if(pid_hijos[i] == -1){
            printf("SIMULADOR: Error creando procesos jefes.\n");
            fflush(stdout);
            exit(EXIT_FAILURE);
        }
        if(!pid_hijos[i]){

            pid=0;
            pid_padre = 0;
            pid_hijos[0]=0;
            pid_hijos[1]=0;
            pid_hijos[2]=0;

            /*el hijo cierra la tuberia de salida para leer datos del padre*/
            close(fd_jefes[i][1]);
            /*Modelo de lectura: read(fd_jefes[i][0], &numero_leido, sizeof(int));*/

            /*printf("Soy jefe hijo con pid = %d",getpid());*/
            /* iniciar recibir seniales de los jefes*/

            act.sa_handler = manejador_jefe;
            sigemptyset(&(act.sa_mask));
            act.sa_flags = 0;

            /*Recibir la senial SIGUSR1*/
            if (sigaction(SIGUSR1, &act, NULL) < 0) {
                perror("sigaction");
                exit(EXIT_FAILURE);
            }


            break;
        }

        /*el padre cierra la tuberia de entrada para enviar datos al hijo*/
        close(fd_jefes[i][0]);
        /*Modelo de escritura write(fd_jefes[i][1], &numero, sizeof(int));*/
    }

    /* Para controlar en primeras versiones
    if(pid_hijos[3]){

        printf("\nSoy el padre con pid %d, mis hijos tiene pid = %d, %d, %d \n",getpid(),pid_hijos[0],pid_hijos[1],pid_hijos[1]);

    }*/

    // ------ CODIGO DE LOS JEFES Y ESPERA DEL SIMULADOR A QUE ACABEN ------ //

    if(!pid){


        usleep(500);
        printf("\nJEFE [%d]: Jefe creado.\n", (i));
        
        for(j = 0; j < N_NAVES; j++){

            /*Crear la tuberia con la que se conectara el jefe con su nave hija*/

            pipe_status=pipe(fd_naves[j]);

            if(pipe_status == -1) {
                perror("Error creando la tuberia\n");
                exit(EXIT_FAILURE);
            }

            pid_hijos[j]= fork();
            if(pid_hijos[j] == -1){
                printf("\nJEFE [%d]: Error creando procesos nave.\n", (i));
                fflush(stdout);
                exit(EXIT_FAILURE);
            }
            if(!pid_hijos[j]){

                 /*la nave cierra la tuberia de salida para leer datos del jefe*/
                close(fd_naves[j][1]);
                /*Modelo de lectura: read(fd_naves[j][0], &numero_leido, sizeof(int));*/

                pid=0;
                pid_padre = 0;
                pid_hijos[0]=0;
                pid_hijos[1]=0;
                pid_hijos[2]=0;

                /*printf("Soy nave hija con pid = %d",getpid());*/

                /* iniciar recibir seniales de las naves */

                act.sa_handler = manejador_nave;
                sigemptyset(&(act.sa_mask));
                act.sa_flags = 0;

                /*Recibir la senial SIGUSR2*/
                if (sigaction(SIGUSR2, &act, NULL) < 0) {
                    perror("sigaction");
                    exit(EXIT_FAILURE);
                }


                break;
            }
            else{

                /*el jefe cierra la tuberia de entrada para enviar datos a la nave*/
                close(fd_naves[i][0]);
                /*Modelo de escritura write(fd_naves[j][1], &numero, sizeof(int));*/


            }

        }

    }

    /*Para controlar en primeras versiones
    if(!pid && pid_hijos[3]){
        printf("\nSoy jefe con pid %d, mis hijos tiene pid = %d, %d, %d \n",getpid(),pid_hijos[0],pid_hijos[1],pid_hijos[1]);

    }*/
        
    if(!pid_hijos[0]){   /*Aqui solo va a entrar las naves*/

        usleep(500);
        printf("\nNAVE [%d]: Nave creada, pertenezco al jefe %d.\n", (j), (i));
        sleep(2);

        while(!juego_terminado){

            /*Leer el mensaje que le haya mandado el jefe*/
            read(fd_naves[j][0], &mensaje, sizeof(mensaje));

            columna=(mapa->info_naves[i][j].posx);
            fila=(mapa->info_naves[i][j].posy);

            if(!strcmp(mensaje,"FIN")){
                /*Toca finalizar*/
                juego_terminado=1;
            }
            else if(!strcmp(mensaje,"MOVER")){
                /*Toca moverse aleatoriamente a una casilla si se puede*/    

                if((fila+MOVER_ALCANCE)<MAPA_MAXY && mapa_get_symbol(mapa, fila+MOVER_ALCANCE, columna)==SYMB_VACIO){
                    mapa->info_naves[i][j].posy=fila+MOVER_ALCANCE;
                    mapa_set_nave(mapa, mapa->info_naves[i][j]);
                    printf("\nNAVE [%d], pertenezco al jefe %d, me muevo a %d (fila), %d (columna)", (j), (i),fila+MOVER_ALCANCE,columna);
                    mapa_clean_casilla(mapa, fila, columna);
                }

                else if((columna+MOVER_ALCANCE)<MAPA_MAXX && mapa_get_symbol(mapa, fila, columna+MOVER_ALCANCE)==SYMB_VACIO){
                    mapa->info_naves[i][j].posx=columna+MOVER_ALCANCE;
                    mapa_set_nave(mapa, mapa->info_naves[i][j]);
                    printf("\nNAVE [%d], pertenezco al jefe %d, me muevo a %d (fila), %d (columna)", (j), (i),fila,columna+MOVER_ALCANCE);
                    mapa_clean_casilla(mapa, fila, columna);
                }

                else if((fila-MOVER_ALCANCE)>=0 && mapa_get_symbol(mapa, fila-MOVER_ALCANCE, columna)==SYMB_VACIO){
                    mapa->info_naves[i][j].posy=fila-MOVER_ALCANCE;
                    mapa_set_nave(mapa, mapa->info_naves[i][j]);
                    printf("\nNAVE [%d], pertenezco al jefe %d, me muevo a %d (fila), %d (columna)", (j), (i),fila-MOVER_ALCANCE,columna);
                    mapa_clean_casilla(mapa, fila, columna);
                }
                
                else if((columna-MOVER_ALCANCE)>=0 && mapa_get_symbol(mapa, fila, columna-MOVER_ALCANCE)==SYMB_VACIO){
                    mapa->info_naves[i][j].posx=columna-MOVER_ALCANCE;
                    mapa_set_nave(mapa, mapa->info_naves[i][j]);
                    printf("\nNAVE [%d], pertenezco al jefe %d, me muevo a %d (fila), %d (columna)", (j), (i),fila,columna-MOVER_ALCANCE);
                    fflush(stdout);
                    mapa_clean_casilla(mapa, fila, columna);
                }
                else{
                    printf("\nNAVE [%d], pertenezco al jefe %d, no he podido moverme.", (j), (i));
                    printf("\nMis coordenadas son %d y %d.", fila, columna);
                    fflush(stdout);
                }

            }
            else if(!strcmp(mensaje,"ATACAR")){
                /*Toca atacar si se puede a un enemigo*/
                printf("\nNAVE [%d], pertenezco al jefe %d, he recibido orden de atacar.", (j), (i));
                fflush(stdout);

                encontrado=false;
                /*cas_columna,cas_fila*/
                for(cas_columna=columna+1;cas_columna-columna<=ATAQUE_ALCANCE && cas_columna < MAPA_MAXX;cas_columna++){
                    symbol=mapa_get_symbol(mapa, fila, cas_columna);
                    if(symbol!=SYMB_VACIO && symbol!=mapa_get_symbol(mapa, fila, columna)){
                        /*NAVE ENEMIGA ENCONTRADA*/
                        /*ENCONTRAR LA NAVE ENEMIGA QUE ESTA EN ESA POSICION*/
                        /*n,m*/
                        for(m=0;m<3;m++){
                            for(n=0;n<3;n++){
                              if(mapa->info_naves[m][n].posx==cas_columna && mapa->info_naves[m][n].posy==fila) {
                                    encontrado=true;
                                    break;
                              } 

                            }
                            if(encontrado){
                                break;
                            }
                        }

                        break;
                    }

                }
                if(encontrado){
                    mapa->info_naves[m][n].vida=mapa->info_naves[m][n].vida - ATAQUE_DANO;

                    if(mapa->info_naves[m][n].vida<=0){
                        mapa->info_naves[m][n].viva=false;
                        mapa_set_symbol(mapa, fila, cas_columna, SYMB_DESTRUIDO);
                        printf("\nNAVE [%d], pertenezco al jefe %d, he destruido la nave %d del jefe%d", (j), (i),n,m);
                        fflush(stdout);
                        mapa->accion_pendiente[m]=1;
                    }
                    else{
                        mapa_set_symbol(mapa, fila, cas_columna, SYMB_TOCADO);
                        printf("\nNAVE [%d], pertenezco al jefe %d, he daniado la nave %d del jefe%d", (j), (i),n,m);
                        fflush(stdout);
                    }
                    

                }

                else{

                    for(cas_columna=columna-1;columna-cas_columna<=ATAQUE_ALCANCE && cas_columna >= 0;cas_columna--){
                        symbol=mapa_get_symbol(mapa, fila, cas_columna);
                        if(symbol!=SYMB_VACIO && symbol!=mapa_get_symbol(mapa, fila, columna)){
                            /*NAVE ENEMIGA ENCONTRADA*/
                            /*ENCONTRAR LA NAVE ENEMIGA QUE ESTA EN ESA POSICION*/
                            /*n,m*/
                            for(m=0;m<3;m++){
                                for(n=0;n<3;n++){
                                  if(mapa->info_naves[m][n].posx==cas_columna && mapa->info_naves[m][n].posy==fila) {
                                        encontrado=true;
                                        break;
                                  } 

                                }
                                if(encontrado){
                                    break;
                                }
                            }

                            break;
                        }

                    }
                    if(encontrado){
                        mapa->info_naves[m][n].vida=mapa->info_naves[m][n].vida - ATAQUE_DANO;

                        if(mapa->info_naves[m][n].vida<=0){
                            mapa->info_naves[m][n].viva=false;
                            mapa_set_symbol(mapa, fila, cas_columna, SYMB_DESTRUIDO);
                            printf("\nNAVE [%d], pertenezco al jefe %d, he destruido la nave %d del jefe%d", (j), (i),n,m);
                            fflush(stdout);
                            mapa->accion_pendiente[m]=1;
                        }
                        else{
                            mapa_set_symbol(mapa, fila, cas_columna, SYMB_TOCADO);
                            printf("\nNAVE [%d], pertenezco al jefe %d, he daniado la nave %d del jefe%d", (j), (i),n,m);
                            fflush(stdout);
                        }
                        

                    }
                    else{

                        for(cas_fila=fila+1;cas_fila-fila<=ATAQUE_ALCANCE && cas_fila < MAPA_MAXY ;cas_fila++){
                            symbol=mapa_get_symbol(mapa, cas_fila, columna);
                            if(symbol!=SYMB_VACIO && symbol!=mapa_get_symbol(mapa, fila, columna)){
                                /*NAVE ENEMIGA ENCONTRADA*/
                                /*ENCONTRAR LA NAVE ENEMIGA QUE ESTA EN ESA POSICION*/
                                /*n,m*/
                                for(m=0;m<3;m++){
                                    for(n=0;n<3;n++){
                                      if(mapa->info_naves[m][n].posx==columna && mapa->info_naves[m][n].posy==cas_fila) {
                                            encontrado=true;
                                            break;
                                      } 

                                    }
                                    if(encontrado){
                                        break;
                                    }
                                }

                                break;
                            }

                        }
                        if(encontrado){
                            mapa->info_naves[m][n].vida=mapa->info_naves[m][n].vida - ATAQUE_DANO;

                            if(mapa->info_naves[m][n].vida<=0){
                                mapa->info_naves[m][n].viva=false;
                                mapa_set_symbol(mapa, cas_fila, columna, SYMB_DESTRUIDO);
                                printf("\nNAVE [%d], pertenezco al jefe %d, he destruido la nave %d del jefe%d", (j), (i),n,m);
                                fflush(stdout);
                                mapa->accion_pendiente[m]=1;
                            }
                            else{
                                mapa_set_symbol(mapa, cas_fila, columna, SYMB_TOCADO);
                                printf("\nNAVE [%d], pertenezco al jefe %d, he daniado la nave %d del jefe%d", (j), (i),n,m);
                                fflush(stdout);
                            }
                            

                        }
                        else{

                            for(cas_fila=fila-1;fila-cas_fila<=ATAQUE_ALCANCE && cas_fila >=0 ;cas_fila--){
                            symbol=mapa_get_symbol(mapa, cas_fila, columna);
                            if(symbol!=SYMB_VACIO && symbol!=mapa_get_symbol(mapa, fila, columna)){
                                /*NAVE ENEMIGA ENCONTRADA*/
                                /*ENCONTRAR LA NAVE ENEMIGA QUE ESTA EN ESA POSICION*/
                                /*n,m*/
                                for(m=0;m<3;m++){
                                    for(n=0;n<3;n++){
                                      if(mapa->info_naves[m][n].posx==columna && mapa->info_naves[m][n].posy==cas_fila) {
                                            encontrado=true;
                                            break;
                                      } 

                                    }
                                    if(encontrado){
                                        break;
                                    }
                                }

                                break;
                            }

                        }
                        if(encontrado){
                            mapa->info_naves[m][n].vida=mapa->info_naves[m][n].vida - ATAQUE_DANO;

                            if(mapa->info_naves[m][n].vida<=0){
                                mapa->info_naves[m][n].viva=false;
                                mapa_set_symbol(mapa, cas_fila, columna, SYMB_DESTRUIDO);
                                printf("\nNAVE [%d], pertenezco al jefe %d, he destruido la nave %d del jefe%d", (j), (i),n,m);
                                fflush(stdout);
                                mapa->accion_pendiente[m]=1;
                            }
                            else{
                                mapa_set_symbol(mapa, cas_fila, columna, SYMB_TOCADO);
                                printf("\nNAVE [%d], pertenezco al jefe %d, he daniado la nave %d del jefe%d", (j), (i),n,m);
                                fflush(stdout);
                            }
                            

                        }
                        else{

                            printf("\nNAVE [%d], pertenezco al jefe %d, no he encontrado objetivos a rango", (j), (i));
                            fflush(stdout);

                        }



                        }


                    }


                }


            }
            else if(!strcmp(mensaje,"DESTRUIR")){
                /*Toca destruirse en el mapa*/
                mapa->info_naves[i][j].viva=false;
                mapa_clean_casilla(mapa, fila, columna);
                mapa_set_num_naves(mapa, i, (mapa_get_num_naves(mapa, i))-1);
                printf("\nNAVE [%d], pertenezco al jefe %d, he sido destruida.", (j), (i));
                fflush(stdout);
            }

        }

        printf("\nNAVE [%d]: Nave ha recibido orden de terminar por partida acabada, pertenezco al jefe %d...", (j), (i));
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }

    if(!pid_padre && pid_hijos[0]){ /*Aqui solo van a entrar los jefes*/
        while(!juego_terminado){

            /*Leer el mensaje que le haya mandado el simulador*/
            read(fd_jefes[i][0], &mensaje, sizeof(mensaje));

            mapa->accion_pendiente[i]=0;

            /*Poner semaforos entre cada nave*/

            if(!strcmp(mensaje,"TURNO")){
                /*Toca turno, primero comprueba que la nave esta viva antes de mandarle mensaje*/


                usleep(SCREEN_REFRESH);
                
                if(mapa->info_naves[i][0].viva==true){

                    printf("\nJEFE [%d]: mandando mover a nave 0.\n", (i));
                    strcpy(mensaje,"MOVER");
                    write(fd_naves[0][1], &mensaje, sizeof(mensaje));

                    printf("\nJEFE [%d]: mandando atacar a nave 0.\n", (i));
                    strcpy(mensaje,"ATACAR");
                    write(fd_naves[0][1], &mensaje, sizeof(mensaje));
                }
                else{
                    /*Toca destruir la nave 0, puesto que ya ha perdido toda su vida*/
                    printf("\nJEFE [%d]: mandando destruir a nave 0.\n", (i));
                    strcpy(mensaje,"DESTRUIR");
                    write(fd_naves[0][1], &mensaje, sizeof(mensaje));

                }
                
                usleep(SCREEN_REFRESH);
                
                if(mapa->info_naves[i][1].viva==true){

                    printf("\nJEFE [%d]: mandando mover a nave 1.\n", (i));
                    strcpy(mensaje,"MOVER");
                    write(fd_naves[1][1], &mensaje, sizeof(mensaje));

                    printf("\nJEFE [%d]: mandando atacar a nave 1.\n", (i));
                    strcpy(mensaje,"ATACAR");
                    write(fd_naves[1][1], &mensaje, sizeof(mensaje));
                }
                else{
                    /*Toca destruir la nave 0, puesto que ya ha perdido toda su vida*/
                    printf("\nJEFE [%d]: mandando destruir a nave 1.\n", (i));
                    strcpy(mensaje,"DESTRUIR");
                    write(fd_naves[1][1], &mensaje, sizeof(mensaje));

                }
                
                usleep(SCREEN_REFRESH);
                
                if(mapa->info_naves[i][2].viva==true){

                    printf("\nJEFE [%d]: mandando mover a nave 2.\n", (i));
                    strcpy(mensaje,"MOVER");
                    write(fd_naves[2][1], &mensaje, sizeof(mensaje));

                    printf("\nJEFE [%d]: mandando mover a nave 2.\n", (i));
                    strcpy(mensaje,"ATACAR");
                    write(fd_naves[2][1], &mensaje, sizeof(mensaje));
                }
                else{
                    /*Toca destruir la nave 0, puesto que ya ha perdido toda su vida*/
                    printf("\nJEFE [%d]: mandando destruir a nave 2.\n", (i));
                    strcpy(mensaje,"DESTRUIR");
                    write(fd_naves[2][1], &mensaje, sizeof(mensaje));

                }
                

            }
            else if(!strcmp(mensaje,"FIN")){
                /*Toca terminar*/

                printf("\nJEFE [%d]: mandando fin a naves.\n", (i));
                write(fd_naves[0][1], &mensaje, sizeof(mensaje));
                write(fd_naves[1][1], &mensaje, sizeof(mensaje));
                write(fd_naves[2][1], &mensaje, sizeof(mensaje));
                wait(NULL);
                wait(NULL);
                wait(NULL);

                juego_terminado=1;
            }

        }

        printf("\nJEFE [%d] ha recibido orden de terminar tras partida acabada...", (i));
        fflush(stdout);

        exit(EXIT_SUCCESS);
    }

    if(pid_padre){ /*Aqui solo va a entrar el padre*/

        while(!juego_terminado){

            printf("\nSimulador: Empieza turno %d\n",turno);
            fflush(stdout);

            
            strcpy(mensaje,"TURNO");
            /*Comienza el turno, primero comprueba que el equipo este vivo antes de darle turno o tengan una accion
                pendiente a realizar como es mandar destruir una nave*/


            /*Poner semaforos entre cada turno*/

            if(mapa_get_num_naves(mapa,0)>0||mapa->accion_pendiente[0]){
                write(fd_jefes[0][1], &mensaje, sizeof(mensaje));
            }
            sleep(1);
            if(mapa_get_num_naves(mapa,1)>0||mapa->accion_pendiente[1]){
                write(fd_jefes[1][1], &mensaje, sizeof(mensaje));
            }
            sleep(1);
            if(mapa_get_num_naves(mapa,2)>0||mapa->accion_pendiente[2]){
                write(fd_jefes[2][1], &mensaje, sizeof(mensaje));
            }
            sleep(1);
            


            for(k=0,cont=0;k<N_EQUIPOS;k++){
                if(mapa_get_num_naves(mapa,k)==0){
                    cont++;
                }

            }

            if(cont>=2){
                juego_terminado=1;
            }

            turno++;
        }


        if(mapa_get_num_naves(mapa,0)==0 && mapa_get_num_naves(mapa,1)==0){

            printf("\nJuego terminado, ha ganado el equipo 2\n");

        }
        else if(mapa_get_num_naves(mapa,0)==0 && mapa_get_num_naves(mapa,2)==0){

            printf("\nJuego terminado, ha ganado el equipo 1\n");

        }
        else if(mapa_get_num_naves(mapa,1)==0 && mapa_get_num_naves(mapa,2)==0){

            printf("\nJuego terminado, ha ganado el equipo 0\n");

        }

        /*Juego ha terminado y manda el mensaje acabar a sus hijos jefes, que ya llaman ellos a sus naves*/
        strcpy(mensaje,"FIN");

        write(fd_jefes[0][1], &mensaje, sizeof(mensaje));

        write(fd_jefes[1][1], &mensaje, sizeof(mensaje));

        write(fd_jefes[2][1], &mensaje, sizeof(mensaje));

        wait(NULL);
        wait(NULL);
        wait(NULL);

    }

    


    //pause();/*Padre esperando a control+c*/


    destruir_mapa(mapa);
    sem_close(sem_simulador);
 	sem_unlink(SEM);
    exit(EXIT_SUCCESS);

}


// ------ FUNCIONES AUXILIARES ------ //


//------------------------------------------------------------------------------------------



tipo_mapa * iniciar_mapa(){

    // ------ CREAMOS MEMORIA COMPARTIDA CON PERMISOS DE LECTURA Y ESCRITURA ------ //

    int fd_shm = shm_open(SHM_MAP_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR); 
    
    if(fd_shm == -1){
        fprintf (stderr, "SIMULADOR: Error creando la memoria compartida del mapa.\n");
        return NULL;
    }
    
    // ------ AJUSTAMOS EL TAMAÑO DE LA MEMORIA PARA QUE SEA EL DE TIPO_MAPA ------ //
    
    int error = ftruncate(fd_shm, sizeof(tipo_mapa));

    if(error == -1){
        fprintf (stderr, "SIMULADOR: Error ajustando el tamaño de la memoria compartida al tipo_mapa. \n");
        shm_unlink(SHM_MAP_NAME);
        return NULL;
    }
    
    /* MAPEAMOS LA MEMORIA COMPARTIDA */
    
    tipo_mapa * mapa_aux = mmap(NULL, sizeof(*mapa_aux), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    
    if(mapa_aux == MAP_FAILED){
        fprintf (stderr, "SIMULADOR: Error mapeando la memoria creada. \n");
        shm_unlink(SHM_MAP_NAME);
        return NULL;
    }

    mapa_aux->terminado=false;

    mapa_aux->accion_pendiente[0]=0;
    mapa_aux->accion_pendiente[1]=0;
    mapa_aux->accion_pendiente[2]=0;
    
    return mapa_aux;

}


//------------------------------------------------------------------------------------------



void estado_inicial_mapa(tipo_mapa * mapa){

    int i;

    if(mapa == NULL){
        printf("SIMULADOR: No se puede inicializar, mapa vacio.\n");
        return;
    }

    estado_inicial_naves(mapa);                 // RELLENAMOS LOS VALORES DE LAS NAVES
    estado_inicial_casillas(mapa);                // RELLENAMOS LOS VALORES DE LAS CASILLAS
    for(i = 0; i < N_EQUIPOS; mapa->num_naves[i] = 3, i++); // INDICAMOS QUE HAY 3 NAVES POR EQUIPO

}



//------------------------------------------------------------------------------------------



void estado_inicial_naves(tipo_mapa * mapa){

    int i, j;

    if(mapa == NULL){
        printf("SIMULADOR: No se puede inicializar, mapa vacio.\n");
        return;
    }

    for(i = 0; i < N_EQUIPOS; i++){
        for(j = 0; j < N_NAVES; j++){

            mapa->info_naves[i][j].equipo = i;
            mapa->info_naves[i][j].vida = VIDA_MAX;
            mapa->info_naves[i][j].viva = true;

            if(i == 0){

                mapa->info_naves[i][j].numNave = equipo0;

                if(mapa->info_naves[i][j].numNave == 0){

                    mapa->info_naves[i][j].posx = 0;
                    mapa->info_naves[i][j].posy = 0;

                }else if(mapa->info_naves[i][j].numNave == 1){

                    mapa->info_naves[i][j].posx = 0;
                    mapa->info_naves[i][j].posy = 1;

                }else{

                    mapa->info_naves[i][j].posx = 1;
                    mapa->info_naves[i][j].posy = 0;

                }

                equipo0++;

            }else if(i == 1){

                mapa->info_naves[i][j].numNave = equipo1;

                if(mapa->info_naves[i][j].numNave == 0){

                    mapa->info_naves[i][j].posx = 19;
                    mapa->info_naves[i][j].posy = 0;

                }else if(mapa->info_naves[i][j].numNave == 1){

                    mapa->info_naves[i][j].posx = 18;
                    mapa->info_naves[i][j].posy = 0;

                }else{

                    mapa->info_naves[i][j].posx = 19;
                    mapa->info_naves[i][j].posy = 1;
                    
                }

                equipo1++;
                

            }else{

                mapa->info_naves[i][j].numNave = equipo2;

                if(mapa->info_naves[i][j].numNave == 0){

                    mapa->info_naves[i][j].posx = 19;
                    mapa->info_naves[i][j].posy = 19;

                }else if(mapa->info_naves[i][j].numNave == 1){

                    mapa->info_naves[i][j].posx = 19;
                    mapa->info_naves[i][j].posy = 18;

                }else{

                    mapa->info_naves[i][j].posx = 18;
                    mapa->info_naves[i][j].posy = 19;
                    
                }

                equipo2++;
                

            }

        }
    }

}



//------------------------------------------------------------------------------------------



void estado_inicial_casillas(tipo_mapa * mapa){

    int i, j;

    if(mapa == NULL){
        printf("SIMULADOR: No se puede inicializar, mapa vacio.\n");
        return;
    }

    for(i = 0; i < MAPA_MAXX; i++){
        for(j = 0; j < MAPA_MAXY; j++){

            mapa->casillas[i][j].equipo = -1;
            mapa->casillas[i][j].numNave = -1;
            mapa->casillas[i][j].simbolo = SYMB_VACIO;

        }
    }

    // Establecemos las naves donde les corresponde al inicio de la partida en cada casilla - Funcion de mapa.h
    // COORDENADAS: 
    // Equipo 0: (0, 0) - (0, 1) - (1, 0)
    // Equipo 1: (20, 0) - (19, 0) - (20, 1)
    // Equipo 2: (19, 20) - (20, 19) - (20, 20)

    for(i = 0; i < N_EQUIPOS; i++){
        for(j = 0; j < N_NAVES; j++){

            if( mapa_set_nave(mapa, mapa->info_naves[i][j]) == -1){
                printf("No se ha puesto la nave del equipo %d numero %d.\n", i, j);
            }

        }
    }

}



//------------------------------------------------------------------------------------------



void destruir_mapa(tipo_mapa * mapa){

    if(mapa){

        mapa->terminado=true;

        munmap(mapa, sizeof(*mapa));

        shm_unlink(SHM_MAP_NAME);

    }

}



//------------------------------------------------------------------------------------------