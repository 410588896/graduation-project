#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#include "MemoryPool.h"
#include "Manager.h"
#include "Global.h"
#include "Timer.h"

//#define MEM
#define MYMEM
//#define TIME_TEST
#define GETTIME

using namespace std;

TrashQue *TQue[THREAD_NUM];
Que *MemPool[THREAD_NUM];

VOID *worker3(VOID *arg)
{
	INT tid = *(INT*)(arg);
	free(arg);
	Block block[1024];
	struct timeval tpstart,tpend; 
	float timeuse; 
	gettimeofday(&tpstart,NULL);
	for(INT i = 0; i < 10; ++i)
	{
		for(INT i = 0; i < 10; ++i)
		{
			block[i].ptr = MemPool[tid - 1]->GetNode(block[i].tid);	
		}
		for(INT i = 0; i < 10; ++i)
		{
			MemPool[tid - 1]->ReturnNode(&block[i], tid);
		}
		for(INT i = 0; i < 10; ++i)
		{
			block[i].tid = 0;	
		}
	}
	gettimeofday(&tpend, NULL); 
	timeuse = 1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
	timeuse /= 1000000; 
	printf("Used Time:%fs\n", timeuse);
	return NULL;
}

VOID *worker2(VOID *arg)
{
	VOID *ptr[1024];
	struct timeval tpstart,tpend; 
	float timeuse; 
	gettimeofday(&tpstart,NULL);
	for(INT i = 0; i < 10; ++i)
	{
		for(INT j = 0; j < 10; ++j)
			ptr[j] = malloc(1024);
		for(INT j = 0; j < 10; ++j)
			free(ptr[j]);
	}
	gettimeofday(&tpend, NULL); 
	timeuse = 1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
	timeuse /= 1000000; 
	printf("Used Time:%fs\n", timeuse);
	return NULL;
}

VOID *worker1(VOID *arg)
{
	INT tid = *(INT*)(arg);
	free(arg);
#ifdef MYMEM 
	Block block[1024];
#ifdef TIME_TEST
	struct CTick Tick;
	CTick_Construct(&Tick);
	Clear(&Tick);
	Start(&Tick);
#endif
#ifdef GETTIME
	struct timeval tpstart,tpend; 
	float timeuse; 
	gettimeofday(&tpstart,NULL); 
#endif
	for(INT k = 0; k < 1000; ++k)
	{
	if(tid >=1 && tid <= 8)
	{
		for(INT i = 0; i < 192; ++i)
		{
			block[i].ptr = MemPool[tid - 1]->GetNode(block[i].tid);	
		}
		for(INT i = 0; i < 192; ++i)
		{
			MemPool[tid - 1]->ReturnNode(&block[i], tid);
		}
		for(INT i = 0; i < 192; ++i)
		{
			block[i].tid = 0;	
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
		for(INT i = 0; i < 64; ++i)
		{
			block[i].tid = 0;	
		}
	}
	}
#ifdef GETTIME
	gettimeofday(&tpend, NULL); 
	timeuse = 1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
	timeuse /= 1000000; 
	printf("Used Time:%fs\n", timeuse); 
#endif
#ifdef TIME_TEST
	Final(&Tick);
	printf("build packet Call %d times, Cost %f us\n",Tick.Call, GetTime(&Tick));	
	printf("AVE Build Cost %f us\n",GetTime(&Tick)/Tick.Call);	
	printf("AVE Build Cost %f ms\n",(GetTime(&Tick)/Tick.Call)/1000);
#endif
#endif

#ifdef MEM
	VOID *ptr[1024];
#ifdef TIME_TEST
	struct CTick Tick;
	CTick_Construct(&Tick);
	Clear(&Tick);
	Start(&Tick);
#endif
#ifdef GETTIME
	struct timeval tpstart,tpend; 
	float timeuse; 
	gettimeofday(&tpstart,NULL); 
#endif
	for(INT k = 0; k < 1000; ++k)
	{
	if(tid >= 1 && tid <= 8)
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
	}
#ifdef GETTIME
	gettimeofday(&tpend, NULL); 
	timeuse = 1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
	timeuse /= 1000000; 
	printf("Used Time:%fs\n", timeuse); 
#endif
#ifdef TIME_TEST
	Final(&Tick);
	printf("build packet Call %d times, Cost %f us\n",Tick.Call, GetTime(&Tick));	
	printf("AVE Build Cost %f us\n",GetTime(&Tick)/Tick.Call);	
	printf("AVE Build Cost %f ms\n",(GetTime(&Tick)/Tick.Call)/1000);
#endif
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
		TQue[i] = new(nothrow) TrashQue(THREAD_NUM * 100);
		MemPool[i] = new(nothrow) Que(i + 1, THREAD_NUM * 100, 1024);
		if(TQue[i] == NULL || MemPool[i] == NULL)
		{
			printf("Mempool init error!\n");
			return -1;
		}
	}
	pthread_create(&mana, NULL, Manager, NULL);
#endif
	for(INT i = 0; i < THREAD_NUM; ++i)
	{
		threadnum = (INT*)malloc(sizeof(INT));
		*threadnum = (i + 1);
		pthread_create(&tid[i], NULL, worker3, threadnum);
	}
	for(INT i = 0; i < THREAD_NUM; ++i)
		pthread_join(tid[i], NULL);
#ifdef MYMEM
	pthread_join(mana, NULL);
#endif
#ifdef MYMEM
	for(INT i = 0; i < THREAD_NUM; ++i)
	{
		delete TQue[i];
		delete MemPool[i];
	}
#endif
	return 0;
}
