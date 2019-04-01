#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define N_ITER 5
#define SECS 40


void manejador_SIGALRM(int sig) {
	printf("\nAlarma\n");
}



int main (void) {
	pid_t pid;
	int counter;

	sigset_t set, set1, oset;

	struct sigaction act;

	sigemptyset(&(act.sa_mask));
	act.sa_flags = 0;


	
	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {

		act.sa_handler = manejador_SIGALRM;
		if (sigaction(SIGALRM, &act, NULL) < 0) {
			perror("sigaction");
			exit(EXIT_FAILURE);
		}

		if (alarm(SECS)){
			fprintf(stderr, "Existe una alarma previa establecida\n");
		}

		sigemptyset(&set);
		sigaddset(&set, SIGUSR1);
		sigaddset(&set, SIGUSR2);	
		sigaddset(&set, SIGALRM);


		sigemptyset(&set1);
		sigaddset(&set, SIGUSR2);	



		while(1) {

			if (sigprocmask(SIG_BLOCK, &set, &oset) < 0) {
				perror("sigprocmask");
				exit(EXIT_FAILURE);
			}

			for (counter = 0; counter < N_ITER; counter++) {
				printf("%d\n", counter);
				sleep(1);
			}

			if (sigprocmask(SIG_BLOCK, &set1, &oset) < 0) {
				perror("sigprocmask");
				exit(EXIT_FAILURE);
			}

			sleep(3);
		}
	}
	while (wait(NULL) > 0);
} 
