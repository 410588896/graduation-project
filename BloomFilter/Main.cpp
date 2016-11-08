#include <iostream>

#include "BloomSet.h"
#include "Old_BF.h"
#include "Timer.h"

#define DBG 1

#if DBG
#include <cstdio>
#endif
using namespace std;
      
      
int main(){
#if DBG
    freopen("data.txt","r", stdin);
#endif
	CHAR buf[1024] = {0};
	BF bf(40000000);
	BloomSet Bf(40000000, 0.5);
	LONG old;
	LONG brand = 0;
	while(cin.getline(buf,sizeof(buf)))
    {
		if(bf.checkBit(buf, strlen(buf)))
			old++;
		else
			bf.setBit(buf, strlen(buf));
		if(Bf.checkBit(buf, strlen(buf)))
			brand++;
		memset(buf, 0, 1024);
    }
	printf("OLD:%d, NEW:%d\n", old, brand);
    return 0;
}
