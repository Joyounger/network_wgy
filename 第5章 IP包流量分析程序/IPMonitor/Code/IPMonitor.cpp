#include "IPMonitor.h"

#define BURRER_SIZE 65535

void main(int argc,char * argv[])
{
	// �ж�����������и�ʽ�Ƿ���ȷ
	if (argc != 2)
	{
		cout << "�밴���¸�ʽ����������: IPMonitor duration_time"
			<< endl << "    ����duration_timeΪ���ʱ��, ��λΪ��"<<endl;
		return;
	}

	WSADATA wsData;

	// ��ʼ��Winsock DLL
	if (WSAStartup(MAKEWORD(2,2),&wsData) != 0)
	{
		cout << "WSAstartup failed!" << endl;
		return;
	}

	// ����Raw Socket
	SOCKET sock;
	if ( (sock = WSASocket(AF_INET, SOCK_RAW, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED)) 
		== INVALID_SOCKET )
	{
		cout << "Create socket failed!" << endl;
		return;
	}

	// ����IPͷ����ѡ���ʾ�û��������Զ�IPͷ���д���
	BOOL bFlag = TRUE;
	if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *)&bFlag, sizeof(bFlag)) == SOCKET_ERROR)
	{
		cout << "Setsockopt failed!" << endl;
		return;
	}

	// ��ȡ����������
	char pHostName[128];
	if (gethostname(pHostName, 100) == SOCKET_ERROR)
	{
		cout << "Gethostname failed!" << endl;
		return;
	}

	// ͨ��������������ȡ����IP��ַ
	hostent * pHostIP;
	if((pHostIP = gethostbyname(pHostName)) == NULL)
	{
		cout<<"Gethostbyname failed!"<<endl;
		return;
	}

	// ���sockaddr_in�ṹ
	sockaddr_in addr_in;
	addr_in.sin_addr = *(in_addr *)pHostIP->h_addr_list[0];		 // �趨IP��ַ
	addr_in.sin_family = AF_INET;			// �趨��ַ����
	addr_in.sin_port = htons(8000);			// �趨�˿�

	// ��ԭʼ�׽��ְ󶨵�������ַ��
	if(bind(sock,(PSOCKADDR)&addr_in,sizeof(addr_in)) == SOCKET_ERROR)
	{
		cout << "Bind failed!" << endl;
		return;
	}

	// ����������Ϊ����ģʽ���Ա�������е�IP��
#define  IO_RCVALL _WSAIOW(IOC_VENDOR,1)
	unsigned long pBufferLen[10];
	unsigned long dwBufferInLen = 1;
	unsigned long dwBytesReturned = 0;
	if ((WSAIoctl(sock, IO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen), &pBufferLen,
		sizeof(pBufferLen), &dwBytesReturned, NULL, NULL)) == SOCKET_ERROR)
	{
		cout<<"Ioctlsocket failed!"<<endl;
		return;
	}
	
	// ��socket����Ϊ������ģʽ
	unsigned long dwTemp = 1;
    ioctlsocket(sock, FIONBIO, &dwTemp);

	// ���ý��ջ�����
	char pBuffer[BURRER_SIZE];

	// ������IP���ݰ�������
	CNodeList IpList;

	double dwDuration = atof(argv[1]);	// �������Ϊ����ʱ��
	time_t beg;
	time_t end;
	time(&beg);		// ��õ�ǰϵͳʱ��

	// �������IP��ַ
	cout << endl;
	cout << "����IP:" 
		 << inet_ntoa(*(in_addr *)&(addr_in.sin_addr.S_un.S_addr)) << endl << endl;
	cout << "��ʼ����..." << endl << endl;

	while (1)
	{		
		time(&end);			// ��õ�ǰϵͳʱ��
		//�������ʱ�䵽���ͽ�������
		if (end-beg >= dwDuration)
		{
			break;
		}

		// ���񾭹�������IP���ݰ�
		int nPacketSize = recv(sock,pBuffer,BURRER_SIZE,0);
		if (nPacketSize > 0)
		{
			IPHEADER * pIpHdr;
			// ͨ��ָ��ѻ������е�����ǿ��ת��ΪIPHEADER���ݽṹ
			pIpHdr = (IPHEADER *)pBuffer;
			// �ж�IP����ԴIP��ַ��Ŀ��IP��ַ�Ƿ�Ϊ����������IP��ַ
			if (pIpHdr->SourceAddress == addr_in.sin_addr.S_un.S_addr 
				|| pIpHdr->DestAddress == addr_in.sin_addr.S_un.S_addr) 
			{
				// ���ԴIP��ַ��Ŀ��IP��ַ�Ǳ���IP���򽫸�IP���ݰ���������
				IpList.addNode(pIpHdr->SourceAddress, pIpHdr->DestAddress, pIpHdr->Protocal);
			}
		}
	}

	// ���ͳ�ƽ��
	cout << "IP���ݰ�ͳ�ƽ��: (" << dwDuration << " ��)"<< endl << endl;
	IpList.print(cout);
	cout << endl;

	return;
}