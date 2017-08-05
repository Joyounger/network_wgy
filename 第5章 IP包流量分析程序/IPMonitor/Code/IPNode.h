// IPNode.h: interface for the CIPNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPNODE_H__1366A568_424A_4BDF_8E76_9AF5BA10D449__INCLUDED_)
#define AFX_IPNODE_H__1366A568_424A_4BDF_8E76_9AF5BA10D449__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// �����CIPNode
class CIPNode  
{
private:
	unsigned long m_dwSourIPAddr;		// ԴIP��ַ
	unsigned long m_dwDestIPAddr;		// Ŀ��IP��ַ
	unsigned char m_chProtocol;			// IP����Э������
	unsigned long m_dwCouter;			// ���ݰ�������

public:
	
	CIPNode * pNext;			// ָ����һ��IP���

	CIPNode();
	virtual ~CIPNode();

	// ���캯��
	CIPNode(unsigned long, unsigned long, unsigned char);

	// �������ݰ�������
	void addCount();
	// ȡ�����ݰ�����
	unsigned long getCount();	
	// ȡ��ԴIP��ַ
	unsigned long getSourIPAddr();	
	// ȡ��Ŀ��IP��ַ
	unsigned long getDestIPAddr();
	// ȡ��Э������
	unsigned char getProtocol();
	// ȡ��Э������(TCP,UDP,ICMP...)
	char * getProtocol_String();
};

#endif // !defined(AFX_IPNODE_H__1366A568_424A_4BDF_8E76_9AF5BA10D449__INCLUDED_)
