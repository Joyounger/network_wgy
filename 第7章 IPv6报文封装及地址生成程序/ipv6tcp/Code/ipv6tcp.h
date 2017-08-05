#ifndef _IPV6PKT_H_
#define _IPV6PKT_H_

#pragma pack(push, 1) //should be aligned on 1 byte boundaries

#define MAX_PAYLOAD_LEN 65535

//IPv6�����ײ�
typedef struct
{
	u_long	ver_cls_lab;	// 4-bit�汾�� + 8-bit�����ȼ� + 20-bit����ǩ
	u_short	payload_len;	// 16-bit �غɳ���
	u_char	next_header;	// 8-bit ��һ�ײ�
	u_char	hop_limit;		// 8-bit ��������
	struct 
	{
		__int64 prefix_subnetid;
		u_char interface_id[8];
	} src_ip;				// 128-bit Դ��ַ
	struct 
	{
		__int64 prefix_subnetid;
		u_char interface_id[8];
	} dst_ip;				// 128-bit Ŀ�ĵ�ַ

} IPV6_HEADER;

//TCPͷ��
typedef struct
{
    u_short src_port;		//16λԴ�˿ں�
    u_short dest_port;		//16λĿ�Ķ˿ں�
	u_long seq;				//32λ���к�
	u_long ack;				//32λȷ�����
	u_char hdr_len;			//4λ�ײ����� + 6λ����
	u_char flags;			//6λ��־λ
	u_short window_size;	//16λ���ڴ�С
	u_short checksum;		//16λУ���
	u_short urgent_pointer; //16λ����ָ��

} TCP_HEADER;

//TCPαͷ��
typedef struct
{
    u_char src_ip[16];		//128λԴ��ַ
    u_char dst_ip[16];		//128λĿ�ĵ�ַ
	u_long pkt_len;			//32λ�ϲ�Э����ܳ���
	u_long next_hdr;		//24λ0 + 8λ��һ�ײ�

} PSEUDO_HEADER;

#pragma pack(pop)

//64λ�������͵������ֽ���ת�����ֽ�����
__int64 hton64(__int64 host64);

//��������У���
USHORT checksum(USHORT* pBuf, int iSize);

#endif //_IPV6PKT_H_

