#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "util.h"

//keep track of usr commands and number of commands
static char *usrCommands[100][33];
static int bgCommands[100] = {0};
static int command_count = 0;
static int *runningProcesses;
static int numRunningProcesses = 0;

// displayMenu
// displays the static menu elements
//
void displayMenu() {
	printf("\n\n-----Mid-Day Commander v0-----\nGood day, Commander! What command would you like to run?\n");
	printf("0:\twhoami:\tprints out username\n");
	printf("1:\tlast:\tprints out last command\n");
	printf("2:\tls:\tlists directory contents\n");
	printf("a:\tadd command:\tAdds a new command to the menu.\n");
	printf("c:\tchange directory:\tChanges process working directory\n");
	printf("e:\texit:\tLeave Mid-Day Commander\n");
	printf("p:\tpwd:\tPrints working directory\n");
	printf("r:\trunning processes:\tPrints running background processes\n");
	printUsrCommands();
	printf("Option:");
}

// printUsrCommands
// prints the user commands stored in usrCommands
//
void printUsrCommands() {
	printf("\n----User Commands----\n");
	for(int command = 0; command < command_count; command++) {
		printf("%d:\t%s:\t", command + 3, usrCommands[command][0]);
		int arg = 1;
		while (usrCommands[command][arg] != NULL) {
			printf("%s ",usrCommands[command][arg]);
			arg++;
		}
		if(bgCommands[command]){
			printf("\t(background)");
		}
		printf("\n");
	}
}

//getUsrString
//reads a line from stdin, does some error checking
//
void getUsrString(char **buf, int buflimit) {
	//getline(&usrstr, &numchars, stdin);
	char *newBuf = malloc(buflimit);
	char *rc = fgets(newBuf, buflimit, stdin);
	if(rc == NULL) { //fgets returns NULL on both Error and when EOF occurs with no characters
		printf("\nERROR: read EOF. Due to this the program will exit\n");
		exit(0);
	}
	*buf = strndup(newBuf, 127);
}


//stripNewlineAddSpace
//strips all newline characters and replaces them with spaces.
//Note: buf must be of size buflimit
void replaceNewline(char **buf, int buflimit, const char replaceChar) {
	assert(strlen(*buf) <= buflimit-1);
	char *newBuf = malloc(buflimit);
	for(int i=0; i < buflimit; i++) {
		if((*buf)[i] == '\0') {
			newBuf[i] = '\0';
			break;
		}else {
			if((*buf)[i] == '\n') {
				newBuf[i] = replaceChar;
			} else {
				newBuf[i] = (*buf)[i];
			}
		}
	}
	*buf = newBuf;
}

int containsAmpersand(char **arg) {
	int rc = NULL;
	int i = 0;
	while(arg[i] != NULL) {
		int len = strlen(arg[i]);
		if(arg[i][len-1] == '&') {
			return 1;
		}
		i++;
	}
	return 0;
}

int markAsBackground(char **arg, int commandNum) {
	int rc = 0;
	int i = 0;
	while(arg[i] != NULL) {
		int len = strlen(arg[i]);
		if(arg[i][len-1] == '&') {
			arg[i][len-1] = '\0';
			bgCommands[commandNum] = 1;
		}
		i++;
	}
	return 0;
}

//check if arg matches any known commands, then checks if it's a background command
int isBackgroundCommand(char *arg[]) {
	int i =0;
	while(i < command_count) {
		int strIndex = 0;
		int matches = 1;
		while(usrCommands[i][strIndex] != NULL && arg[strIndex] != NULL) {
			if(!strcmp(usrCommands[i][strIndex], arg[strIndex])) {
				//matches
			}else {
				matches = 0; //Doesn't match, go to next string
			}
			strIndex++;
		}
		if(matches) {
			if(bgCommands[i] == 1) {
				return 1;
			}else {
				return 0;
			}
		}
		i++;
	} //Didn't find match
	return 0; //Didn't even see the command
}

