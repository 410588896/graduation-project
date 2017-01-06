#include "LinkRank.h"

BOOL LinkRank(const struct LRank &Link)
{
	if(Link.LPT && Link.LL)
	{
		INT sum = Link.LI + Link.LT + Link.LWL + Link.LCT + Link.LOI + Link.LSED;
		if(sum >= 3)
			return true;
		else
			return false;
	}
	else
		return false;
}
