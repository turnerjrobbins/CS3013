#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

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
	printf("\n");

	// Initialize structures for storing usr command and args
	char *cmd = NULL; 
	char *arg[32] = { NULL };
	int rc = getCommand(*usrstr, &cmd, arg); //returns 1 if command for parent process

	pid_t child_pid;
	if(rc == 0) {  
		child_pid = fork();
	}

	struct timeval start, end;	
	struct rusage usage;
	gettimeofday(&start, NULL);

	if(rc == 0 && child_pid == 0) { //execute child command
		execvp(cmd, arg);
	}else if (child_pid < 0) {
		fprintf(stderr, "Whoops, something wrong with fork!");
	}else {
		if(rc) { // execute parent command
			switch(rc) {
				case ADDCOMMAND:
					printf("processing add command\n");
					break;
				case CHANGECOMMAND:
					printf("processing change command: %d %s\n", getpid(), arg[0]);
					arg[0][strlen(arg[0]) - 1] = '\0';
					const char* cdBuf = arg[0];
					int rc = chdir(cdBuf);
					if(!rc==0) {
						printf("DIRECTORY CHANGE FAILED, CHECK YOUR PATH: %s\n", strerror(errno));
					}
					break;
				case PRINTCOMMAND:
					printf("processing print command %d\n", getpid());
					char *buf = malloc(buflimit);
					getcwd(buf, buflimit);
					printf("Current Working Directory: %s", buf);
					free(buf);
					break;
				case EXITCOMMAND:
					printf("Exit called, exiting...");
					return 0;
					break;
				default:
					printf("Parent command not recognized\n");
					break;
			}

		} else {
			wait(0);
			gettimeofday(&end, NULL);
			printf("---Statistics---\n");
			struct rusage usage;
			getrusage(RUSAGE_CHILDREN, &usage);
			printf("Elapsed Time: %ld microseconds\n", end.tv_usec - start.tv_usec);
			printf("Hard page faults: %ld\n", usage.ru_majflt);
			printf("Soft page faults: %ld\n", usage.ru_minflt);
		}
	}

	goto start; //TAKE THAT, DIJKSTRA!
	return 0; //appendage, pls remove
}