//getCommand
//takes the usercharacter and then performs all the requisite processing to
//get the arguments. Also does the heavy lifting for storing commands in usrCommands
//return value: 0 for child process, see enum for parent process
commandID getCommand(char usrchar, char **file, char *arg[]) {
	//
	if(isdigit(usrchar)) {
		int usrDigit = usrchar - '0';
		if(usrDigit < 3){
			; //do nothing
		}else if(usrDigit <= (command_count + 2)) {
			int i = 0;
			*file = usrCommands[usrDigit-3][i];
			while(usrCommands[usrDigit-3][i] != NULL) {
				arg[i] = usrCommands[usrDigit-3][i];
				i++;
			}
			arg[i] = usrCommands[usrDigit-3][i];
			return CHILDCOMMAND;
		}else {
			printf("You input an invalid digit. Valid digits are 3-%d\n", command_count + 2);
			return ERRORCOMMAND;
		}
	}
	if(usrchar == '0') {
		printf("----whoami----\n");
		*file = "whoami";
		arg[0] = "whoami";
		arg[1] = NULL;
	} else if (usrchar == '1') {
		printf("\n----last logins----\n");
		*file = "last";
		arg[0] = "last";
		arg[1] = NULL;
	} else if (usrchar == '2') {
		printf("----list directory contents----\n");
		*file = "ls";
		arg[0] = "ls";
		printf("Arguments?: ");
		getUsrString(&arg[1], buflimit);
		replaceNewline(&arg[1], buflimit, '\0');
		printf("\n%s\n", arg[1]);
		
		printf("Directory?: ");	
		getUsrString(&arg[2], buflimit);
		replaceNewline(&arg[2], buflimit, '\0');
		arg[3] = NULL;

	} else if (usrchar == 'a') {
		printf("\n----Add Command----\n");
		char *newline = NULL;
		getUsrString(&newline, buflimit);
		replaceNewline(&newline, buflimit, ' ');

		char UnambiguouslyModifiableString[buflimit];
		strcpy(UnambiguouslyModifiableString, newline); 

		char *str = NULL;
		//parse newline for command, then arguments
		const char *delim = " ";
		int count = 0;
		arg[count] = strtok(UnambiguouslyModifiableString, delim);
		while(arg[count] != NULL && count < 33) {
			count ++;
			arg[count] = strtok(NULL, delim);
		}
		//check for error
		if(count > 32) {
			printf("\nERROR: You may have tried to enter more than 32 arguments. As a result the program is exiting.\n");
			exit(0);
		}

		if(containsAmpersand(arg)) {
			markAsBackground(arg, command_count);
		}

		//printf("copying\n");
		//copy this into our known user commands
		char * buf;
		int i = 0;
		while(arg[i] != NULL){
			buf = malloc(buflimit);
			if(arg[i][0] == '\0') {
				arg[i] = NULL;
				usrCommands[command_count][i] = NULL;
				break;
			}
			strcpy(buf, arg[i]);
			usrCommands[command_count][i] = buf;
			//printf("Copying: %s\n", usrCommands[command_count][i]);
			i++;
		}
		
		printf("appending null\n");
		usrCommands[command_count][i] = NULL;
		command_count ++;
		return ADDCOMMAND;

	} else if (usrchar == 'c') {
		printf("----Change Directory----\n");
		printf("Path? (Quotes/spaces not supported)");
		char * newline = NULL;
		getUsrString(&newline, buflimit);
		arg[0] = newline;
		return CHANGECOMMAND;
	} else if (usrchar == 'e') {
		return EXITCOMMAND;
	} else if (usrchar == 'p') { 
		return PRINTCOMMAND;
	} else if(usrchar == 'r') {
		return RUNNINGCOMMAND;
	} else {
		printf("You picked incorrectly");
		return ERRORCOMMAND;
	}
	return CHILDCOMMAND;
}

