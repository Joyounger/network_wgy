// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once


#include <valarray>
#include <iostream>
#include <iomanip>

#include <tchar.h>
#include <windows.h>
#include <time.h>
#define SEND 0xf0000000
#define ACK 0x0f000000
#define CONF 0x00f00000

extern DWORD dwBus;
// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
DWORD WINAPI HostB(LPVOID);
DWORD WINAPI HostA(LPVOID);
