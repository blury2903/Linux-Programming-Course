#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void func(int signum)
{
    printf("Im in func()\n");
    wait(NULL);
}

int main(int argc, char const *argv[])
{
    pid_t child_pid;
    child_pid = fork();

    if(child_pid >= 0)
    {
        if(child_pid == 0)
        {
            printf("I'm the child process\n");
            printf("My PID is: %d, my parent PID is: %d\n", getpid(), getppid());
            while(1);
        }
        else
        {   
            signal(SIGCHLD, func);
            printf("I'm the parent process\n");
            printf("My PID is: %d\n", getpid());
            while(1);
        }
    }
    else
    {
        printf("fork() error\n"); 
    }

    return 0;
}