#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

void displayMenu() {
	printf("\n\n-----Mid-Day Commander v0-----\nGood day, Commander! What command would you like to run?\n");
	printf("0:\twhoami\n");
	printf("1:\tlast\n");
	printf("2:\tls\n");
	printf("Option:");
	/*
	printf("a. add command : Adds a new command to the menu.");
	printf("c. change directory : Changes process working directory");
	printf("e. exit : Leave Mid-Day Commander");
	printf("p. pwd : Prints working directory");	
	*/
}

void getCommand(char usrchar, char **file, char **arg) {

	if(usrchar == '0') {
		printf("----whoami----\n");
		*file = "whoami";
		arg[0] = "whoami";
		arg[1] = NULL;
	}else if (usrchar == '1') {
		printf("----last----\n");
		*file = "last";
		arg[0] = "last";
		arg[1] = NULL;
	}
	else if (usrchar == '2') {
		printf("----list directory contents----\n");
		*file = "ls";
		arg[0] = "ls";
		
		printf("Arguments?: ");
		size_t numchars = 0;
		getline(&arg[1], &numchars, stdin);
		arg[1] = strtok(arg[1], "\n");
		
		printf("Directory?: ");		
		getline(&arg[2], &numchars, stdin);
		arg[2] = strtok(arg[2], "\n");
		arg[3] = NULL;

	}else {
		printf("You picked incorrectly");
	}

	//printf("Executing command: %s, arg 1: %s\n", *file, arg[0]);
}



int main() {
	start:
	displayMenu();

	char *usrstr;
	size_t numchars = 0;
	getline(&usrstr, &numchars, stdin);
	
	char *cmd = NULL; 
	char *arg[64] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};	
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
	goto start;
	return 0;
}


