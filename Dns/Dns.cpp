#include<iostream>
#include <netdb.h>
#include <sys/time.h>
#include <cstdio>
#include <cstring>

#include "Type.h"
#include "Hash.h"

using namespace std;

const CHAR host[1024][128] = {
	"www.baidu.com","www.qq.com","www.sina.com.cn","www.taobao.com","www.tmall.com",
	"www.hrbeu.edu.cn","www.souhu.com","www.apple.com.cn","www.163.com", "www.360.com"
};

INT main()
{
	struct hostent *hptr;
	struct timeval tpstart,tpend; 
	float timeuse; 
	gettimeofday(&tpstart,NULL);
	for(INT i = 0; i < 10; ++i)
	{
		//gethostbyname(host[i]);
		BKDRHash(host[0], strlen(host[0]));
		JSHash(host[0], strlen(host[0]));
		RSHash(host[0], strlen(host[0]));
		for(INT j = 0; j < 50; j++)
		{
			strcmp("www.baidu.com", "www.sina.com.cn");
		}
	}
	gettimeofday(&tpend, NULL); 
	timeuse = 1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
	timeuse /= 1000000; 
	printf("Used Time:%fs\n", timeuse);
	return 0;
}
