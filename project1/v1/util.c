#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

#include "util.h"

//keep track of usr commands and number of commands
static char *usrCommands[100][33];
static int command_count = 0;

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

//getCommand
//takes the usercharacter and then performs all the requisite processing to
//get the arguments. Also does the heavy lifting for storing commands in usrCommands
//return value: 0 for child process, see enum for parent process
enum CommandID getCommand(char usrchar, char **file, char *arg[]) {
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
		printf("----last----\n");
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
		printf("----Add Command----\n");
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

		printf("copying\n");
		//copy this into our known user commands
		char * buf;
		int i = 0;
		while(arg[i] != NULL){
			buf = malloc(buflimit);
			strcpy(buf, arg[i]);
			usrCommands[command_count][i] = buf;
			printf("Copying: %s\n", usrCommands[command_count][i]);
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
	} else {
		printf("You picked incorrectly");
		return ERRORCOMMAND;
	}
	return CHILDCOMMAND;
}