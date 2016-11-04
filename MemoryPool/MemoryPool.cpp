#include "MemoryPool.h"

extern TrashQue TQue[THREAD_NUM];

Que::~Que()
{
	for(INT i = 0; i < Num; ++i)
		delete Chunk[i];
	delete []Chunk;
	Chunk = NULL;
	Head = 0;
	Tail = 0;
}

VOID Que::SetTid(UINT _tid)
{
	for(INT i = 0; i < Num; ++i)
		Chunk[i]->tid = _tid;
	return;
}

VOID Que::InitMemoryPool(UINT num, UINT size)
{
	Chunk = new Block* [num];
	for(INT i = 0; i < num; ++i)
		Chunk[i] = new Block(size);
	Head = num - 1;
	Tail = 0;
	return;
}

VOID* Que::GetNode(UINT &tid)
{
	if((Free > 0) && (Head != Tail))
	{
		//--Free;
		__sync_sub_and_fetch (&Free, 1);  //--Free
		tid = Chunk[Head]->tid;
		INT tmp = Head;
		if(Head != 0)
			--Head;
		else
			Head = Num - 1; 
		return Chunk[tmp]->ptr;
	}
	else
	{
		tid = 0;
		return (VOID*)new CHAR(Size);
	}
	return NULL;
}

VOID Que::ReturnNode(Block *node, UINT _tid)
{
	if(node->tid == 0)
		delete (CHAR*)node->ptr;
	else if(_tid == node->tid)
	{
		if((Head + 1) % Num != Tail)
		{
			Head = (Head + 1) % Num; 
			Chunk[Head]->ptr = node->ptr;
			Chunk[Head]->tid = node->tid;
			__sync_add_and_fetch (&Free, 1);  //--Free
		}
		else
			ReturnToTrash(node, _tid);
	}
	else
		ReturnToTrash(node, _tid);	
	return;
}

VOID TrashQue::Enque(VOID *ptr, UINT _tid)
{
	if(!IsFull())
	{
		Chunk[Rear].ptr = ptr;
		Chunk[Rear].tid = _tid;
		Rear = (Rear + 1) % Maxsize;
	}
	else
	{
		Maxsize = Maxsize + 16; 
		Chunk = (Node*)realloc(Chunk, Maxsize * sizeof(Node));
		if(Chunk == NULL)	
		{
			std::cout<<"Queue is full and can not expand!"<<std::endl;
			exit(-1);
		}
	}
}

BOOL TrashQue::Deque(VOID **ptr, UINT &_tid)
{
	if(!IsEmpty())
	{
		*ptr = Chunk[Front].ptr;
		_tid = Chunk[Front].tid;
		Front = (Front + 1) % Maxsize;
		return true;
	}		
	else
	{
#ifdef DEBUG
		std::cout<<"The queue is empty!"<<std::endl;
#endif
		return false;
	}
}

BOOL TrashQue::IsEmpty()
{
	if(Rear == Front)
		return true;
	else 
		return false;
}

BOOL TrashQue::IsFull()
{
	if((Rear + 1) % Maxsize == Front)
		return true;
	else
		return false;
}

VOID ReturnToTrash(Block *node, UINT _tid)
{
	TQue[_tid - 1].Enque(node->ptr, node->tid);
	return;
}

