// IPNode.cpp: implementation of the CIPNode class.
//
//////////////////////////////////////////////////////////////////////

#include "IPNode.h"

// Default constructor
CIPNode::CIPNode()
{

}

// Default destructor
CIPNode::~CIPNode()
{

}

CIPNode::CIPNode(unsigned long dwSourIP,unsigned long dwDestIP,unsigned char chPro)
{
	m_dwSourIPAddr = dwSourIP;
	m_dwDestIPAddr = dwDestIP;
	m_chProtocol = chPro;
	m_dwCouter = 1;			// ��ʼ�����ݰ�����Ϊ1
}

// �������ݰ�������
void CIPNode::addCount()
{
	m_dwCouter++;
}

// ȡ�����ݰ�����
unsigned long CIPNode::getCount()
{
	return m_dwCouter;
}

// ȡ��ԴIP��ַ
unsigned long CIPNode::getSourIPAddr()
{
	return m_dwSourIPAddr;
}

// ȡ��Ŀ��IP��ַ
unsigned long CIPNode::getDestIPAddr()
{
	return m_dwDestIPAddr;
}

// ȡ��Э������
unsigned char CIPNode::getProtocol()
{
	return m_chProtocol;
}

// ȡ��Э������
char * CIPNode::getProtocol_String()
{
	switch(m_chProtocol)
	{
	case 1:
		return "ICMP";
		break;
	case 2:
		return "IGMP";
		break;
	case 4:
		return "IP in IP";
		break;
	case 6:
		return "TCP";
		break;
	case 8:
		return "EGP";
		break;
	case 17:
		return "UDP";
		break;
	case 41:
		return "IPv6";
		break;
	case 46:
		return "RSVP";
		break;
	case 89:
		return "OSPF";
		break;
	default:
		return "UNKNOWN";
	}
}
