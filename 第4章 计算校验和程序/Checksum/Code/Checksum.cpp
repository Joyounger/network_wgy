#include<iostream.h>
#include<fstream.h>
#include<winsock.h>			// ����ʹ������˳����ʾ��htons
#pragma comment(lib, "WS2_32.LIB")



/**************************************************************************
 * ����������ݵ�У���
 *
 *		���������
 *			pBuffer		ָ����ҪУ������ݻ�����
 *			nSize		��ҪУ������ݵĴ�С�����ֽ�Ϊ��λ
 *
 *		����ֵ��
 *			16λ��У����
 *
 **************************************************************************/
unsigned short checksum_calculating(unsigned short *pBuffer, int nSize)
{
	unsigned long dwCksum = 0;		// 32λ�ۼӺ�

	// �����ֽ�Ϊ��λ�����ۼ�
	while(nSize > 1)
	{
		dwCksum += *pBuffer++;
		nSize -= sizeof(unsigned short);
	}
	// ������ֽ���Ϊ������������һ���ֽ�
	if (nSize)
	{
		dwCksum += *(unsigned char*) pBuffer;
	}
	// ��32λ�ۼӺ͵ĸ�16λ���16λ��һ�����
	dwCksum = (dwCksum >> 16) + (dwCksum & 0xffff);
	// ����һ�����ܲ����ĸ�16λ��λ�ٴ����16λ�ۼ�
	dwCksum += (dwCksum >> 16);

	// ����16λУ���
	return (unsigned short) (~dwCksum);
}


void main(int argc, char * argv[])
{
	// �ж�����������и�ʽ�Ƿ���ȷ
	if (argc != 3)
	{
		cout << "�밴���¸�ʽ����������: Checksum inputfile outputfile" <<endl;
		return;
	}
	
	// ���������ļ���
	ifstream fInfile;
	// ��������ļ���
	fstream fOutfile;

	// ��2���Ʒ�ʽ��ָ���������ļ�
	fInfile.open(argv[1], ios::in|ios::binary);

	// ���ļ�ָ���Ƶ��ļ�ĩβ
	fInfile.seekg(0, ios::end);

	// ȡ�������ļ��ĳ���
	unsigned short wLen = (unsigned short)fInfile.tellg();

	// �ļ�ָ��λ�ó�ʼ��
	fInfile.seekg(0, ios::beg);
	
	// �������ݱ�����������������СΪ4+wLen ������4Ϊ���ݱ������ֶΡ������ֶ�
	// �Լ�У����ֶεĳ��Ⱥͣ�wLenΪ�����ֶγ��ȣ��������ļ����ȣ����ֽ�Ϊ��λ��
	unsigned char * pBuf = new unsigned char[4 + wLen];

	pBuf[0] = unsigned char(0xab);		// �����ݱ������ֶθ�ֵ
	pBuf[1] = unsigned char(wLen);		// �����ݱ������ֶθ�ֵ
	*(unsigned short *)(pBuf + 2) = 0;	// ����У���֮ǰ��У����ֶ�����Ϊ0
	
	fInfile.read(pBuf+4, wLen);			// ���������ļ�������ݱ��������ֶ�

	// ����У��Ͳ��ѽ�����뵽���ݱ���У����ֶ�
	*(unsigned short *)(pBuf+2) = checksum_calculating((unsigned short *)pBuf,4+wLen);
		
	// ��Ļ���У��ͼ�����
	cout.width(4);					
	cout << "У���Ϊ��0x" << hex << htons( *(unsigned short *)(pBuf+2) )
		 << "	(������˳����ʾ)"<< endl;
	
	// ��2���Ʒ�ʽ������ļ�
	fOutfile.open(argv[2],ios::in|ios::out|ios::binary|ios::trunc);

	// ��pBuf�е����ݱ�д������ļ�
	fOutfile.write((char *)pBuf, wLen+4);

	cout<< "���ݱ��ѳɹ�������" << argv[2] << "�ļ���!" << endl;

	delete pBuf;		// �ͷ����ݱ�������

	fInfile.close();	// �ر������ļ���
	fOutfile.close();	// �ر�����ļ���
}

