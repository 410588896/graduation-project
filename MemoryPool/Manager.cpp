#include "Manager.h"

extern TrashQue *TQue[THREAD_NUM];
extern Que *MemPool[THREAD_NUM];

VOID ReAlloc(UINT stid, UINT dtid)
{

}

VOID *Manager(VOID *arg)
{
	while(true)
	{
		for(INT i = 0; i < THREAD_NUM; ++i)	
		{
			for(INT j = 0; j < THREAD_NUM; ++j)
			{
				while((!(TQue[i]->IsEmpty()))&&(MemPool[j]->Free < (MemPool[j]->Num >> 1)))
				{
					VOID *ptr = NULL;
					UINT _tid = 0;
					TQue[i]->Deque(&ptr, _tid);
					//wei cha
					MemPool[_tid]->Chunk[--(MemPool[_tid]->Tail)]->ptr = ptr;
					MemPool[_tid]->Chunk[(MemPool[_tid]->Tail)]->tid = _tid;
				}
			}
		}
		for(INT i = 0; i < THREAD_NUM; ++i)
		{

		}
	}
	return NULL;
}
