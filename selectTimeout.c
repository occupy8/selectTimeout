#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/eventfd.h>

int sock;
#define MAX_TIME  (10)

static int mysleep(int sockfd, int nsec)
{
        uint64_t  u;
        int n;
        fd_set rset;
        struct timeval tval;
	//int sock = eventfd(1, 0);

	printf("sockfd :%d\n", sockfd);
        FD_ZERO(&rset);
        FD_SET(sockfd, &rset);
        tval.tv_sec = nsec;
        tval.tv_usec = 0;

        if ((n = select(sockfd+1, &rset, NULL, NULL, &tval)) == 0) {
                return 0;
        }

        if (FD_ISSET(sockfd, &rset)) {
		printf("something data can read!");
                read(sockfd, &u, sizeof(u));
        }

        FD_ZERO(&rset);

        return (0);
}

void *sleepTest(void *data){

	int fd = *((int*)data);

	time_t now = time(NULL);
	printf("sleep before= %d\n", now);
	mysleep(fd, MAX_TIME);
	now = time(NULL);
	printf("sleep after= %d\n", now);

	return NULL;
}

int main(void){
	sock = eventfd(0, 0);
	if(sock == -1)
		return 0;

	printf("create sock :%d\n", sock);

	pthread_t thid;
	thid = pthread_create(&thid, NULL, sleepTest, &sock);

	sleep(3);
	uint64_t u = 1;
	write(sock, &u, sizeof(u));


	while(1)
		sleep(2);

	close(sock);

	return 0;
}
