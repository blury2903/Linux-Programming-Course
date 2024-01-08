#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int wstatus, wpid;
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
            wpid = waitpid(child_pid, &wstatus, 0);
            if(wpid == -1)
            {
                printf("waitpid() error\n");
                return 0; 
            }
            if(WIFEXITED(wstatus))
            {
                printf("exited, status=%d\n", WEXITSTATUS(wstatus));
            }
            else if(WIFSIGNALED(wstatus))
            {
                printf("killed by signal %d\n", WTERMSIG(wstatus));
            }
            else if(WIFSTOPPED(wstatus))
            {
                printf("stopped by signal %d\n", WSTOPSIG(wstatus));
            }
            else if(WIFCONTINUED(wstatus))
            {
                printf("continued\n");
            } 
        }
    }
    else
    {
        printf("fork() error\n"); 
    }

    return 0;
}