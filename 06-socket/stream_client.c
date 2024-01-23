#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     //  Chứa cấu trúc cần thiết cho socket. 
#include <netinet/in.h>     //  Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <unistd.h>

#define BUFF_SIZE 256
#define PORT 3000
#define SERV_IP "192.168.159.128"

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
		


int main(int argc, char *argv[])
{
    int serv_fd, numb_read, numb_write;
    char buff_read[BUFF_SIZE], buff_write[BUFF_SIZE];
    struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    //create socket
    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(serv_fd == -1)
        handle_error("socket() error");
    
    //connect to server
    if(connect(serv_fd, (const struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        handle_error("connect() error");

    while(1) {
        printf("Type message: ");
        fgets(buff_write, sizeof(buff_write), stdin);
        if(strncmp("exit\n", buff_write, 6) == 0)
        {
            numb_write = write(serv_fd, buff_write, sizeof(buff_write));
            printf("Client exit\n");
            exit(EXIT_FAILURE);
        } else {
            numb_write = write(serv_fd, buff_write, sizeof(buff_write));
        }
        if(numb_write == -1)
            handle_error("write() error");

        numb_read = read(serv_fd, buff_read, sizeof(buff_read));
        if(numb_read == -1)
            handle_error("read() error");
        printf("%s\n", buff_read);
	    memset(buff_write, 0, sizeof(BUFF_SIZE));
	    memset(buff_read, 0, sizeof(BUFF_SIZE));

    }

    close(serv_fd);

    return 0;
}

