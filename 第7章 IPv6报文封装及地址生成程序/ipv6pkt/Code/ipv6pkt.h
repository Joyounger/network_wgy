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

#pragma pack(pop)

//64λ�������͵������ֽ���ת�����ֽ�����
__int64 hton64(__int64 host64);


#endif //_IPV6PKT_H_

