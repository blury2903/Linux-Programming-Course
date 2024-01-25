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
    char buff_read[BUFF_SIZE];
    int fd;

    // mkfifo(<pathname>, <permission>)
    mkfifo(FIFO_FILE, 0666);

    while (1) {
        // Read first
        fd = open(FIFO_FILE, O_RDONLY);
        read(fd, buff_read, BUFF_SIZE);

        if(strncmp("exit\n", buff_read, 6) == 0) {
            printf("The reader program exited...\n");
            return 0;
        }
        printf("Message from writer: %s", buff_read);
        close(fd);

    }

    return 0;
}
