#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
/*gcc ./tp1.c -o test*/
int main (int argc, char* argv[]) {
	if (argc!=3) {
		fprintf(stderr,"Usage: boucler nbsec nbfois\n");
		exit(1);
	}
	const char* fois = argv[2];
	for (int i=0;i<atoi(fois);i++) {
		pid_t p = getpid();
		printf("Mon numero est %d, il est ...\n");
		sleep(atoi(argv[1]));
	}
	return 0;
}
