#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Type.h"
#include "Global.h"

struct Block
{
	Block() : ptr(NULL), tid(0) {};
	Block(UINT size) : tid(0) {ptr = (VOID*)new CHAR(size);};
	~Block() { if(ptr) delete (CHAR*)ptr; };
	VOID *ptr;
	UINT tid;
};

class Que
{
	public:
		Que(UINT _tid, UINT num = 128, UINT size = 1024) : Num(num), Free(num), Size(size) { InitMemoryPool(num, size); SetTid(_tid); };
		~Que();
		VOID SetTid(UINT _tid);
		VOID InitMemoryPool(UINT num, UINT size);
		VOID *GetNode(UINT &tid); //alloc memory from private que
		VOID ReturnNode(Block *node, UINT _tid); //return node to private que

		Block **Chunk;
		UINT Head;
		UINT Tail;
		UINT Size;
		UINT Num;
		UINT Free;
};

struct Node
{
	Node() { ptr = NULL; tid = 0; };
	VOID *ptr;
	UINT tid;
};

class TrashQue
{
	private:
		Node *Chunk;
		UINT Front;
		UINT Rear;
		UINT Maxsize;
	public:
		TrashQue(UINT num) : Front(0), Rear(0), Maxsize(num) { Chunk = (Node*)malloc(sizeof(Node) * num); };
		~TrashQue() { if(Chunk) free(Chunk); Chunk = NULL; };
		VOID Enque(VOID *ptr, UINT _tid);
		BOOL Deque(VOID **ptr, UINT &_tid);
		BOOL IsFull();
		BOOL IsEmpty();
};

VOID ReturnToTrash(Block *node, UINT _tid);

#endif

