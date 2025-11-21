#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
void mafonction (int s) {
	if (s==SIGUSR1) {
		printf("SIGUSR1 recu de %d\n",getpid());
	}
}
int main () {
	
	struct sigaction s;
	s.sa_handler= mafonction;
	sigemptyset(&(s.sa_mask));
	s.sa_flags = 0;
	sigaction(SIGUSR1, &s, NULL);
	
	while (1) {
		sleep(2);
		printf("mon num %d\n",getpid());
	}
	return 0;
}
