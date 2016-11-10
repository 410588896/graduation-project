// CMyMemPool.cpp : 定义控制台应用程序的入口点。
//
#include <sys/time.h>
#include <cstdio>

#include "MemPool.h"

#define DATA_BLOCK_LEN 1024
#define THREAD_NUM 16

void *worker(void *arg)
{
	CMemPool *mem = (CMemPool*)arg;
	void *ptr[1024];
	struct timeval tpstart,tpend; 
	float timeuse; 
	gettimeofday(&tpstart,NULL);
	for(int i = 0; i < 10; ++i)
	{
		for(int i = 0; i < 10; ++i)
			ptr[i] = mem->Get();
		for(int i = 0; i < 10; ++i)
			mem->Release(ptr[i]);
	}
	gettimeofday(&tpend, NULL); 
	timeuse = 1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
	timeuse /= 1000000; 
	printf("Used Time:%fs\n", timeuse);
	return NULL;
}


int main(int argc, char* argv[])
{
	CMemPool myPool(DATA_BLOCK_LEN, 0, 1600);
	pthread_t tid[THREAD_NUM];
	
	for(int i = 0; i < THREAD_NUM; ++i)
		pthread_create(&tid[i], NULL, worker, &myPool);

	for(int i = 0 ; i < THREAD_NUM; ++i)
		pthread_join(tid[i], NULL);

	return 0;
}
