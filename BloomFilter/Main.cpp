#include <iostream>
#include <sys/time.h>

#include "BloomSet.h"
#include "Old_BF.h"
#include "Timer.h"

//#define DBG 1
//#define TIME_TEST

#if DBG
#include <cstdio>
#endif
using namespace std;
      
      
int main(){
#if DBG
  //  freopen("data.txt","r", stdin);
#endif
	CHAR buf[1024] = {0};
	BF bf(250000);
	BloomSet Bf(59524, 0.5);
	INT old = 0;
	INT brand = 0;
	FILE *fp = fopen("data220000.txt", "r");
#ifdef TIME_TEST
	struct timeval tpstart,tpend; 
	float timeuse;
#endif
	struct timeval tpstart,tpend; 
	float timeuse;
	gettimeofday(&tpstart, NULL); 
	while(fgets(buf,sizeof(buf), fp) != NULL)
    {
#ifdef TIME_TEST
		gettimeofday(&tpstart,NULL);
#endif
//		if(bf.checkBit(buf, strlen(buf)) == false)
//		{
//			bf.setBit(buf, strlen(buf));
//			old++;
//		}
#ifdef TIME_TEST
		gettimeofday(&tpend, NULL); 
		timeuse = 1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
		timeuse /= 1000000; 
		printf("Used Time:%fs\n", timeuse);
#endif
#ifdef TIME_TEST
		gettimeofday(&tpstart,NULL);
#endif
		if(Bf.checkBit(buf, strlen(buf)) == false)
			brand++;
#ifdef TIME_TEST
		gettimeofday(&tpend, NULL); 
		timeuse = 1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
		timeuse /= 1000000; 
		printf("Used Time:%fs\n", timeuse);
#endif
		memset(buf, 0, 1024);
    }
	gettimeofday(&tpend, NULL); 
	timeuse = 1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
	timeuse /= 1000000; 
	printf("Used Time:%fs\n", timeuse);
	printf("OLD:%d, NEW:%d\n", old, brand);
    return 0;
}
