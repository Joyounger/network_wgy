#if !defined(AFX_CLIENTBYSOCKETDLG_H__BCD212ED_1E03_4319_A76D_8D4199E46364__INCLUDED_)
#define AFX_CLIENTBYSOCKETDLG_H__BCD212ED_1E03_4319_A76D_8D4199E46364__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CClientBySocketDlg dialog
//���������¼�
#define NETWORK_EVENT	(WM_USER + 0x0105)
#define NETWORK_LOG		(WM_USER + 0x0106)

#define LOG_conn_ing 0
#define LOG_conn_suc 1
#define LOG_send_itm 2
#define LOG_send_str 3
#define LOG_recv_upt 4
#define LOG_recv_str 5
#define LOG_conn_cls 6
#define LOG_recv_tim 7
#define LOG_recv_udp 8

typedef unsigned char uchar;
typedef unsigned short usint;
typedef unsigned long ulint;

//��һ���޷��ų����͵Ļ���IP��ַת��Ϊ�ַ������͵��û��Ѻ�IP��ʽ
inline char* ipTostr(ulint ulIP)
{
	in_addr ipAddr;
	ipAddr.s_addr = ulIP;
	return inet_ntoa(ipAddr);
}

class CClientBySocketDlg : public CDialog
{
// Construction
public:
	CClientBySocketDlg(CWnd* pParent = NULL);	// standard constructor

// App Data
	WSADATA WSAData;
	SOCKET Sock;
	SOCKET Sock_U;
	struct hostent *pHost;
	SOCKADDR_IN serv_addr;		//Ŀ�ĵ�IP��ַ�Ͷ˿ڵ����ݽṹ
	SOCKADDR_IN serv_udp_addr;	//Ŀ�ĵ�IP��ַ�Ͷ˿ڵ����ݽṹ
	SOCKADDR_IN rmot_udp_addr;	//��ŶԶ�udp���յ�ַ�ṹ���ظ�ʹ�ã���ʱ��ʱ���ô˽ṹ
	int rmot_udp_addr_len;
	char LocalName[16];
	int nSele;
	SOCKADDR_IN addr_in;
	SOCKADDR_IN addr_in_udp;
	unsigned short int servUDPort;
	BOOL InitNetwork(); //��ʼ�����纯��

	//���������첽�¼��Ĵ�����
	void OnClose(SOCKET CurSock);	//�Զ�Socket�Ͽ�
	void OnSend(SOCKET CurSock);	//�����������ݰ�
	void OnReceive(SOCKET CurSock);	//�������ݰ�����
	void OnConnect(SOCKET CurSock, int error);		//�ͻ�����������
	void OnNetEvent(WPARAM wParam, LPARAM lParam);	//�첽�¼��ص�����
	void OnLogDisp(WPARAM wParam, LPARAM lParam);	//��־�������
	void OnResum();					//�����ж����ú���
	void OnGetudpPort(SOCKET s);	//���ͻ�֪UDP�ں�����
	void OnReceiveUDP(SOCKET s);	//����UDP��Ϣ
	void OnReceiveTCP(SOCKET s);	//����TCP����
	void OnGetTime(SOCKET s);		//����TCPʱ��
	void OnGotudpPort(SOCKET s);	//��֪UDP�˿ں�

// Dialog Data
	//{{AFX_DATA(CClientBySocketDlg)
	enum { IDD = IDD_CLIENTBYSOCKET_DIALOG };
	CIPAddressCtrl	m_ipconn;
	CEdit	m_catlog;
	CComboBox	m_iplist;
	CString	m_udptxt;
	CString	m_catlog_txt;
	int		m_conn_to_port;
	CString	m_curlog;
	CString	m_curstat;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientBySocketDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientBySocketDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void On_UDPECHO();
	afx_msg void On_TCPTIME();
	afx_msg void On_UDP_SEND_CLK();
	afx_msg void On_CONN();
	afx_msg void On_Disconn();
	afx_msg void On_Close();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTBYSOCKETDLG_H__BCD212ED_1E03_4319_A76D_8D4199E46364__INCLUDED_)
