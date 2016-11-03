#ifndef BLOOMSET_H
#define BLOOMSET_H

#include <string.h>

#include "BloomFilter.h"

struct BloomSet
{
		BloomFilter *Host;
		BloomFilter *Path;
		BloomFilter *FName;
		BloomSet(INT size, double threshold):Count(1)
		{
			Host = new BloomFilter(size, threshold);
			Path = new BloomFilter(size, threshold);
			FName = new BloomFilter(size, threshold);
		}
		~BloomSet()
		{
			delete Host;
			delete Path;
			delete FName;
		}
		BOOL checkBit(const CHAR *str, INT len);
	private:
		UINT Count;
};

#endif

