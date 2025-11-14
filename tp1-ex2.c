#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

pid_t ppere;
void mafonction (int s) {
	if (ppere !=getpid()) {
		
		if (s==SIGINT) {
			printf("SIGINT fils recu de %d\n",getpid());
		} else {
			exit (0);
		}
	} else {
		if (s==SIGINT) {
			printf("pere ctrlc\n");
			
		}
	}
}


void fils () {
	while (1) {
		sleep(1);
		printf("mon num %d\n",getpid());
	}
}


int main (int argc, char* argv[]) {
	if (argc!=1) {
		fprintf(stderr,"Usage: ex1\n");
		exit(1);
	}
	
	ppere=getpid();
	
	struct sigaction s;
	s.sa_handler= mafonction;
	sigemptyset(&(s.sa_mask));
	sigaddset(&(s.sa_mask), SIGINT);
	s.sa_flags = SA_RESTART;
	
	sigaction(SIGINT, &s, NULL);
	
	pid_t pid = fork ();
	
	if (pid == 0) {
		fils();
	} else if (pid <0) {
		perror("erreur fork\n");
		exit (1);
	}
	
	wait(NULL);
	printf("je me finis en dernier\n");
	return 0;
}
