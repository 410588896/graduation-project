#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Hash.h"
#include "Type.h"

class BloomFilter
{
    public:
        BloomFilter(INT size, double threshold) : SIZE(size), Threshold(threshold), Numof1(0) { vec.resize(size); };
        VOID setBit(const CHAR *str, INT len);
        VOID setBit(UINT count);

        BOOL checkBit(const CHAR *str, INT len);
        BOOL checkBit(UINT count);
    private:
        std::vector<CHAR> vec;
        const UINT SIZE;  //bloomfilter size
		UINT Numof1;  //the number of 1
		double Threshold;
};

#endif

