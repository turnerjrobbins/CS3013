#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "util.h"

//
//
//
int main() {
	start: //I suppose I could use a while loop, but this is more fun
	//because it makes Dijkstra toss in his grave (is he dead?).
	displayMenu();
	
	char *usrstr = malloc(sizeof(char) * 128); //char * 128 might be redundant
	
	getUsrString(&usrstr, buflimit); //gets input (see util.h)

	// Initialize structures for storing usr command and args
	char *cmd = NULL; 
	char *arg[32] = { NULL };	
	getCommand(*usrstr, &cmd, arg);

	pid_t child_pid = fork();

	struct timeval start, end;	
	struct rusage usage;
	gettimeofday(&start, NULL);

	if(child_pid ==0) {
		execvp(cmd, arg);
	}else if (child_pid < 0) {
		fprintf(stderr, "Whoops, something wrong with fork!");
	}else {
		wait(0);
		gettimeofday(&end, NULL);
		printf("---Statistics---\n");
		struct rusage usage;
		getrusage(RUSAGE_CHILDREN, &usage);
		printf("Elapsed Time: %ld microseconds\n", end.tv_usec - start.tv_usec);
		printf("Hard page faults: %ld\n", usage.ru_majflt);
		printf("Soft page faults: %ld\n", usage.ru_minflt);

	}

	goto start; //TAKE THAT, DIJKSTRA!
	return 0; //appendage, pls remove
}


