#ifndef LINKRANK_H
#define LINKRANK_H

#include <iostream>
#include <stdio.h>

#include "Type.h"

struct LRank
{
	BOOL LPT;
	BOOL LL;
	INT LI;
	INT LT;
	INT LWL;
	INT LCT;
	INT LOI;
	INT LSED;
};

BOOL LinkRank(const struct LRank &Link);

#endif

