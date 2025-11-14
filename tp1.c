#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {
	if (argc!=3) {
		fprintf(stderr,"Usage: boucler nbsec nbfois\n");
	}
	const char* fois = argv[2];
	for (int i=0;i<atoi(fois);i++) {
		pid_t p = getpid();
		printf("Mon numero est %d, il est ...\n");
		sleep(atoi(argv[1]));
	}
	return 0;
}
