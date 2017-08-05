#include "stdafx.h"
#include "ServerBySocket.h"
#include "ServerBySocketDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
ulint ulSeleIP = 0;
char* getUDP = "GET UDP PORT";
char* getTIME = "GET CUR TIME";
char recvbuff[1024];
char timestr[9];

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerBySocketDlg dialog

CServerBySocketDlg::CServerBySocketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerBySocketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerBySocketDlg)
	m_port = 0;
	m_connstat = _T("");
	m_curlog = _T("");
	m_catlog_txt = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerBySocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerBySocketDlg)
	DDX_Control(pDX, IDC_CATLOG, m_catlog);
	DDX_Control(pDX, IDC_IPlist, m_iplist);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDV_MinMaxInt(pDX, m_port, 1024, 65535);
	DDX_Text(pDX, IDC_CONNSTAT, m_connstat);
	DDX_Text(pDX, IDC_LOGSTAT, m_curlog);
	DDX_Text(pDX, IDC_CATLOG, m_catlog_txt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerBySocketDlg, CDialog)
	//{{AFX_MSG_MAP(CServerBySocketDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCLOSE, OnClose)
	ON_BN_CLICKED(IDC_STOPSERV, On_Stopserv)
	ON_BN_CLICKED(IDC_STARTSERV, On_Startserv)
	ON_MESSAGE(NETWORK_EVENT, OnNetEvent)
	ON_MESSAGE(NETWORK_LOG, OnLogDisp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerBySocketDlg message handlers

BOOL CServerBySocketDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	memset(&ClientSock, 0, sizeof(ClientSock));		//�����ӿͻ�SOCK�Ľṹ����0
	memset(&ClientSock_U, 0, sizeof(ClientSock_U));	//��UDP�Զ˷��ؽṹ����0
	U_port = htons(UDP_PORT);
	nSele = 0;
	InitNetwork(); //��ʼ������
	while (pHost->h_addr_list[nSele++]) {
		m_iplist.AddString(ipTostr(*(ulint*)(pHost->h_addr_list[nSele - 1])));
	}
	m_iplist.SetCurSel(0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerBySocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerBySocketDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerBySocketDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CServerBySocketDlg::InitNetwork()
{
	//��ʼ��winsock��̬��
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0){
		MessageBox("�޷���ʼ��SOCKET!", "SOCKET", MB_OK);
		SendMessage(WM_CLOSE);
	}
	//��ʼ��Raw Socket���ڳ�ʼ��ʱ����socket�Ƿ�����������
	if ((ServerSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR){
		MessageBox("����SOCKET����!", "SOCKET", MB_OK);
		closesocket(ServerSock);
		WSACleanup();
		SendMessage(WM_CLOSE);
	}
	closesocket(ServerSock);

	//��ȡ������ 
	if (gethostname((char*)LocalName, sizeof(LocalName)) == SOCKET_ERROR){
		MessageBox("�޷�ȡ�ñ�����!", "SOCKET", MB_OK);
		SendMessage(WM_CLOSE);
	}
	//��ȡ����IP��ַ 
	if ((pHost = gethostbyname((char*)LocalName)) == NULL){
		MessageBox("�޷�ȡ��IP��ַ!", "SOCKET", MB_OK);
		SendMessage(WM_CLOSE);
	}
	return TRUE;
}

void CServerBySocketDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	if (ServerSock) closesocket(ServerSock);
	if (ServerSock_U) closesocket(ServerSock_U);
	WSACleanup();
	SendMessage(WM_CLOSE);	
}

void CServerBySocketDlg::OnNetEvent(WPARAM wParam, LPARAM lParam)
{
	//�õ������¼�����
	int iEvent = WSAGETSELECTEVENT(lParam);
	//�õ��������¼��Ŀͻ����׽���
	SOCKET CurSock = (SOCKET)wParam;
	switch(iEvent)
	{
	case FD_ACCEPT: //Connection or multi-point join operation initiated on socket completed
		OnAccept(CurSock);
		break;
	case FD_CLOSE: //Connection identified by socket has been closed
		OnClose(CurSock);
		break;
	case FD_READ: //Socket ready for reading
		OnReceive(CurSock);
		break;
	case FD_WRITE: //Socket ready for writing
		OnSend(CurSock);
		break;
	default: break;
	}
}

void CServerBySocketDlg::OnClose(SOCKET CurSock)
{
	UpdateData(TRUE);
	//��������˵�ͨ�ţ��ͷ���Ӧ��Դ
	if (CurSock == ServerSock) {
		CloseClientSocket(ClientSock);
		closesocket(CurSock);
		closesocket(ServerSock_U);
		m_connstat = "������ֹͣ";
		UpdateData(FALSE);
		SendMessage(NETWORK_LOG, LOG_stop_svr, 5);
		OnResum();
		return;
	}
	//������CurSock��Ӧ�Ŀͻ��˵�ͨ�ţ��ͷ���Դ
	ClientSock[FindSockNo(ClientSock, CurSock)].inuse = 0;
	closesocket(CurSock);//�ر���Ӧ��CurSock
	total_conn--;
	m_connstat = char(total_conn + 48);
	m_connstat += "�������";
	UpdateData(FALSE);
	SendMessage(NETWORK_LOG, LOG_conn_cls, FindSockNo(ClientSock, CurSock));
}

void CServerBySocketDlg::OnSend(SOCKET CurSock)//�����������ݰ�
{
}

void CServerBySocketDlg::OnReceive(SOCKET CurSock)//���������ݰ�������д���
{
	memset(&recvbuff, 0, sizeof(recvbuff));//���ջ��������
	if ((CurSock != ServerSock_U) && (CurSock != ServerSock)) {//tcp��Ӧ
		recv(CurSock, recvbuff, sizeof(recvbuff), 0);
		if (!strcmp(recvbuff, getUDP)) {//����UDP�˿�
			SendMessage(NETWORK_LOG, LOG_reqe_udp, FindSockNo(ClientSock, CurSock));
			send(CurSock, (const char*)(&U_port), sizeof(U_port), 0);
			SendMessage(NETWORK_LOG, LOG_send_udp, FindSockNo(ClientSock, CurSock));
		}
		if (!strcmp(recvbuff, getTIME)) {//���ͷ�����ʱ��
			_strtime(timestr);
			SendMessage(NETWORK_LOG, LOG_reqe_tim, FindSockNo(ClientSock, CurSock));
			send(CurSock, timestr, sizeof(timestr), 0);
			SendMessage(NETWORK_LOG, LOG_send_tim, FindSockNo(ClientSock, CurSock));
		}
	}
	else if (CurSock == ServerSock_U) {//��udp���յ�ԭ������
		recvfrom(CurSock, recvbuff, sizeof(recvbuff), 0, (LPSOCKADDR)&(ClientSock_U.addr), &(ClientSock_U.addr_len));//��UDP��
		SendMessage(NETWORK_LOG, LOG_recv_str, LPARAM(recvbuff));
		sendto(CurSock, recvbuff, sizeof(recvbuff), 0, (LPSOCKADDR)&(ClientSock_U.addr), ClientSock_U.addr_len);//�ͻ���
		SendMessage(NETWORK_LOG, LOG_send_str, LPARAM(recvbuff));
	}
}

void CServerBySocketDlg::OnAccept(SOCKET CurSock)//����ͻ��˵���������
{
	int temp = 0;
	int CurConn = FindFirstEmpty(ClientSock);
	if ((CurConn < CLNT_MAX_NUM) && (total_conn <= CLNT_MAX_NUM)) {
		ClientSock[CurConn].Sock = 0xFFFFFFFF;
		ClientSock[CurConn].Sock = accept(ServerSock, (LPSOCKADDR)&(ClientSock[CurConn].addr), &(ClientSock[CurConn].addr_len));
		temp = WSAGetLastError();
		ClientSock[CurConn].inuse = 1;
		total_conn++;//����������1
		if(WSAAsyncSelect(ClientSock[CurConn].Sock, m_hWnd, NETWORK_EVENT, FD_CLOSE | FD_READ | FD_WRITE) == SOCKET_ERROR){
			MessageBox("ע�������첽�¼�ʧ��!");
			closesocket(ServerSock);
			closesocket(ServerSock_U);
			OnResum();
			return;
		}
		m_connstat = char(total_conn + 48);
		m_connstat += "�������";
		UpdateData(FALSE);
		SendMessage(NETWORK_LOG, LOG_conn_suc, CurConn);
		return;
	}
	//������������������ʵ���ò�������Ϊ��listen�������Ѿ������ˣ�����gracfully�عر�
	SOCKET abandon = accept(ServerSock, (LPSOCKADDR)&(ClientSock_Abandon.addr), &(ClientSock_Abandon.addr_len));
	SendMessage(NETWORK_LOG, LOG_conn_ing, 0);
	closesocket(abandon);
}

void CServerBySocketDlg::OnLogDisp(WPARAM wParam, LPARAM lParam) //��־�������
{
	UpdateData(TRUE);
	int iEvent = int(wParam);
	string catlogAdd = "";
	char port[20];
	char* getITM = (char*)lParam;
	switch(iEvent)
	{
	case LOG_conn_ing: //��������
		if (lParam) {
			_strtime(timestr);
			catlogAdd += "-*-*-*-*-*-�������ԣ�";
			catlogAdd += timestr;
			catlogAdd += " ��ʼ����-*-*-*-*-*-";
		}
		else {
			catlogAdd += "��������<-*-";
			catlogAdd += ipTostr(ClientSock_Abandon.addr.sin_addr.S_un.S_addr);
			catlogAdd += ":";
			itoa(ntohs(ClientSock_Abandon.addr.sin_port), port, 10);
			catlogAdd += port;
			catlogAdd += "-*->������ʧ��!�������������!";
		}
		break;
	case LOG_conn_suc: //���ӳɹ�
		catlogAdd += "��������<-*-";
		catlogAdd += ipTostr(ClientSock[lParam].addr.sin_addr.S_un.S_addr);
		catlogAdd += ":";
		itoa(ntohs(ClientSock[lParam].addr.sin_port), port, 10);
		catlogAdd += port;
		catlogAdd += "-*->����������!";
		break;
	case LOG_reqe_tim:	//ʱ������
		catlogAdd += "�յ�����<-*-";
		catlogAdd += ipTostr(ClientSock[lParam].addr.sin_addr.S_un.S_addr);
		catlogAdd += ":";
		itoa(ntohs(ClientSock[lParam].addr.sin_port), port, 10);
		catlogAdd += port;
		catlogAdd += "-*->�ķ�����ʱ��ͬ������!";
		break;
	case LOG_send_str: //�����ַ���
		catlogAdd += "������Ϣ<-*-";
		catlogAdd += ipTostr(ClientSock_U.addr.sin_addr.S_un.S_addr);
		catlogAdd += ":";
		itoa(ntohs(ClientSock_U.addr.sin_port), port, 10);
		catlogAdd += port;
		catlogAdd += "-*->(UDP)��";
		catlogAdd += getITM;
		break;
		break;
	case LOG_stop_svr://ֹͣ����
			_strtime(timestr);
			catlogAdd += "-*-*-*-*-*-�������ڣ�";
			catlogAdd += timestr;
			catlogAdd += " ֹͣ����-*-*-*-*-*-";
		break;
	case LOG_recv_str://�����ַ���
		catlogAdd += "�յ�����<-*-";
		catlogAdd += ipTostr(ClientSock_U.addr.sin_addr.S_un.S_addr);
		catlogAdd += ":";
		itoa(ntohs(ClientSock_U.addr.sin_port), port, 10);
		catlogAdd += port;
		catlogAdd += "-*->��UDP��Ϣ��";
		catlogAdd += getITM;
		break;
	case LOG_conn_cls: //�ر�����
		catlogAdd += "�ͻ��ˣ�<-*-";
		catlogAdd += ipTostr(ClientSock[lParam].addr.sin_addr.S_un.S_addr);
		catlogAdd += ":";
		itoa(ntohs(ClientSock[lParam].addr.sin_port), port, 10);
		catlogAdd += port;
		catlogAdd += "-*->�������ѶϿ���";
		break;
	case LOG_send_tim: //ͬ��ʱ��
		catlogAdd += "��ͻ���<-*-";
		catlogAdd += ipTostr(ClientSock[lParam].addr.sin_addr.S_un.S_addr);
		catlogAdd += ":";
		itoa(ntohs(ClientSock[lParam].addr.sin_port), port, 10);
		catlogAdd += port;
		catlogAdd += "-*->���͵�ǰʱ��:";
		catlogAdd += timestr;
		break;
	case LOG_send_udp: //����UDP�˿ڡ�
		catlogAdd += "��ͻ���<-*-";
		catlogAdd += ipTostr(ClientSock[lParam].addr.sin_addr.S_un.S_addr);
		catlogAdd += ":";
		itoa(ntohs(ClientSock[lParam].addr.sin_port), port, 10);
		catlogAdd += port;
		itoa(UDP_PORT, port, 10);
		catlogAdd += "-*->����UDP����˿ں�:";
		catlogAdd += port;
		break;
	case LOG_reqe_udp: //����UDP����
		catlogAdd += "�յ�����<-*-";
		catlogAdd += ipTostr(ClientSock[lParam].addr.sin_addr.S_un.S_addr);
		catlogAdd += ":";
		itoa(ntohs(ClientSock[lParam].addr.sin_port), port, 10);
		catlogAdd += port;
		catlogAdd += "-*->��UDP����˿ںŻ�֪����!";
		break;
	default: break;
	}	
	m_curlog = catlogAdd.c_str();
	UpdateData(FALSE);
	catlogAdd += "\r\n";
	m_catlog.SetSel(m_catlog_txt.GetLength(), m_catlog_txt.GetLength(), 0);//���ѡ����β
	m_catlog.ReplaceSel(catlogAdd.c_str());
	UpdateData(TRUE);
	m_catlog.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);//�Զ���������־���
}

