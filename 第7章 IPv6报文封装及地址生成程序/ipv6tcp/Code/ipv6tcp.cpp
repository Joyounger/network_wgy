#include <fstream.h>
#include <Winsock2.h>
#include "Iphlpapi.h"
#include "ipv6tcp.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

int main(int argc, char *argv[])
{
	//��������в���
	if (argc != 3)
	{
		cout << "Usage: ipv6tcp.exe inputfile outputfile\n";
		return -1;
	}

	//����������ļ�
	ifstream inFile(argv[1], ios::in|ios::nocreate|ios::binary);
	if (!inFile)
	{
		cerr << "Cannot open file: " << argv[1] << endl;
		return -1;
	}
	ofstream outFile(argv[2], ios::out|ios::trunc|ios::binary);
	if (!outFile)
	{
		cerr << "Cannot create file: " << argv[2] << endl;
		inFile.close();
		return -1;
	}

	//�������������
	char Buf[sizeof(IPV6_HEADER) + MAX_PAYLOAD_LEN];
	IPV6_HEADER* pIPv6Hdr = (IPV6_HEADER*)Buf;

	//������������ֶ�
	inFile.read(Buf+sizeof(IPV6_HEADER)+sizeof(TCP_HEADER), MAX_PAYLOAD_LEN-sizeof(TCP_HEADER));
	int iDataCount = inFile.gcount();

	//���IPv6������ͷ
	//4λ�汾��
	pIPv6Hdr->ver_cls_lab = 6;
	//8λ�����ȼ�
	pIPv6Hdr->ver_cls_lab <<= 8;
	pIPv6Hdr->ver_cls_lab += 0;
	//20λ����ǩ
	pIPv6Hdr->ver_cls_lab <<= 20;
	pIPv6Hdr->ver_cls_lab += 0;
	pIPv6Hdr->ver_cls_lab = htonl(pIPv6Hdr->ver_cls_lab);
	//16λ�غɳ���
	pIPv6Hdr->payload_len = htons(iDataCount);
	//8λ��һ�ײ�
	pIPv6Hdr->next_header = IPPROTO_TCP;
	//8λ��������
	pIPv6Hdr->hop_limit = 128;

	//128λԴ��ַ���ɱ���Mac��ַ����
	pIPv6Hdr->src_ip.prefix_subnetid = 0x1;	//3λȫ�򵥲���ַǰ׺
	pIPv6Hdr->src_ip.prefix_subnetid <<= 45;
	pIPv6Hdr->src_ip.prefix_subnetid += 0x01;//45λȫ��·��ǰ׺
	pIPv6Hdr->src_ip.prefix_subnetid <<= 16;
	pIPv6Hdr->src_ip.prefix_subnetid += 0x01;//16λ����ID
	pIPv6Hdr->src_ip.prefix_subnetid = hton64(pIPv6Hdr->src_ip.prefix_subnetid);

	//��ȡ��������ӿ���Ϣ�б�
	IP_ADAPTER_INFO *pAdapterInfo = new IP_ADAPTER_INFO;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	DWORD dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	if (dwRetVal == ERROR_BUFFER_OVERFLOW)
	{
		delete pAdapterInfo;
		pAdapterInfo = new IP_ADAPTER_INFO[ulOutBufLen/sizeof(IP_ADAPTER_INFO)];
		dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	}
	//����һ��������Mac��ַ��ΪԴ��ַ
	if (dwRetVal == ERROR_SUCCESS)
	{
		pIPv6Hdr->src_ip.interface_id[0] = pAdapterInfo->Address[0];
		pIPv6Hdr->src_ip.interface_id[1] = pAdapterInfo->Address[1];
		pIPv6Hdr->src_ip.interface_id[2] = pAdapterInfo->Address[2];
		pIPv6Hdr->src_ip.interface_id[3] = 0xFF;
		pIPv6Hdr->src_ip.interface_id[4] = 0xFE;
		pIPv6Hdr->src_ip.interface_id[5] = pAdapterInfo->Address[3];
		pIPv6Hdr->src_ip.interface_id[6] = pAdapterInfo->Address[4];
		pIPv6Hdr->src_ip.interface_id[7] = pAdapterInfo->Address[5];
	}
	else //����޷�ȡ�ñ���Mac��ַ����ʹ��00-00-80-1A-E6-65����
	{
		cout << "Failed to GetAdaptersInfo, using assumptive SrcMac 00-00-80-1A-E6-65 instead.\n";
		pIPv6Hdr->src_ip.interface_id[0] = 0x00;
		pIPv6Hdr->src_ip.interface_id[1] = 0x00;
		pIPv6Hdr->src_ip.interface_id[2] = 0x80;
		pIPv6Hdr->src_ip.interface_id[3] = 0xFF;
		pIPv6Hdr->src_ip.interface_id[4] = 0xFE;
		pIPv6Hdr->src_ip.interface_id[5] = 0x1A;
		pIPv6Hdr->src_ip.interface_id[6] = 0xE6;
		pIPv6Hdr->src_ip.interface_id[7] = 0x65;
	}
	pIPv6Hdr->src_ip.interface_id[0] ^= 0x02; //��"ȫ��/����"λ��
	
	//128λĿ�ĵ�ַ����00-00-E4-86-3A-DC����
	pIPv6Hdr->dst_ip.prefix_subnetid = 0x1;	//3λȫ�򵥲���ַǰ׺
	pIPv6Hdr->dst_ip.prefix_subnetid <<= 45;
	pIPv6Hdr->dst_ip.prefix_subnetid += 0x02;//45λȫ��·��ǰ׺
	pIPv6Hdr->dst_ip.prefix_subnetid <<= 16;
	pIPv6Hdr->dst_ip.prefix_subnetid += 0x02;//16λ����ID
	pIPv6Hdr->dst_ip.prefix_subnetid = hton64(pIPv6Hdr->dst_ip.prefix_subnetid);
	pIPv6Hdr->dst_ip.interface_id[0] = 0x00;
	pIPv6Hdr->dst_ip.interface_id[1] = 0x00;
	pIPv6Hdr->dst_ip.interface_id[2] = 0xE4;
	pIPv6Hdr->dst_ip.interface_id[3] = 0xFF;
	pIPv6Hdr->dst_ip.interface_id[4] = 0xFE;
	pIPv6Hdr->dst_ip.interface_id[5] = 0x86;
	pIPv6Hdr->dst_ip.interface_id[6] = 0x3A;
	pIPv6Hdr->dst_ip.interface_id[7] = 0xDC;
	pIPv6Hdr->dst_ip.interface_id[0] ^= 0x02; //��"ȫ��/����"λ��

	//���TCPͷ��
	TCP_HEADER* pTcpHdr = (TCP_HEADER*)(Buf+sizeof(IPV6_HEADER));
	pTcpHdr->src_port = htons(3000);	//16λԴ�˿�
	pTcpHdr->dest_port = htons(3001);	//16λĿ�Ķ˿�
	pTcpHdr->seq = htonl(0);			//32λ���к���0
	pTcpHdr->ack = htonl(0);			//32λȷ�Ϻ���0
	pTcpHdr->hdr_len = sizeof(TCP_HEADER)/sizeof(u_long) << 4; //4λTCP�ײ�����+�����ֶ�
	pTcpHdr->flags = 0x02;				//��־�ֶ���SYN
	pTcpHdr->window_size = htons(64800);//���ڴ�С
	pTcpHdr->checksum = 0;				//У�����ʱ��Ϊ0
	pTcpHdr->urgent_pointer = 0;		//����ָ��

	//����TCPαͷ��
	char CheckBuf[sizeof(PSEUDO_HEADER)+MAX_PAYLOAD_LEN];
	PSEUDO_HEADER* pPseudoHdr = (PSEUDO_HEADER*)CheckBuf;
	memcpy(pPseudoHdr->src_ip, &(pIPv6Hdr->src_ip), 16);	//Դ��ַ
	memcpy(pPseudoHdr->dst_ip, &(pIPv6Hdr->dst_ip), 16);	//Ŀ�ĵ�ַ
	pPseudoHdr->pkt_len = htonl(sizeof(TCP_HEADER)+iDataCount); //�ϲ�Э�����ݰ�����
	pPseudoHdr->next_hdr = htonl(IPPROTO_TCP);	//�ϲ�Э��

	//����TCPУ���
	memcpy(CheckBuf+sizeof(PSEUDO_HEADER), Buf+sizeof(IPV6_HEADER), sizeof(TCP_HEADER)+iDataCount);
	pTcpHdr->checksum = checksum((u_short*)CheckBuf, sizeof(PSEUDO_HEADER)+sizeof(TCP_HEADER)+iDataCount);

	//��IPv6���ݰ�������ļ�
	outFile.write(Buf, sizeof(IPV6_HEADER)+sizeof(TCP_HEADER)+iDataCount);

	cout << "An IPv6 packet has been written to \"" << argv[2] << "\" successfully!\n";

	delete []pAdapterInfo;
	inFile.close();
	outFile.close();
	return 0;
}


__int64 hton64(__int64 host64)
{
	char tmp, *p=(char*)&host64;
	for (int i=0; i<4; i++)
	{
		tmp = p[i];
		p[i] = p[7-i];
		p[7-i] = tmp;
	}
	return host64;
}


USHORT checksum(USHORT* pBuf, int iSize) 
{
	unsigned long cksum = 0;
	while (iSize > 1) 
	{
		cksum += *pBuf++;
		iSize -= sizeof(USHORT);
	}
	if (iSize) 
		cksum += *(UCHAR*)pBuf;

	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (USHORT)(~cksum);
}
