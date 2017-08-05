// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D82421A2_CB27_4352_AE1B_69E5E3335D8B__INCLUDED_)
#define AFX_STDAFX_H__D82421A2_CB27_4352_AE1B_69E5E3335D8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <stdlib.h>
#include <stdio.h>

#include <pcap.h>

#include <iostream>

using namespace std;
#define BROADMAC		{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF} //�㲥MAC
#define EH_TYPE			0x0806							//ARP����
#define ARP_HRD			0X0001							//Ӳ�����ͣ���̫���ӿ�����Ϊ1		
#define ARP_PRO			0x0800							//Э�����ͣ�IPЭ������Ϊ0X0800
#define ARP_HLN			0x06							//Ӳ����ַ���ȣ�MAC��ַ����Ϊ6B
#define ARP_PLN			0x04							//Э���ַ���ȣ�IP��ַ����Ϊ4B
#define ARP_REQUEST		0x0001							//������ARP����Ϊ1
#define ARP_REPLY		0x0002							//������ARPӦ��Ϊ2
#define ARP_THA			{0,0,0,0,0,0}					//Ŀ��MAC��ַ��ARP�����и��ֶ�û�����壬��Ϊ0��ARP��Ӧ��Ϊ���շ���MAC��ַ
#define ARP_PAD			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define SPECIAL			0x70707070						//�������Լ�MAC��ַ������ԴIP��112.112.112.112

#define ETH_HRD_DEFAULT	{BROADMAC, {0,0,0,0,0,0}, htons(EH_TYPE)}
#define ARP_HRD_DEFAULT	{htons(ARP_HRD), htons(ARP_PRO), ARP_HLN, ARP_PLN, htons(ARP_REQUEST), {0,0,0,0,0,0}, 0, ARP_THA, 0, ARP_PAD}

#define IPTOSBUFFERS 12

#define WM_PACKET	WM_USER + 105	
struct ethernet_head
{
	unsigned char dest_mac[6];									//Ŀ������MAC��ַ
	unsigned char source_mac[6];								//Դ��MAC��ַ
	unsigned short eh_type;										//��̫������
};

struct arp_head
{
	unsigned short hardware_type;								//Ӳ�����ͣ���̫���ӿ�����Ϊ1
	unsigned short protocol_type;								//Э�����ͣ�IPЭ������Ϊ0X0800
	unsigned char add_len;										//Ӳ����ַ���ȣ�MAC��ַ����Ϊ6B
	unsigned char pro_len;										//Э���ַ���ȣ�IP��ַ����Ϊ4B
	unsigned short option;										//������ARP����Ϊ1��ARPӦ��Ϊ2

	unsigned char sour_addr[6];									//ԴMAC��ַ�����ͷ���MAC��ַ
	unsigned long sour_ip;										//ԴIP��ַ�����ͷ���IP��ַ
	unsigned char dest_addr[6];									//Ŀ��MAC��ַ��ARP�����и��ֶ�û�����壻ARP��Ӧ��Ϊ���շ���MAC��ַ
	unsigned long dest_ip;										//Ŀ��IP��ַ��ARP������Ϊ���������IP��ַ��ARP��Ӧ��Ϊ���շ���IP��ַ
	unsigned char padding[18];
};

struct arp_packet										//����arp���ṹ
{
	ethernet_head eth;									//��̫��ͷ��
	arp_head arp;										//arp���ݰ�ͷ��
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D82421A2_CB27_4352_AE1B_69E5E3335D8B__INCLUDED_)
