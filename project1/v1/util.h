#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

//keep track of usr commands and number of commands
static char **usrcommands = NULL;
static int command_count = 0;
const size_t buflimit = 128;
static const int buf_size = 128 * 64; //characters per string * number of strings 


void displayMenu() {
	printf("\n\n-----Mid-Day Commander v0-----\nGood day, Commander! What command would you like to run?\n");
	printf("0:\twhoami:\tprints out username\n");
	printf("1:\tlast:\tprints out last command\n");
	printf("2:\tls:\tlists directory contents\n");
	printf("a:\tadd command:\tAdds a new command to the menu.\n");
	printf("c:\tchange directory:\tChanges process working directory\n");
	printf("e:\texit:\tLeave Mid-Day Commander\n");
	printf("p:\tpwd:\tPrints working directory\n");
	printf("Option:");
}

void getUsrString(char **buf, int buflimit) {
	//getline(&usrstr, &numchars, stdin);
	char * newBuf = malloc(128);
	fgets(newBuf, buflimit, stdin);
	newBuf = strtok(newBuf, "\n");
	*buf = strndup(newBuf, 128);
}

void getCommand(char usrchar, char **file, char **arg) {
	

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
		printf("----Add Command----");
		char *newline = NULL;
		getUsrString(&newline, buflimit);

		char *str = NULL;
		//parse newline for command, then arguments
		str = strtok(newline, " ");
		int argCount = 0;
		while(str != NULL) {
			arg[argCount] = &str;

			argCount ++;
			str = strtok(NULL, " ");
		}
		//append final NULL
		arg[argCount + 1] = null;

	} else if (usrchar == 'c') {
	
	} else if (usrchar == 'e') {

	} else if (usrchar = 'p') { 

	} else {
		printf("You picked incorrectly");
	}

	//printf("Executing command: %s, arg 1: %s\n", *file, arg[0]);
}

#endif