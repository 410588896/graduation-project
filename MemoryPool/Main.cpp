#include <pthread.h>

#include "MemoryPool.h"
#include "Manager.h"
#include "Global.h"

using namespace std;

TrashQue *TQue[THREAD_NUM];
Que *MemPool[THREAD_NUM];

VOID *worker(VOID *arg)
{
	INT tid = *(INT*)(arg);
	free(arg);
	Block block;
	block.ptr = MemPool[tid - 1]->GetNode(block.tid);
	while(1);
}

INT main()
{
	pthread_t mana;
	pthread_t tid[THREAD_NUM];
	INT *threadnum;
	for(INT i = 0; i < THREAD_NUM; ++i)
	{
		TQue[i] = new TrashQue(128);
		MemPool[i] = new Que(i + 1, 128, 1024);
	}
	pthread_create(&mana, NULL, Manager, NULL);
	
	for(INT i = 0; i < THREAD_NUM; ++i)
	{
		threadnum = (INT*)malloc(sizeof(INT));
		*threadnum = (i + 1);
		pthread_create(&tid[i], NULL, worker, threadnum);
	}
	pthread_join(mana, NULL);
	for(INT i = 0; i < THREAD_NUM; ++i)
		pthread_join(tid[i], NULL);
	for(INT i = 0; i < THREAD_NUM; ++i)
	{
		delete TQue[i];
		delete MemPool[i];
	}
	return 0;
}
