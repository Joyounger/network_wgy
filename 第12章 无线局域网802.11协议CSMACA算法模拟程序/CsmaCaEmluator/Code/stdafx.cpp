// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// CsmaCaEmluator.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
//�����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������

	
DWORD dwBus;

DWORD WINAPI HostA(LPVOID)
{
	int i=0;
	int nSendTimes=0;
	while (i!=10)
	{
Wait:
		while(dwBus!=0)
		{
			Sleep(100);
		}
		Sleep(rand()%100);
		if(dwBus!=0)
		{
			goto Wait;
		}
		dwBus=SEND +i;
		Sleep(1000);
		if(dwBus==ACK+i)
		{
			printf("���͵�%d�����ݰ��ɹ���\r\n",i+1);
			i++;
			nSendTimes=0;
			Sleep(1000);
		}
		else
		{
			nSendTimes++;
			if(nSendTimes<10)
			{
				printf("δ���յ���%d�����ݰ���ACK��Ӧ����%d�����·��͡�\r\n",i+1,nSendTimes);
				int nSleepTime=(rand()%3)*(int)pow(2,nSendTimes);
			
				Sleep(nSleepTime*50);//����ȴ�һ��ʱ����������
			}
			else
			{
				printf("�������͵�%d�����ݰ���\r\n",i+1);
				i++;
			}
		}
	}
	return 0;
}
DWORD WINAPI HostB(LPVOID)
{
	do
	{
		while((dwBus&0xfffffff0)!=SEND)
		{
			Sleep(10);
		}
		dwBus=ACK+(dwBus&0x0000000f);
	}while(dwBus!=SEND+9);
	return 0;
}
