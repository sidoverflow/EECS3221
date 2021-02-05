#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define MAX_LINE 80 /* The maximum length command */

int osh_run(char *line[])
{
    pid_t pid;
    /* fork a child process */
    pid = fork();
    if (pid < 0)
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0)
    { /* child process will invoke execvp() */
        char file[] = "/bin/";
        strcat(file, line);
        execlp(line, line, NULL);
    }
    else
    { /* parent process will invoke wait() for the child to complete unless command included & */
        wait(NULL);
        printf("Child Complete \n");
    }
}

int main(void)
{
    char *line[MAX_LINE];
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int shouldrun = 1;            /* flag to determine when to exit program */
    while (shouldrun)
    {
        // 1. Print prompt
        printf("osh:~$ ");
        // 2. Read command line
        gets(line);
        // 3. Parse command

        // 4. Record command in history

        // 5. Check if command is built-in and run it
        if (strcmp(line, "exit") == 0)
            exit(0);
        // 6. Else, run given command
        int status = osh_run(line);
        if (status == 1)
            shouldrun = 0;
    }
}