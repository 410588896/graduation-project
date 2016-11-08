#include <pthread.h>

#include "MemoryPool.h"
#include "Manager.h"
#include "Global.h"
#include "Timer.h"

//#define MEM
#define MYMEM

using namespace std;

TrashQue *TQue[THREAD_NUM];
Que *MemPool[THREAD_NUM];

VOID *worker(VOID *arg)
{
	INT tid = *(INT*)(arg);
	free(arg);
#ifdef MYMEM 
	Block block[1024];
	if(tid >=0 && tid <= 7)
	{
		for(INT i = 0; i < 192; ++i)
		{
			block[i].ptr = MemPool[tid - 1]->GetNode(block[i].tid);	
		}
		for(INT i = 0; i < 192; ++i)
		{
			MemPool[tid - 1]->ReturnNode(&block[i], tid);
		}
	}
	else
	{
		for(INT i = 0; i < 64; ++i)
		{
			block[i].ptr = MemPool[tid - 1]->GetNode(block[i].tid);	
		}
		for(INT i = 0; i < 64; ++i)
		{
			MemPool[tid - 1]->ReturnNode(&block[i], tid);
		}
	}
#endif

#ifdef MEM
	VOID *ptr[1024];
	if(tid >=0 && tid <= 7)
	{
		for(INT i = 0; i < 192; ++i)
		{
			ptr[i] = malloc(1024);
		}
		for(INT i = 0; i < 192; ++i)
		{
			free(ptr[i]);
		}
	}
	else
	{
		for(INT i = 0; i < 64; ++i)
		{
			ptr[i] = malloc(1024);
		}
		for(INT i = 0; i < 64; ++i)
		{
			free(ptr[i]);
		}
	}
#endif
	return NULL;
}

INT main()
{
	pthread_t mana;
	pthread_t tid[THREAD_NUM];
	INT *threadnum;
#ifdef MYMEM
	for(INT i = 0; i < THREAD_NUM; ++i)
	{
		TQue[i] = new TrashQue(128);
		MemPool[i] = new Que(i + 1, 128, 1024);
	}
	pthread_create(&mana, NULL, Manager, NULL);
#endif
	for(INT i = 0; i < THREAD_NUM; ++i)
	{
		threadnum = (INT*)malloc(sizeof(INT));
		*threadnum = (i + 1);
		pthread_create(&tid[i], NULL, worker, threadnum);
	}
#ifdef MYMEM
	pthread_join(mana, NULL);
#endif
	for(INT i = 0; i < THREAD_NUM; ++i)
		pthread_join(tid[i], NULL);
#ifdef MYMEM
	for(INT i = 0; i < THREAD_NUM; ++i)
	{
		delete TQue[i];
		delete MemPool[i];
	}
#endif
	return 0;
}
