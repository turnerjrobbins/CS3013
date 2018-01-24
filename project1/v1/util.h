#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

static const size_t buflimit = 128;
static const int buf_size = 128 * 64; //characters per string * number of strings 

enum CommandID {
	CHILDCOMMAND,
	ADDCOMMAND,
	CHANGECOMMAND,
	EXITCOMMAND,
	PRINTCOMMAND,
	ERRORCOMMAND
};

void displayMenu();

//void tokenizer(char *);

void printUsrCommands();

void replaceNewline(char **buf, int buflimit, const char replaceChar);

void getUsrString(char **buf, int buflimit);

enum CommandID getCommand(char usrchar, char **file, char *arg[]);



#endif