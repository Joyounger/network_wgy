
string strSelDeviceName = "";
unsigned char* bLocalMac;
pcap_if_t* pDevGlobalHandle = 0;
int nThreadSignal = 0;
int GetMacSignal = 0;
char* IpToStr(unsigned long ulIP);	// ��һ���޷��ų����͵Ļ���IP��ַת��Ϊ�ַ������͵��û��Ѻ�IP��ʽ
char* MacToStr(unsigned char* chMAC);//��MAC��ַ�������Ϊ����-���Ĵ�д���ַ��ɶ���ʽ
char* DelSpace(char* in);//ȥ���ַ����ж���Ŀո�
unsigned char* BuildArpRequestPacket(unsigned char* source_mac, unsigned char* arp_sha, unsigned long chLocalIP, unsigned long arp_tpa, int PackSize);//��װARP�����
unsigned char* GetSelfMac(char* pDevName, unsigned long chLocalIP);
void SendArpRequest(pcap_if_t* pDev, unsigned char* bLocalMac);


UINT StartArpScan(LPVOID mainClass);//����ARP�������ݰ����̺߳���

UINT WaitForArpRepeatPacket(LPVOID mainClass);//����ARP��Ӧ���̺߳���
