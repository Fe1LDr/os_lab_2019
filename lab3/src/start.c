#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
    int pid = fork();
	if ( pid == 0 ) {
		execv( "./sequential_min_max", argv);
	}
	/* Put the parent to sleep for 2 seconds--let the child finished executing */
    return 0;
}