///////////////////////////////////////////////////////////////////////////////
/// \file         Shell.h
/// \author       Paul Valdez
/// \date         December 6, 2020
/// \brief        Shell simulator that can affect files on computer.
///
///
/// REVISION HISTORY:
/// \date  12/06/2020 (created)
///
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> //for O_RDONLY, O_WRONLY
#include <sys/wait.h> //For wait

#ifndef SHELL_H
#define SHELL_H

#define BUFSIZE 256 //size = 4kb = 1024
#define PATHSIZE 1024

int lpd[2];
pid_t cpid, pid;

char *myTokenize(char *line, int *my_argc, char *my_argv[]);
int myCd(char *pathname);
int myScan(char *command_line, char *head, char *tail);
void myPipe(char *command_line, int *pd, char *env[]);
void myCommand(char *command_line, char *env[]);
int myShell(int argc, char *argv[], char *env[]);
void memoryManager(int *my_argc, char *my_argv[]);

#endif