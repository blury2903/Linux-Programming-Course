#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


#define MSG_SIZE    20

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

char *msg1 = "This is message 1";
char *msg2 = "This is message 2";


int main(int argc, char const *argv[])
{
    char buff_read[MSG_SIZE];
    int num_read = 0;
    int fds[2];
    pid_t child_pid;

    if (pipe(fds) < 0) {
        handle_error("pipe() error\n");
    }

    child_pid = fork();
    
    if(child_pid >= 0) {
        if(child_pid == 0) {
            printf("This is a child process, I'm a reader\n");
            printf("My PID is: %d, my parent PID is: %d\n", getpid(), getppid());
            if(close(fds[1] == -1)) {
                handle_error("close(fds[1]) error\n");
            }
            while(1) {
                num_read = read(fds[0], buff_read, MSG_SIZE);
                if (num_read == -1) {
                    handle_error("read() error\n");
                } else if (num_read == 0) {
                    printf("pipe end-of-pipe\n");
                    if(close(fds[0] == -1)) {
                        handle_error("close(fds[0]) error\n");
                    }
                    break;
                } else {
                    printf("msg from parent process: %s\n", buff_read);
                }
            }
        } else {
            printf("This is a parent process, I'm a writer\n");
            printf("My PID is: %d\n", getpid());
            if(close(fds[0] == -1)) {
                handle_error("close(fds[0]) error\n");
            }

            write(fds[1], msg1, MSG_SIZE);
            write(fds[1], msg2, MSG_SIZE);
            if (close(fds[1]) == -1) {
                handle_error("close(fds[1]) error\n");
            }

            while(1);
        }
    } else {
        handle_error("fork() error");
    }

    return 0;
}
