/**
 * Assignment 1
 * Author: Sidharth Sudarsan
 * Course: EECS3221 | Winter 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define MAX_LINE 80      /* The maximum length command */
#define BUILT_IN_COUNT 4 /* The number of commands built-in to osh */

char *built_in_functions[BUILT_IN_COUNT] = {"cd", "help", "exit", "!!"};
char *prev_args[MAX_LINE / 2 + 1];
int osh_run(char **args, char *prev_line);
int osh_run_built_in(char **args, char *prev_line);
int osh_run_process(char **args);
void osh_parse(char *line, char **args);
int built_in(char **args);
void set_prev_args(char *line);
int command_count = 0;

void osh_parse(char *line, char **args)
{
    char *split = strtok(line, " ");
    int i = 0;
    args[i] = split;

    while (split != NULL)
    {
        args[i] = split;
        i++;
        split = strtok(NULL, " ");
    }
    args[i] = NULL;
}

int built_in(char **args)
{
    int i;
    for (i = 0; i < BUILT_IN_COUNT; i++)
    {
        if (strcmp(*args, built_in_functions[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void set_prev_args(char *line)
{
    osh_parse(line, prev_args);
}

int osh_run_built_in(char **args, char *prev_line)
{
    if (strcmp(*args, "cd") == 0)
    {
        if (chdir(args[1]) < 0)
        {
            perror(args[1]);
        }
    }
    else if (strcmp(*args, "help") == 0)
    {
        printf("OSH, version 1.0.0 by Sidharth Sudarsan\n");
        printf("These shell commands are defined internally.  Type `help' to see this list.\n");
        printf("Use `man -k' to find out more about commands not in this list.\n");
        printf("\n cd \n help \n exit \n");
    }
    else if (strcmp(*args, "exit") == 0)
    {
        return 0;
    }
    else if (strcmp(*args, "!!") == 0)
    {
        //fprintf(stderr, "osh: %s\n", prev_args[0]);
        //perror(prev_args);
        //return osh_run_previous();
        char *l_args[MAX_LINE / 2 + 1];
        osh_parse(prev_line, l_args);
        //fprintf(stderr, "osh: %s\n", l_args[0]);
        return osh_run(l_args, "");
    }
    return 1;
}

int osh_run_process(char **args)
{
    pid_t pid;
    /* fork a child process */
    pid = fork();
    if (pid < 0)
    { /* error occurred */
        fprintf(stderr, "osh: fork child failed \n");
        return 0;
    }
    else if (pid == 0)
    { /* child process will invoke execvp() */
        if (execvp(*args, args) < 0)
        {
            fprintf(stderr, "osh: command not found: %s\n", *args);
        }
    }
    else
    { /* parent process will invoke wait() for the child to complete unless command included & */
        wait(NULL);
        //printf("Child Complete \n");
    }
    return 1;
}

int osh_run(char **args, char *prev_line)
{
    // Run either built-in or process according to command
    if (built_in(args))
    {
        return osh_run_built_in(args, prev_line);
    }
    else
    {
        return osh_run_process(args);
    }
}

int main(void)
{
    char line[MAX_LINE];
    char line_copy[MAX_LINE];
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int shouldrun = 1;            /* flag to determine when to exit program */
    char prev_line[MAX_LINE] = "";
    while (shouldrun)
    {
        // 1. Print prompt
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("osh: %s $ ", cwd);
        fflush(stdout);

        // 2. Read command line
        gets(line);
        strcpy(line_copy, line);
        printf("%s\n", prev_line);
        // 3. Parse command
        osh_parse(line, args);

        // 4. Run command
        shouldrun = osh_run(args, prev_line);

        strcpy(prev_line, line_copy);
    }
    return 0;
}