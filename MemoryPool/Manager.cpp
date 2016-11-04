#include "Manager.h"

extern TrashQue *TQue[THREAD_NUM];
extern Que *MemPool[THREAD_NUM];

VOID ReAlloc(UINT stid, UINT dtid)
{
	VOID *ptr = NULL;
	UINT _tid = 0;
	ptr = MemPool[stid - 1]->Chunk[MemPool[stid - 1]->Tail]->ptr; 
	_tid = MemPool[stid - 1]->Chunk[(MemPool[stid - 1]->Tail)]->tid;
	MemPool[stid - 1]->Tail = (MemPool[stid - 1]->Tail + 1) % MemPool[stid - 1]->Num;
	__sync_sub_and_fetch (&(MemPool[stid - 1]->Free), 1);  
	if(MemPool[dtid - 1]->Tail == 0)
		MemPool[dtid - 1]->Tail = MemPool[dtid - 1]->Num - 1;
	else
		--(MemPool[dtid - 1]->Tail);
	MemPool[dtid - 1]->Chunk[(MemPool[dtid - 1]->Tail)]->ptr = ptr;
	MemPool[dtid - 1]->Chunk[(MemPool[dtid - 1]->Tail)]->tid = _tid;
	__sync_add_and_fetch (&(MemPool[dtid - 1]->Free), 1);  
	return;
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
					if(MemPool[j]->Tail == 0)
						MemPool[j]->Tail = MemPool[j]->Num - 1;
					else
						--(MemPool[j]->Tail);
					MemPool[j]->Chunk[(MemPool[j]->Tail)]->ptr = ptr;
					MemPool[j]->Chunk[(MemPool[j]->Tail)]->tid = _tid;
					__sync_add_and_fetch (&(MemPool[j]->Free), 1);  
				}
			}
		}
		for(INT i = 0; i < THREAD_NUM; ++i)
		{
			
		}
	}
	return NULL;
}
