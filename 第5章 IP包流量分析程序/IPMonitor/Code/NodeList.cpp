// NodeList.cpp: implementation of the CNodeList class.
//
//////////////////////////////////////////////////////////////////////

#include "NodeList.h"

// Default constructor
CNodeList::CNodeList()
{
	pHead = pTail = NULL;
}

// Default destructor
CNodeList::~CNodeList()
{
	// ɾ�����������н��
	if (pHead != NULL)
	{
		CIPNode * pTemp = pHead;
		pHead = pHead->pNext;
		delete pTemp;
	}
}

// ���²����IP���ݰ���������
void CNodeList::addNode(unsigned long dwSourIP,unsigned long dwDestIP,unsigned char chPro)
{
	if (pHead == NULL)	// �����
	{
		pTail = new CIPNode(dwSourIP,dwDestIP,chPro);
		pHead = pTail;
		pTail->pNext = NULL;
	}
	
	else	// ������ʱ
	{
		CIPNode * pTemp;
		for(pTemp = pHead; pTemp; pTemp = pTemp->pNext)
		{
			// ����������Ѵ��ڸ����͵�IP���������ݰ�������1
			if (pTemp->getSourIPAddr() == dwSourIP && 
				pTemp->getDestIPAddr() == dwDestIP && 
				pTemp->getProtocol() == chPro)
			{
				// ���ݰ�������1
				pTemp->addCount();
				break;
			}
		}
		// ��������в����ڸ����͵�IP�����򴴽��µĽ���������
		if (pTemp == NULL)
		{
			pTail->pNext = new CIPNode(dwSourIP,dwDestIP,chPro);
			pTail = pTail->pNext;
			pTail->pNext = NULL;
		}
	}
}

// �������
ostream& CNodeList::print(ostream & out)
{
	CIPNode * pTemp;
	if(pHead == NULL)
	{
		out << "û�в���IP���ݰ�!" << endl;
	}
	else
	{
		out << "Դ��ַ  " << '\t' << "Ŀ�ĵ�ַ" << '\t' << "Э������  " << "���ݰ�����" <<endl;
		for(pTemp = pHead; pTemp; pTemp = pTemp->pNext)
		{
			unsigned long dwSourTemp = pTemp->getSourIPAddr();
			unsigned long dwDestTemp = pTemp->getDestIPAddr();
			out << inet_ntoa(*(in_addr *)&(dwSourTemp)) << '\t';
			out << inet_ntoa(*(in_addr *)&(dwDestTemp)) << '\t';
			out << resetiosflags(ios::right) << setiosflags(ios::left)
				<< setfill(' ') << setw(10) << pTemp->getProtocol_String()
				<< resetiosflags(ios::left) << setiosflags(ios::right);
			out << pTemp->getCount() << endl;
		}
	}
	return out;
}
