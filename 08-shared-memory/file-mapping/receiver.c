#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

#define SHARED_MEM_SIZE     100            
#define FILE_NAME           "share_mem_obj" 



int main()
{
    int fd = shm_open(FILE_NAME, O_RDWR, 0666); 
    if (fd < 0) {
        printf("shm_open() is failed, %s.\n", strerror(errno));
        return -1;
    }

    if (ftruncate(fd, SHARED_MEM_SIZE) < 0) {
        printf("ftruncate() is failed, %s.\n", strerror(errno));
        return -1;
    }


    char *data = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // return addr
    if (data == MAP_FAILED){
        printf("mmap() is failed, %s.\n", strerror(errno));
        return -1;
    }

    printf("%s: Write data: %s\n", __FILE__, data);
    munmap(data, SHARED_MEM_SIZE);
    close(fd);
    shm_unlink(FILE_NAME);

    return 0;
}