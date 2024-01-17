#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h> 

#define PATH_NAME   "singleThr_text.txt"

pthread_t thread_id1;


static void *thr_handle(void *args) 
{
    int fd;
    char* pn = PATH_NAME; 
    fd = open(pn, O_WRONLY | O_CREAT, 0666);
    if (fd == -1) { 
	    printf("open() %s is failed\n", pn);
        pthread_exit(NULL);
    } else {
        for(int i = 0; i < 1000000; i++) {
            write(fd, "1", 1);
        }
    }
    close(fd);
    pthread_exit(NULL);

}

int main(int argc, char const *argv[])
{

    int ret;

    if (ret = pthread_create(&thread_id1, NULL, &thr_handle, NULL)) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }




    pthread_join(thread_id1, NULL);

    printf("All of threads done!\n");

    return 0;
}
