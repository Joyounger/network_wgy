// ServerBySocketDlg.h : header file
//

#if !defined(AFX_SERVERBYSOCKETDLG_H__5F4761E7_4D2E_4891_B785_27822568EF3C__INCLUDED_)
#define AFX_SERVERBYSOCKETDLG_H__5F4761E7_4D2E_4891_B785_27822568EF3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CServerBySocketDlg dialog
//���������¼�
#define NETWORK_EVENT	(WM_USER + 0x0105)
#define NETWORK_LOG		(WM_USER + 0x0106)

#define LOG_conn_ing 0
#define LOG_conn_suc 1
#define LOG_reqe_tim 2
#define LOG_send_str 3
#define LOG_stop_svr 4
#define LOG_recv_str 5
#define LOG_conn_cls 6
#define LOG_send_tim 7
#define LOG_send_udp 8
#define LOG_reqe_udp 9

typedef unsigned char uchar;
typedef unsigned short usint;
typedef unsigned long ulint;

#define CLNT_MAX_NUM 9	//���ͻ�������
#define UDP_PORT 5555	//UDP����˿ں�
#define MAX_QUEUE 5		//����������󳤶�

//��һ���޷��ų����͵Ļ���IP��ַת��Ϊ�ַ������͵��û��Ѻ�IP��ʽ
inline char* ipTostr(ulint ulIP)
{
	in_addr ipAddr;
	ipAddr.s_addr = ulIP;
	return inet_ntoa(ipAddr);
}

//�洢����socket�ȶԶ���Ϣ�Ľṹ��
typedef struct client_sock {
	SOCKET Sock;
	SOCKADDR_IN addr;
	int addr_len;
	uchar inuse;
} client_sock;

class CServerBySocketDlg : public CDialog
{
// Construction
public:
	CServerBySocketDlg(CWnd* pParent = NULL);	// standard constructor

// App Data
	client_sock ClientSock[CLNT_MAX_NUM]; //�洢��accept���ص���ͻ���ͨ�ŵ�Socket�Ƚṹ��Ϣ������
	client_sock ClientSock_U;
	client_sock ClientSock_Abandon;
	int total_conn;
	usint U_port;
	int FindFirstEmpty(client_sock* in); //�����׸������ӳ�
	int FindSockNo(client_sock* in, SOCKET s); //���Ҹ���Socket��client�����еı��
	void CloseClientSocket(client_sock* in); //�ر����пͻ�����
	WSADATA WSAData;
	SOCKET ServerSock;
	SOCKET ServerSock_U;
	struct hostent *pHost;
	char LocalName[16];
	int nSele;
	SOCKADDR_IN addr_in;
	SOCKADDR_IN addr_in_udp;
	BOOL InitNetwork(); //��ʼ�����纯��
	
	//���������첽�¼��Ĵ�����
	void OnClose(SOCKET CurSock);	//�Զ�Socket�Ͽ�
	void OnSend(SOCKET CurSock);	//�����������ݰ�
	void OnReceive(SOCKET CurSock);	//�������ݰ�����
	void OnAccept(SOCKET CurSock);	//�ͻ�����������
	void OnNetEvent(WPARAM wParam, LPARAM lParam);//�첽�¼��ص�����
	void OnLogDisp(WPARAM wParam, LPARAM lParam); //��־�������
	void OnResum();					//�����ж����ú���

// Dialog Data
	//{{AFX_DATA(CServerBySocketDlg)
	enum { IDD = IDD_SERVERBYSOCKET_DIALOG };
	CEdit	m_catlog;
	CComboBox	m_iplist;
	int		m_port;
	CString	m_connstat;
	CString	m_curlog;
	CString	m_catlog_txt;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerBySocketDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerBySocketDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void On_Stopserv();
	afx_msg void On_Startserv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERBYSOCKETDLG_H__5F4761E7_4D2E_4891_B785_27822568EF3C__INCLUDED_)
