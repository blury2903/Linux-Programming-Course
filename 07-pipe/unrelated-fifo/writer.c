#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// FIFO file path
#define FIFO_FILE   "./myfifo"
#define BUFF_SIZE   1024

int main(int argc, char const *argv[])
{   
    char buff_write[BUFF_SIZE];
    int fd;

    // mkfifo(<pathname>, <permission>)
    mkfifo(FIFO_FILE, 0666);

    while (1) {
        printf("Message to reader : ");   
        fgets(buff_write, BUFF_SIZE, stdin);

        fd = open(FIFO_FILE, O_WRONLY);
        if (fd == -1) { 
            printf("open() %s is failed\n", FIFO_FILE);
            return 0;
        } 
        write(fd, buff_write, strlen(buff_write) + 1);
        close(fd);
        if(strncmp("exit\n", buff_write, 6) == 0) {
            printf("The writer program exited...\n");
            return 0;
        }
    }

    return 0;
}
