#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

static const int command_count = 0;
static const size_t buflimit = 128;
static const int buf_size = 128 * 64; //characters per string * number of strings 


void displayMenu();

void printUsrCommands();

void getUsrString(char **buf, int buflimit);

void getCommand(char usrchar, char **file, char **arg);



#endif