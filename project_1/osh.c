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
int osh_run(char **args, int is_bg, char *previous_line);
int osh_run_built_in(char **args, char *previous_line);
int osh_run_process(char **args, int is_background);
void osh_parse(char *line, char **args);
int built_in(char **args);
void save_previous(char *previous_line, char *line);
int is_bg(char **args);

void osh_parse(char *line, char **args)
{
    char *split = strtok(line, " ");
    int i = 0;
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

int osh_run_built_in(char **args, char *previous_line)
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
        int is_background = 0;
        char *previous_args[MAX_LINE / 2 + 1];
        if (strlen(previous_line) == 0)
        {
            fprintf(stderr, "osh: no command in history \n");
        }
        else
        {
            char previous_copy[MAX_LINE];
            save_previous(previous_copy, previous_line);
            osh_parse(previous_line, previous_args);
            is_background = is_bg(args);
            return osh_run(previous_args, is_background, previous_copy);
        }
    }
    return 1;
}

int osh_run_process(char **args, int is_background)
{
    //printf("%d", is_background);
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
        if (!is_background)
            waitpid(pid, NULL, 0);
        //wait(NULL);
        //printf("Child Complete \n");
    }
    return 1;
}

int osh_run(char **args, int is_background, char *previous_line)
{
    if (built_in(args))
    {
        return osh_run_built_in(args, previous_line);
    }
    else
    {
        return osh_run_process(args, is_background);
    }
}

void save_previous(char *previous_line, char *line)
{
    if (strcmp(line, "!!") != 0)
    {
        strcpy(previous_line, line);
    }
}

int is_bg(char **args)
{
    int i;
    while (args[i] != NULL)
        i++;

    int j;
    while (args[i - 1][j] != '\0')
        j++;

    //printf("%c \n", args[i - 1][j - 1]);
    if (args[i - 1][j - 1] == '&')
    {
        args[i - 1][j - 1] = '\0';
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;
}

int main(void)
{
    char line[MAX_LINE];
    char previous_line[MAX_LINE];
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int shouldrun = 1;            /* flag to determine when to exit program */
    int is_background = 0;
    while (shouldrun)
    {
        // 1. Print prompt
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("osh: %s $ ", cwd);
        fflush(stdout);
        // 2. Read command line
        gets(line);
        save_previous(previous_line, line);
        // 3. Parse command
        osh_parse(line, args);
        is_background = is_bg(args);
        // 4. Run command
        shouldrun = osh_run(args, is_background, previous_line);
    }
    return 0;
}

