#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>

#define THREAD_NUM 16

using namespace std;

void *worker(void* arg)
{
	void *ptr[1024];
	struct timeval tpstart,tpend; 
	float timeuse; 
	gettimeofday(&tpstart,NULL);
	for(int i = 0; i < 1000; ++i)
	{
		for(int j = 0; j < 1000; ++j)
			ptr[j] = malloc(1024);
		for(int j = 0; j < 1000; ++j)
			free(ptr[j]);
	}
	gettimeofday(&tpend, NULL); 
	timeuse = 1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
	timeuse /= 1000000; 
	printf("Used Time:%fs\n", timeuse);
	return NULL;
}

int main()
{
	pthread_t tid[THREAD_NUM];
	for(int i = 0; i < THREAD_NUM; ++i)
	{
		pthread_create(&tid[i], NULL, worker, NULL);
	}
	for(int i = 0; i < THREAD_NUM; ++i)
		pthread_join(tid[i], NULL);
	return 0;
}