int addProcess(pid_t pid) {
	int newsize;
	if(runningProcesses == NULL) {
		runningProcesses = malloc(sizeof(pid_t) * 1000);
	}
	/*else {
		newsize = sizeof(runningProcesses) + sizeof(pid_t);
	}
	int *newArray = runningProcesses;
	runningProcesses = calloc(numRunningProcesses + 1, sizeof(int));
	for(int i=0; i < numRunningProcesses; i++) {
		runningProcesses[i] = newArray[i];
	}
	runningProcesses[numRunningProcesses + 1] = pid;
	numRunningProcesses++;
	//free(newArray);
	//runningProcesses = newArray;
	//runningProcesses = realloc(runningProcesses, newsize);
	//runningProcesses[numRunningProcesses] = pid;
	*/
	runningProcesses[numRunningProcesses] = pid;
	numRunningProcesses++;
	return 0;
}

int removeProcess(int pid) {
	if(numRunningProcesses == 0) {
		printf("\nERROR: NO RUNNING PROCESSES TO REMOVE\n");
	}
	if(isRunningProcess(pid)) {
		numRunningProcesses = numRunningProcesses - 1;
	}
	return 0;
	/*
	if(runningProcesses == NULL) {
		printf("ERROR: ATTEMPTED TO REMOVE PROCESS FROM EMPTY ARRAY\n");
		return -1;
	}
	//int *newArray = malloc(sizeof(runningProcesses) - sizeof(int));
	int *newArray = calloc(numRunningProcesses - 1, sizeof(pid_t));
	int removedIndexMod = 0;
	for(int i = 0; i < numRunningProcesses; i++) {
		if(runningProcesses[i] == pid) {
			removedIndexMod++;
		}
		else{
			newArray[i-removedIndexMod] = runningProcesses[i]; 
		}
	}
	//free(runningProcesses);
	runningProcesses = newArray;
	*/
}

int removeAllProcesses(int *list, int num) {
	int newList[numRunningProcesses-num];
	for(int i =0; i < numRunningProcesses; i++) {
		int add = 1;
		for(int j = 0; j < num ; j++) {
			if (runningProcesses[i] == list[j]) {
				add=0;
				printf("Process %d removed\n", list[j]);
				break;
			}
		}
		if(add) {
				newList[i] = runningProcesses[i];
		}
	}
	//overwrite old list with newlist
	int i = 0;
	while(i < numRunningProcesses-num) {
		runningProcesses[i] = newList[i];
		i++;
	}
	while(i < 1000) {
		runningProcesses[i] = 0;
		i++;
	}
	return 0;
}

int numBackgroundProcesses() {
	return numRunningProcesses;
}

int *getProcessList() {
	return runningProcesses;
}

int isRunningProcess(int pid) {
	if(runningProcesses == NULL) {
		return 0;
	}
	for(int i = 0; i < numRunningProcesses; i++) {
		if(runningProcesses[i] == pid) {
			return 1;
		}
	}
	return 0;
}

void printUsageStats(int pid, struct timeval *pidStart) {
	static struct rusage usage;
	static long cuMajflt = 0, cuMinflt =0;
	struct timeval end;
	gettimeofday(&end, NULL);
	getrusage(RUSAGE_CHILDREN, &usage);

	//Display Stats
	printf("---Statistics---\n");
	long elapsedTime = (end.tv_sec-pidStart->tv_sec) * 1000 + (end.tv_usec-pidStart->tv_usec)/1000;
	printf("Elapsed Time: %ld \n", elapsedTime);
	printf("Hard page faults: %ld\n", usage.ru_majflt - cuMajflt); //only print difference in accumulated stats
	printf("Soft page faults: %ld\n", usage.ru_minflt - cuMinflt);

	//Store Stats
	cuMajflt += usage.ru_majflt - cuMajflt; //add the new difference
	cuMinflt += usage.ru_minflt - cuMinflt;
}