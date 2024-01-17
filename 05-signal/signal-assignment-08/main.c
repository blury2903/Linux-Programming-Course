#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
 
void sig_handler1(int sig_num)
{
	printf("\nIm signal handler1: %d\n", sig_num);
}

void sig_handler2(int sig_num)
{
	printf("Im signal handler2: %d\n", sig_num);
}

void sig_handler3(int sig_num)
{
    printf("Im signal handler3: %d\n", sig_num);
}
 
int main()
{
  	if (signal(SIGINT, sig_handler1) == SIG_ERR) {
		fprintf(stderr, "Cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	}

    if (signal(SIGUSR1, sig_handler2) == SIG_ERR) {
		fprintf(stderr, "Cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	}

    if (signal(SIGUSR2, sig_handler3) == SIG_ERR) {
		fprintf(stderr, "Cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	}

 	sleep(4);
	kill(getpid(), SIGINT);

	printf("\nprocess ID: %d\n", getpid());
	while (1)
	{

	}
    return 0;
}
