///////////////////////////////////////////////////////////////////////////////
/// \file         Shell.c
/// \author       Paul Valdez
/// \date         December 6, 2020
/// \brief        Shell simulator that can affect files on computer.
///
///
/// REVISION HISTORY:
/// \date  12/06/2020 (created)
///
///////////////////////////////////////////////////////////////////////////////

#include "shell.h"

char *myTokenize(char *line, int *my_argc, char *my_argv[]) {
    *my_argc = 0;

    char temp[BUFSIZE] = "";
    strcpy(temp, line);

    char *s = strtok(temp, " ");
    while (s) {
        *(my_argv + *my_argc) = (char *) malloc(sizeof(char) * strlen(s));
        strcpy(*(my_argv + *my_argc), s);
        *my_argc += 1;
        s = strtok(NULL, " ");
    }

    //Set the end of myargv to a null
    *(my_argv + *my_argc) = 0;
    return my_argv[0];
}

int myCd(char *pathname) {
    if (pathname) {
        if (*pathname == '/') {
            pathname++;
        }
        return chdir(pathname);
    } else {
        return chdir(getenv("HOME"));
    }
}

int myScan(char *command_line, char *head, char *tail) {
    int command_line_length = strlen(command_line);

    // Check for pipe from the back
    for (int index = command_line_length - 1; index >= 0; index--)
    {
        if (*(command_line + index) == '|')
        {
            strcpy(tail, command_line + index + 1);
            strncpy(head, command_line, index);
            *(command_line + index) = '\0';
            return 1;
        }
    }
    // If no pipe, then set head to command line and tail to null
    strcpy(head, command_line);
    tail = NULL;
    return 0;
}

void myPipe(char *command_line, int *pd, char *env[])
{
    int pipe_exists = 0;
    char head[BUFSIZE] = "", tail[BUFSIZE] = "";

    // Verify if pipe was passed in as writer
    if (pd)
    {
        close(*pd);
        dup2(*(pd + 1), 1);
        close(*(pd + 1));
    }

    // Check for pipe and divide into head and tail.
    pipe_exists = myScan(command_line, head, tail);
    //Check if there is a pipe
    if (pipe_exists)
    {
        pipe(lpd);
        pid = fork();

        // Check if current process is the parent
        if (pid)
        {
            // Open as reader on lpd
            close(*(lpd + 1));
            dup2(*lpd, 0);
            close(*lpd);

            myCommand(tail, env);
        }
        else
        {
            myPipe(head, lpd, env);
        }
    }
    else
    {
        myCommand(command_line, env);
    }
}

void myCommand(char *command_line, char *env[])
{
    int my_argc = 0;
    char *cmd = "", *my_argv[BUFSIZE] = { "" };
    cmd = myTokenize(command_line, &my_argc, my_argv);

    int file_descriptor = 0;
    // Verify IO redirection
    for (int i = my_argc - 1; i >= 0; --i)
    {
        // Open file to read.
        if (strcmp(my_argv[i], "<") == 0)
        {
            close(0);
            file_descriptor = open(my_argv[i + 1], O_RDONLY);
            my_argv[i] = 0;
            break;
        }
        // Open file to write.
        if (strcmp(my_argv[i], ">") == 0)
        {
            close(1);
            file_descriptor = open(my_argv[i + 1], O_WRONLY | O_CREAT, 0644);
            my_argv[i] = 0;
            break;
        }
        // Open file to append.
        if (strcmp(my_argv[i], ">>") == 0)
        {
            close(1);
            file_descriptor = open(my_argv[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            my_argv[i] = 0;
            break;
        }
    }
    
    char *pathname = "";
    pathname = getenv("PATH");
    char *path_components = strtok(pathname, ":");

    int command_length = strlen(cmd);
    char temp[command_length + 1], full_command[BUFSIZE] = "";
    strcpy(temp, "/");
    strcat(temp, cmd);

    while (path_components)
    {
        strcpy(full_command, path_components);
        strcat(full_command, temp);
        execve(full_command, my_argv, env);
        path_components = strtok(NULL, ":");
    }
}

int myShell(int argc, char *argv[], char *env[])
{
    int home_size = strlen(getenv("HOME"));

    // For personalization
    char user_id[BUFSIZE] = "", host_name[BUFSIZE] = "";
    getlogin_r(user_id, BUFSIZE);
    gethostname(host_name, BUFSIZE);

    int my_argc = 0;
    char buf[BUFSIZE] = "", *current_dir = "", command_line[BUFSIZE] = "", *cmd = "";
    char *my_argv[BUFSIZE] = { "" };
    while (1)
    {
        getcwd(buf, BUFSIZE);
        current_dir = buf + home_size;

        // Personalizated display prompt
        printf("\033[1;35m%s@%s:\033[1;36m~%s\033[0m$ ", user_id, host_name, current_dir);

        fgets(command_line, BUFSIZE, stdin);

        if (strcmp(command_line, "\n") != 0)
        {
            // Clear command line new line character
            *(command_line + strlen(command_line) - 1) = '\0';
            cmd = myTokenize(command_line, &my_argc, my_argv);
            if (strcmp(cmd, "cd") == 0)
            {
                myCd(my_argv[1]);
            }
            else if (strcmp(cmd, "exit") == 0)
            {
                memoryManager(&my_argc, my_argv);
                exit(0);
            }
            else
            {
                cpid = fork();
                if (cpid)
                {
                    // Check for a background process.
                    if (strcmp(my_argv[my_argc - 1], "&") != 0)
                    {
                        // Wait for child process
                        cpid = wait(&cpid);
                    }
                }
                else
                {
                    myPipe(command_line, 0, env);
                }
            }
        }
        // Clear memory and reset my_argv
        memoryManager(&my_argc, my_argv);
    }
    return 0;
}

void memoryManager(int *my_argc, char *my_argv[]) {
    for (int index = 0; index < *my_argc; ++index) {
        free(*(my_argv + index));
        *(my_argv + index) = NULL;
    }
    *my_argc = 0;
}

int main(int argc, char *argv[], char *env[])
{
    myShell(argc, argv, env);
    return 0;
}