void CServerBySocketDlg::On_Stopserv() //ֹͣ����Ĵ���
{
	// TODO: Add your control notification handler code here
	if (ClientSock_U.addr.sin_port) {
		strcpy(recvbuff, "��Ҫֹͣ������:)\0");
		closesocket(ServerSock_U);
		sendto(ClientSock_U.Sock, recvbuff, sizeof(recvbuff), 0, (LPSOCKADDR)&(ClientSock_U.addr), ClientSock_U.addr_len);//����ֹ��Ϣ20051123
		Sleep(1);
	}
	shutdown(ServerSock, 2); //������ֹ����
	SendMessage(NETWORK_EVENT, ServerSock, FD_CLOSE);
}

void CServerBySocketDlg::OnResum()//�����ж�����
{
	GetDlgItem(IDC_STOPSERV)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPlist)->EnableWindow(TRUE);
	GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_STARTSERV)->EnableWindow(TRUE);
}

void CServerBySocketDlg::On_Startserv() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); //ȡ�������ݵ�����
	if ((m_port < 1024) || (m_port > 65535)) return; //�˿ڲ��Ϸ��򷵻�
	GetDlgItem(IDC_STARTSERV)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPlist)->EnableWindow(FALSE);
	GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
	
	//����TCP socket
	ServerSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	nSele = m_iplist.GetCurSel();
	addr_in.sin_addr  = *(in_addr*)pHost->h_addr_list[nSele]; //IP
	ulSeleIP = *(ulint*)(pHost->h_addr_list[nSele]);
	addr_in.sin_family = AF_INET; 
	addr_in.sin_port = htons(m_port); //�������˿ں��ֹ�ָ��
	for (int i = 0; i < CLNT_MAX_NUM; i++){
		ClientSock[i].addr_len = sizeof(ClientSock[i].addr);
		ClientSock[i].addr = addr_in;
	}
	total_conn = 0;

	if (bind(ServerSock, (PSOCKADDR)&addr_in, sizeof(addr_in)) == SOCKET_ERROR){ 
		MessageBox("����������!", "SOCKET", MB_OK);
		closesocket(ServerSock);
		OnResum();
		return;
	}
	if ((WSAGetLastError()) == 10013) {
		MessageBox("SOCKET����ʧ��!", "SOCKET_TCP", MB_OK);
		closesocket(ServerSock);
		return;
	}
	
	//����UDP socket
	addr_in_udp = addr_in; //��ַ���Ǳ���������ֱ������ʹ��
	addr_in_udp.sin_port = htons(UDP_PORT); //�˿�

	ServerSock_U = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (bind(ServerSock_U, (PSOCKADDR)&addr_in_udp, sizeof(addr_in_udp)) == SOCKET_ERROR){ 
		MessageBox("����������!", "SOCKET_UDP", MB_OK);
		closesocket(ServerSock);
		closesocket(ServerSock_U);
		OnResum();
		return;
	}

	ClientSock_U.addr_len = sizeof(ClientSock_U.addr);
	ClientSock_U.addr = addr_in_udp;
	ClientSock_U.addr.sin_port = 0;
	ClientSock_Abandon = ClientSock_U;

	//׼��������ɣ�����ע��TCP���첽��Ӧ�¼�
	if(WSAAsyncSelect(ServerSock, m_hWnd, NETWORK_EVENT, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR){
		MessageBox("ע�������첽�¼�ʧ��!");
		closesocket(ServerSock);
		closesocket(ServerSock_U);
		OnResum();
		return;
	}

	//ע��UDP���첽��Ӧ�¼�
	if(WSAAsyncSelect(ServerSock_U, m_hWnd, NETWORK_EVENT, FD_READ | FD_WRITE) == SOCKET_ERROR){
		MessageBox("ע�������첽�¼�ʧ��!");
		closesocket(ServerSock);
		closesocket(ServerSock_U);
		OnResum();
		return;
	}

	//��ʼ��TCP�Ͻ�������
	if (listen(ServerSock, MAX_QUEUE) == SOCKET_ERROR) {
		MessageBox("�޷���ʼ��TCP����!");
		closesocket(ServerSock);
		closesocket(ServerSock_U);
		OnResum();
		return;		
	}
	
	m_connstat = "��������.....";
	UpdateData(FALSE);
	ClientSock_U.Sock = ServerSock_U;
	GetDlgItem(IDC_STOPSERV)->EnableWindow(TRUE);
	SendMessage(NETWORK_LOG, LOG_conn_ing, 5);
	ShowWindow(SW_MINIMIZE);//��С����������
}


int CServerBySocketDlg::FindFirstEmpty(client_sock* in)
{
	for (int i = 0; i < CLNT_MAX_NUM; i++) {
		if (!in[i].inuse) return i;
	}
	return CLNT_MAX_NUM;
}

int CServerBySocketDlg::FindSockNo(client_sock* in, SOCKET s)
{
	for (int i = 0; i < CLNT_MAX_NUM; i++) {
		if (in[i].Sock == s) return i;
	}
	return CLNT_MAX_NUM;
}

void CServerBySocketDlg::CloseClientSocket(client_sock* in)
{
	for (int i = 0; i < CLNT_MAX_NUM; i++) {
		if (in[i].inuse) {
			closesocket(in[i].Sock);
			in[i].inuse = 0;
		}
	}
}
