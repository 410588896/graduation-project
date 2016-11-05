#include "Manager.h"

extern TrashQue *TQue[THREAD_NUM];
extern Que *MemPool[THREAD_NUM];

VOID ReAlloc(UINT stid, UINT dtid)
{
	VOID *ptr = NULL;
	UINT _tid = 0;
	ptr = MemPool[stid]->Chunk[MemPool[stid]->Tail]->ptr; 
	_tid = MemPool[stid]->Chunk[(MemPool[stid]->Tail)]->tid;
	MemPool[stid]->Tail = (MemPool[stid]->Tail + 1) % MemPool[stid]->Num;
	__sync_sub_and_fetch (&(MemPool[stid]->Free), 1);  
	if(MemPool[dtid]->Tail == 0)
		MemPool[dtid]->Tail = MemPool[dtid]->Num - 1;
	else
		--(MemPool[dtid]->Tail);
	MemPool[dtid]->Chunk[(MemPool[dtid]->Tail)]->ptr = ptr;
	MemPool[dtid]->Chunk[(MemPool[dtid]->Tail)]->tid = _tid;
	__sync_add_and_fetch (&(MemPool[dtid]->Free), 1);  
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
			//alloc trategy
			INT small[THREAD_NUM] = {0};
			INT big[THREAD_NUM] = {0};
			INT smallnum = 0;
			INT bignum = 0;
			INT more = 0;
			INT smallest = MemPool[0]->Free;
			INT less = 0;
			for(INT i = 0; i < THREAD_NUM; ++i)
			{
				if(MemPool[i]->Free < (MemPool[i]->Num >> 1))
				{
					if(smallest > MemPool[i]->Free)
					{
						less = i;
						smallest = MemPool[i]->Free;
					}
					small[smallnum] = i;
					++smallnum;
				}
				if(MemPool[i]->Free > MemPool[i]->Num >> 1)
				{
					big[bignum] = i;
					more += (MemPool[i]->Num >> 1) - MemPool[i]->Free;
					++bignum;
				}
				if(smallnum != 0 && bignum != 0)
				{
					if(more < smallnum)
					{
						INT tmp = 0;
						while(tmp <= bignum)
						{
							while(MemPool[big[tmp]]->Free > (MemPool[big[tmp]]->Num >> 1))
								ReAlloc(big[tmp], less);
							++tmp;
						}
					}
					else
					{
						INT avg = more / smallnum;
						for(INT j = 0; j < smallnum; ++i)
						{
							if(small[j] == less)
							{
								INT tmp = 0;
								while(tmp < (avg + (more % smallnum)))
								{
									while(MemPool[big[tmp]]->Free > (MemPool[big[tmp]]->Num >> 1))
										ReAlloc(big[tmp], less);
									++tmp;
								}
							}
							else
							{
								INT tmp = 0;
								while(tmp < avg)
								{
									while(MemPool[big[tmp]]->Free > (MemPool[big[tmp]]->Num >> 1))
										ReAlloc(big[tmp], small[j]);
									++tmp;
								}
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}
