#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
        }
        else
        {   
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