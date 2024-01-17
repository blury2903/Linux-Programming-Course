#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_t thread_id1, thread_id2;

struct human {
    char name[30];
    char year_of_birth[10];
    char phone_number[20];
    char country[30];
};

static void *thr_handle(void *args) 
{
    pthread_t cthread = pthread_self();
    struct human *data = (struct human *)args;

    if (pthread_equal(cthread, thread_id1)) 
    {
        printf("This is thread without information\n\n");
    } 
    else 
    {
        printf("This is thread with information\n\n");
        printf("Name: %s\n\n", data->name);
        printf("Year of birth: %s\n", data->year_of_birth);
        printf("Phone number: %s\n", data->phone_number);
        printf("Country: %s\n", data->country);
    }
}

int main(int argc, char const *argv[])
{

    int ret;
    struct human data = {0};

    strncpy(data.name, "blury", sizeof(data.name));                 
    strncpy(data.year_of_birth, "2001\n", sizeof(data.year_of_birth));
    strncpy(data.phone_number, "123456789\n", sizeof(data.phone_number));
    strncpy(data.country, "Bac Lieu\n", sizeof(data.country));

    if (ret = pthread_create(&thread_id1, NULL, &thr_handle, NULL)) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }


    if (ret = pthread_create(&thread_id2, NULL, &thr_handle, &data)) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);

    printf("All of threads done!\n");

    return 0;
}
