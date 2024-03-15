#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t child_pid;
    char *addr = mmap(0, 1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
    {
       printf("%s: mmap failed\n", __FILE__);
    }
    strcpy(addr, "Message before call fork function");
    printf("%s\n", addr);
    child_pid = fork();         
    if (child_pid >= 0) {
        if (0 == child_pid) {   
            strcpy(addr, "Message after call fork function");
            printf("I'm the child process\n");
            printf("My PID is: %d, my parent PID is: %d\n", getpid(), getppid());  
            printf("Message: %s\n", addr);
            if (munmap(addr, sizeof(addr)) == -1)
            {
               printf("munmap fail\n");
            }
            exit(EXIT_SUCCESS);
        } else {                    
            wait(NULL); //Wait for the child process to finish
            printf("I'm the parent process\n");
            printf("My PID is: %d\n", getpid());  
            printf("Message: %s\n", addr);
            if (munmap(addr, sizeof(int)) == -1)
            {
                printf("munmap fail\n");
            }
            exit(EXIT_SUCCESS);
        }
    } else {
        printf("fork() unsuccessfully\n");     
    }

    return 0;
}
