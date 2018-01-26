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
	//variables for storing cumulative rusage
	
	start: //I suppose I could use a while loop, but this is more fun
	//because it makes Dijkstra toss in his grave (is he dead?).
	displayMenu();
	
	char *usrstr = malloc(sizeof(char) * 128); //char * 128 might be redundant
	
	getUsrString(&usrstr, buflimit); //gets input (see util.h)
	//printf("%s\n",usrstr);
	replaceNewline(&usrstr, buflimit, '\0');
	if(strlen(usrstr) > 1) {
		printf("\nPlease input only 1 character\n");
		goto start;
	}

	// Initialize structures for storing usr command and args
	char *cmd = NULL; 
	char *arg[33] = { NULL }; //33 = 32 + NULL
	commandID command_rc = getCommand(usrstr[0], &cmd, arg); //see util.h for the typedef commandID

	//only fork off a child if the command is supposed to be a child command
	pid_t child_pid;
	if(command_rc == CHILDCOMMAND) {  
		child_pid = fork();
	}

	//start timer
	struct timeval start, end;	
	gettimeofday(&start, NULL);

	if(command_rc == 0 && child_pid == 0) { //execute child command
		execvp(cmd, arg);
	}else if (child_pid < 0) {
		fprintf(stderr, "Whoops, something wrong with fork!");
	}else { //this is the main process branch
		if(command_rc != 0) { // execute parent command
			//printf("command_rc is %d, runningcommand is %d\n", command_rc, RUNNINGCOMMAND);
			switch(command_rc) {
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
				case RUNNINGCOMMAND:
					printf("\n---Background Processes---\n");
					int* pl = getProcessList();
					for(int i = 0; i < numBackgroundProcesses(); i++){
						printf("PID: %d\n", pl[i]);
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
					if(numBackgroundProcesses()) {
						printf("There are outstanding processes. Waiting for those to finish\n");
						int i = 0;
						while(i < numBackgroundProcesses()) { //wait until all processes have been waited for, and handle statistics
							int exitWait_rc = waitpid(-1, NULL, 0);
							if(exitWait_rc == -1) {
								printf("ERROR: %s\n", strerror(errno));
							}
							printf("\n----Process %d has finished----\n", exitWait_rc);
							printUsageStats(exitWait_rc, &start);
							i++;
						}
					}
					printf("Exit called, exiting...\n");
					return 0;
					break;
				default:
					printf("Parent command not recognized\n");
					break;
			}

		} else { //If a child command was executed, we need to handle waiting for it
			int waitpid_rc = 1;
			//if the command contains an ampersand check waitpid for any finished background processes
			printf("Running %s :: %d\n", arg[0], isBackgroundCommand(arg));
			if(isBackgroundCommand(arg)) {
				//run as background command
				//add the command to the list of running commands
				addProcess(child_pid);
				int numProcesses = numBackgroundProcesses();
				int *toRemove = malloc(sizeof(pid_t) * numProcesses);
				int rn = 0;
				for(int i=0; i < numProcesses; i++) {
					waitpid_rc = waitpid(getProcessList()[i],NULL, WNOHANG);
					if(waitpid_rc > 0) {
						//print stats of rc here
						printf("\n----Process %d has finished----\n", waitpid_rc);
						printUsageStats(waitpid_rc, &start);
						//remove rc from list?
						toRemove[rn] = waitpid_rc;
						rn++;
					}else if(waitpid_rc==-1){
						printf("\nERROR: Child returned with error: %s\n", strerror(errno));
					}
				}
				removeAllProcesses(toRemove, rn);
				printf("No processes finished\n");
			//else if the command is a normal command, wait for the command then print stats
			}else {
				waitpid_rc = 0;
				while(waitpid_rc != child_pid) {
					waitpid_rc = waitpid(-1, NULL, 0);
					if(waitpid_rc==child_pid) {
						printf("\n%s has finished\n",arg[0]);
						printUsageStats(waitpid_rc, &start);
					}else if(isRunningProcess(waitpid_rc)) {
						printf("\n----Process %d has finished----\n", waitpid_rc);
						printUsageStats(waitpid_rc, &start);
						removeProcess(waitpid_rc);
						printf("PROCESS REMOVED: %d\n",waitpid_rc);
					}
				}
			}
		}
	}

	goto start; //TAKE THAT, DIJKSTRA!
	return 0; //appendage, pls remove
}


