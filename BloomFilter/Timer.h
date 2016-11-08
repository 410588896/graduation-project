#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdio.h>

#include <stdlib.h>

#include <memory.h>

#include <math.h>

#define LINUX

#ifdef  WIN32

#include <windows.h>

#include <intrin.h>

#pragma intrinsic(__rdtsc)

#endif//WIN32

//�� �� ��*********************************************************************

#ifdef  LINUX

#define VOID    void    //�� �� ��

#endif//LINUX

//���Ͷ���*********************************************************************

#ifdef  WIN32

typedef short           SHRT;

typedef int             SINT;

typedef __int64         LINT;

#endif//WIN32

#ifdef  LINUX

//typedef char            CHAR;

//typedef unsigned char   BYTE;

//typedef short           SHRT;

//typedef unsigned short  WORD;

//typedef int             SINT;

//typedef unsigned int    UINT;

//typedef long            LONG;

//typedef long long       LINT;

#endif//LINUX

typedef struct CTick{

    UINT    Call;   //��������

    LINT    Time;   //����ʱ��

    LINT    Prev;   //��ʼʱ��

    LINT    Post;   //ֹͣʱ��

    LINT    Ring;   //ʱ��Ƶ��

    LINT    Cost;   //��ת����

}CTick;

LINT Query()
{
	//��������

        LINT nRetCode = 0; CHAR Buffer[32];

        LONG nErrCode = 0; FILE *hFile = 0;

        //��ѯƵ��

        nErrCode = system("cat /proc/cpuinfo | grep \"cpu MHz\" | awk -F: '{print $2}' > /tmp/cpufreq.txt");

        if(nErrCode == -1) { return -1; }

        //��ȡƵ��

        hFile = fopen("/tmp/cpufreq.txt", "r");

        if(hFile == NULL) { return -2; }

        if(fgets(Buffer, sizeof(Buffer), hFile) == NULL)
        {
            fclose(hFile); return -3;
        }

        fclose(hFile);

        nRetCode = (LINT)(atof(Buffer) * pow(1024.00, 2.00));

        //�����ļ�

        nErrCode = system("rm -f /tmp/cpufreq.txt");

        //����Ƶ��

        return nRetCode;
}

LINT Fetch()
{
        //��������

        LINT nRetCode = 0;

        //��ѯ����

        asm volatile("rdtsc" : "=a" (*((UINT*)(&nRetCode) + 0)),

                               "=d" (*((UINT*)(&nRetCode) + 1)));
        //���ؼ���

        return nRetCode;
}

void Clear(CTick *Tick)
{
	Tick -> Call = 0;
	
	Tick -> Time = 0;
}


void CTick_Construct(CTick *Tick)
{
	int nIndex;

	Tick -> Ring = Query();

	Tick -> Cost = Query();

	for(nIndex = 0; nIndex < 80; nIndex += 1)
	{
		Tick -> Prev = Fetch();

		Tick -> Post = Fetch();

		Tick -> Cost = Tick -> Cost < (Tick -> Post - Tick -> Prev)
				
					 ? Tick -> Cost : (Tick -> Post - Tick -> Prev);	
	}
	
	Clear(Tick);
}


void Start(CTick *Tick)
{
	Tick -> Prev = Fetch();
}

void Final(CTick *Tick)
{	
	Tick -> Post = Fetch();

	Tick -> Call += 1;

	Tick -> Time += ((Tick -> Post - Tick -> Prev) > Tick -> Cost ?

					 (Tick -> Post - Tick -> Prev - Tick -> Cost) : 0);
}

double GetTime(CTick *Tick)
{
	return Tick -> Time * 1000000.00 / Tick -> Ring;
}

#endif
