#include<stdio.h> 
#include<string.h> 
#include<unistd.h> 
#include<fcntl.h> 

#define PATH_NAME "hello.txt"
  
int main (void) 
{ 
    int fd;
    char* pn = PATH_NAME; 
    int numb_read, numb_write;
    char buf1[14] = "Hello Hello \n"; 

    fd = open(pn, O_RDWR | O_APPEND);

    if (fd == -1)
    { 
	    printf("open() %s is failed\n", pn);
        return 0;
    } 

    int aaaa = lseek(fd, 0, SEEK_SET);
    printf("%d\n", aaaa);
    write(fd, buf1, strlen(buf1));

    close(fd); 

    return 0; 
} 