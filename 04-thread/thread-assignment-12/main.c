#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#define PATH_NAME "infomation.txt"

pthread_t thread_id1, thread_id2, thread_id3;
pthread_mutex_t thr_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thr_cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t thr_cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t thr_cond3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t thr_cond4 = PTHREAD_COND_INITIALIZER;

struct student {
    char name[30];
    char date_of_birth[20];
    char country[20];
};

int cur_thr = 1;
int flag = 1;
int fd;
char *pn = PATH_NAME;
size_t size_wrData;

void rm_newLine(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void *thr_inputData(void *args) {
    struct student *data = (struct student *)args;

    while (1) {
        pthread_mutex_lock(&thr_mutex);
        while (cur_thr != 1) {
            pthread_cond_wait(&thr_cond1, &thr_mutex);
            if (flag == 0) {
                printf("Thread 1 is exiting\n");
                sleep(1);
                pthread_cond_signal(&thr_cond2);
                pthread_mutex_unlock(&thr_mutex);
                pthread_exit(NULL);
            }
        }

        printf("This is thread 1\n\n");

        printf("Input your name: \n");
        fgets(data->name, sizeof(data->name) - 1, stdin);

        printf("Input your date of birth: \n");
        fgets(data->date_of_birth, sizeof(data->date_of_birth) - 1, stdin);

        printf("Input your country: \n");
        fgets(data->country, sizeof(data->country) - 1, stdin);

        sleep(1);
        cur_thr = 2;

        pthread_cond_signal(&thr_cond2);
        pthread_mutex_unlock(&thr_mutex);
    }

    pthread_exit(NULL);
}

void *thr_writeData(void *args) {
    struct student *data = (struct student *)args;
    
    while (1) {
        pthread_mutex_lock(&thr_mutex);
        while (cur_thr != 2) {
            pthread_cond_wait(&thr_cond2, &thr_mutex);
            if (flag == 0) {
                printf("Thread 2 is exiting\n");
                sleep(1);
                pthread_cond_signal(&thr_cond3);
                pthread_mutex_unlock(&thr_mutex);
                pthread_exit(NULL);
            }
        }

        printf("\nThis is thread 2\n\n");

        fd = open(pn, O_WRONLY | O_APPEND | O_CREAT, 0666);

        if (fd == -1) { 
            printf("open() %s is failed\n", pn);
        } else {
            char wr_data[100] = {0};
            rm_newLine(data->name);
            rm_newLine(data->date_of_birth);
            rm_newLine(data->country);
            sprintf(wr_data, "%s\t %s\t %s\n", data->name, data->date_of_birth, data->country);
            size_wrData = write(fd, wr_data, strlen(wr_data));
        }
        close(fd);

        printf("Successfully wrote to %s\n\n", pn);

        sleep(1);
        cur_thr = 3;

        pthread_cond_signal(&thr_cond3);
        pthread_mutex_unlock(&thr_mutex);
    }
    pthread_exit(NULL);
}

void *thr_printData(void *args) {
    struct student *data = (struct student *)args;
    char buffer[100];

    while (1) {
        pthread_mutex_lock(&thr_mutex);
        while (cur_thr != 3) {
            pthread_cond_wait(&thr_cond3, &thr_mutex);
            if (flag == 0) {
                printf("Thread 3 is exiting\n");
                sleep(1);
                pthread_mutex_unlock(&thr_mutex);
                pthread_exit(NULL);
            }
        }

        printf("This is thread 3\n\n");

        fd = open(pn, O_RDONLY | O_CREAT, 0666);

        if (fd == -1) { 
            printf("open() %s is failed\n", pn);
        } else {
            lseek(fd, -size_wrData, SEEK_END);
            read(fd, buffer, size_wrData);
            printf("%s\n", buffer);
            size_wrData = 0;
        }
        close(fd);

        printf("Successfully read from %s\n\n", pn);

        sleep(1);
        cur_thr = 4;

        pthread_cond_signal(&thr_cond4);
        pthread_mutex_unlock(&thr_mutex);
    }
    pthread_exit(NULL);
}

int main() {
    struct student *data = malloc(sizeof(struct student));

    pthread_create(&thread_id1, NULL, &thr_inputData, data);
    pthread_create(&thread_id2, NULL, &thr_writeData, data);
    pthread_create(&thread_id3, NULL, &thr_printData, data);

    pthread_mutex_lock(&thr_mutex);
    pthread_cond_signal(&thr_cond1);
    while (1) {
        while (cur_thr != 4) {
            pthread_cond_wait(&thr_cond4, &thr_mutex);
        }

        printf("Type 'e' to quit or ENTER to continue program\n");
        char e_input[2];
        fgets(e_input, sizeof(e_input), stdin);

        if (strcmp(e_input, "e") == 0) {
            flag = 0;
            pthread_cond_signal(&thr_cond1);
            sleep(1);
            break;
        } else {
            flag = 1;
            pthread_cond_signal(&thr_cond1);
            sleep(1);
            cur_thr = 1;
        }
    }

    pthread_mutex_unlock(&thr_mutex);

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);
    pthread_join(thread_id3, NULL);

    free(data);

    printf("All threads done!\n");
    sleep(1);

    return 0;
}
