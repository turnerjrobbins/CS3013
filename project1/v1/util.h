void displayMenu() {
	printf("\n\n-----Mid-Day Commander v0-----\nGood day, Commander! What command would you like to run?\n");
	printf("0:\twhoami\n");
	printf("1:\tlast\n");
	printf("2:\tls\n");
	printf("Option:");
	printf("a. add command : Adds a new command to the menu.");
	printf("c. change directory : Changes process working directory");
	printf("e. exit : Leave Mid-Day Commander");
	printf("p. pwd : Prints working directory");
}

static const int string_size = 128;
static const in num_commands = 64;

void getCommand(char usrchar, char **file, char **arg) {
	static char **usrcommands = malloc(sizeof(char) * string_size * num_commands);
	static int command_count = 0;
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
		size_t numchars = 0;
		getline(&arg[1], &numchars, stdin);
		arg[1] = strtok(arg[1], "\n");
		
		printf("Directory?: ");		
		getline(&arg[2], &numchars, stdin);
		arg[2] = strtok(arg[2], "\n");
		arg[3] = NULL;

	} else if (usrchar == 'a') {
		printf("----Add Command----");

	} else if (usrchar == 'c') {
	
	} else if (usrchar == 'e') {

	} else if (usrchar = 'p') { 

	} else {
		printf("You picked incorrectly");
	}

	//printf("Executing command: %s, arg 1: %s\n", *file, arg[0]);
}
