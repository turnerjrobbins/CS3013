#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

static const size_t buflimit = 128;
static const int buf_size = 128 * 64; //characters per string * number of strings 

typedef enum {
	CHILDCOMMAND,
	ADDCOMMAND,
	CHANGECOMMAND,
	EXITCOMMAND,
	PRINTCOMMAND,
	RUNNINGCOMMAND,
	ERRORCOMMAND
}commandID;

void displayMenu();

//void tokenizer(char *);

void printUsrCommands();

int containsAmpersand(char **arg);

int markAsBackground(char **arg, int commandNum);

int isBackgroundCommand(char *arg[]);

void replaceNewline(char **buf, int buflimit, const char replaceChar);

void getUsrString(char **buf, int buflimit);

commandID getCommand(char usrchar, char **file, char *arg[]);

int addProcess(pid_t pid);

int removeProcess(pid_t pid);

int removeAllProcesses(int *list, int num);

int numBackgroundProcesses();

int *getProcessList();

int isRunningProcess(int pid);

void printUsageStats(int pid, struct timeval *pidStart);





#endif