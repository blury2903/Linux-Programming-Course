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
    int serv_fd, client_fd, numb_read, numb_write, client_len;
    char buff_read[BUFF_SIZE];
    char buff_write[BUFF_SIZE] = "Server received message";
    struct sockaddr_in serv_addr, client_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr =  INADDR_ANY;

    //create socket
    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(serv_fd == -1)
        handle_error("socket() error");
    
    if(bind(serv_fd, (const struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind() error");

    if (listen(serv_fd, 1) == -1)
        handle_error("listen()");


    client_len = sizeof(client_addr);
    client_fd = accept(serv_fd, (struct sockaddr*)&client_addr, (socklen_t *) &client_len); 
    if(client_fd == -1)
        handle_error("accept() error");

    while(1) {
        numb_read = read(client_fd, buff_read, sizeof(buff_read));
        if(numb_read == -1)
            handle_error("read() error");
        if(strncmp("exit\n", buff_read, 6) == 0) {
            printf("Server exit\n");
            exit(EXIT_FAILURE);
        }
        printf("Message from client: %s\n", buff_read);
        memset(buff_read, 0, sizeof(buff_read));
        
        numb_write = write(client_fd, buff_write, sizeof(buff_write));
        if(numb_write == -1)
            handle_error("write() error");
    }

    close(client_fd);

    close(serv_fd);

    return 0;
}

