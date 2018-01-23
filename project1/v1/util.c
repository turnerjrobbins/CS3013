#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

//keep track of usr commands and number of commands
static char *usrCommands[100][32];
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
		printf("%d:\t%s:\t", command + 4, usrCommands[command][0]);
		int arg = 1;
		while (usrCommands[command][arg] != NULL) {
			printf("%s ",usrCommands[command][arg]);
			arg++;
		}
		printf("\n");
	}
}

//
//
//
void getUsrString(char **buf, int buflimit) {
	//getline(&usrstr, &numchars, stdin);
	char *newBuf = malloc(buflimit);
	fgets(newBuf, buflimit, stdin);
	*buf = strndup(newBuf, 127);
}

//
//
//return value: 0 for child process, 1 for parent process
enum CommandID getCommand(char usrchar, char **file, char *arg[]) {
	//
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
		printf("\n%s\n", arg[1]);
		
		printf("Directory?: ");		
		getUsrString(&arg[2], buflimit);
		arg[3] = NULL;

	} else if (usrchar == 'a') {
		printf("----Add Command----\n");
		char *newline = NULL;
		getUsrString(&newline, buflimit);

		char UnambiguouslyModifiableString[buflimit];
		strcpy(UnambiguouslyModifiableString, newline); 

		char *str = NULL;
		//parse newline for command, then arguments
		const char *delim = " ";
		int count = 0;
		arg[count] = strtok(UnambiguouslyModifiableString, delim);
		while(arg[count] != NULL) {
			count ++;
			arg[count] = strtok(NULL, delim);
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
	} else if (usrchar = 'p') { 

		return PRINTCOMMAND;
	} else {
		printf("You picked incorrectly");
		return ERRORCOMMAND;
	}
	return CHILDCOMMAND;
}