#include "stdafx.h"
#include "ClientBySocket.h"
#include "ClientBySocketDlg.h"

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
char rubsh[1024];

uchar gettime = 0;
uchar getudp = 0;
uchar gotudp = 0;
uchar connected = 0;
DWORD serv_ip;

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
// CClientBySocketDlg dialog

CClientBySocketDlg::CClientBySocketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientBySocketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientBySocketDlg)
	m_udptxt = _T("");
	m_catlog_txt = _T("");
	m_conn_to_port = 0;
	m_curlog = _T("");
	m_curstat = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientBySocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientBySocketDlg)
	DDX_Control(pDX, IDC_IPCONN, m_ipconn);
	DDX_Control(pDX, IDC_CATLOG, m_catlog);
	DDX_Control(pDX, IDC_IPLIST, m_iplist);
	DDX_Text(pDX, IDC_UDPTXT, m_udptxt);
	DDV_MaxChars(pDX, m_udptxt, 100);
	DDX_Text(pDX, IDC_CATLOG, m_catlog_txt);
	DDX_Text(pDX, IDC_PORT, m_conn_to_port);
	DDX_Text(pDX, IDC_CURLOG, m_curlog);
	DDX_Text(pDX, IDC_CURSTAT, m_curstat);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientBySocketDlg, CDialog)
	//{{AFX_MSG_MAP(CClientBySocketDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_UDPECHO, On_UDPECHO)
	ON_BN_CLICKED(IDC_TCPTIME, On_TCPTIME)
	ON_BN_CLICKED(IDC_UDPSEND, On_UDP_SEND_CLK)
	ON_BN_CLICKED(ID_CONN, On_CONN)
	ON_BN_CLICKED(IDC_DISCONN, On_Disconn)
	ON_MESSAGE(NETWORK_EVENT, OnNetEvent)
	ON_MESSAGE(NETWORK_LOG, OnLogDisp)
	ON_BN_CLICKED(IDCLOSE, On_Close)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientBySocketDlg message handlers

BOOL CClientBySocketDlg::OnInitDialog()
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
	rmot_udp_addr_len = sizeof(rmot_udp_addr);
	memset(&rmot_udp_addr, 0, rmot_udp_addr_len);
	nSele = 0;
	InitNetwork(); //��ʼ������
	while (pHost->h_addr_list[nSele++]) {
		m_iplist.AddString(ipTostr(*(ulint*)(pHost->h_addr_list[nSele - 1])));
	}
	m_iplist.SetCurSel(0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientBySocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientBySocketDlg::OnPaint() 
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
HCURSOR CClientBySocketDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CClientBySocketDlg::On_UDPECHO() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_UDPTXT)->EnableWindow(TRUE);
	GetDlgItem(IDC_UDPSEND)->EnableWindow(TRUE);
}

void CClientBySocketDlg::On_TCPTIME() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_UDPTXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_UDPSEND)->EnableWindow(FALSE);
	recv(Sock, rubsh, sizeof(rubsh), 0);//Ϊ��֤���������SOCKET��ʣ�໺�壬��Ϊֻ����2���ֽ�
	SendMessage(NETWORK_LOG, LOG_send_itm, LPARAM(getTIME));//д��־
	gettime = 1;
	send(Sock, getTIME, strlen(getTIME) + 1, 0);
}

BOOL CClientBySocketDlg::InitNetwork()
{
	//��ʼ��winsock��̬��
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0){
		MessageBox("�޷���ʼ��SOCKET!", "SOCKET", MB_OK);
		SendMessage(WM_CLOSE);
	}
	//��ʼ��Raw Socket���ڳ�ʼ��ʱ����socket�Ƿ�����������
	if ((Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR){
		MessageBox("����SOCKET����!", "SOCKET", MB_OK);
		closesocket(Sock);
		WSACleanup();
		SendMessage(WM_CLOSE);
	}
	closesocket(Sock);

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

void CClientBySocketDlg::On_UDP_SEND_CLK() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);//ȡ�ؽ������ݵ��ڲ�����
	SendMessage(NETWORK_LOG, LOG_send_str, LPARAM(&m_udptxt));//д��־
	sendto(Sock_U, m_udptxt, strlen(m_udptxt), 0, (PSOCKADDR)&serv_udp_addr, sizeof(serv_udp_addr));
}

void CClientBySocketDlg::On_CONN() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	//ȡ�������ݵ�����
	gettime = 0;
	getudp = 0;
	gotudp = 0;
	connected = 0;
	GetDlgItem(ID_CONN)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPLIST)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPCONN)->EnableWindow(FALSE);
	GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
	
	Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	nSele = m_iplist.GetCurSel();
	addr_in.sin_addr  = *(in_addr*)pHost->h_addr_list[nSele]; //IP
	ulSeleIP = *(ulint*)(pHost->h_addr_list[nSele]);
	addr_in.sin_family = AF_INET; 
	addr_in.sin_port  = 0;	//�Կͻ�����ϵͳ�Զ�������ж˿�
	if (bind(Sock, (PSOCKADDR)&addr_in, sizeof(addr_in)) == SOCKET_ERROR){ 
		MessageBox("����������!", "SOCKET", MB_OK);
		closesocket(Sock);
		return;
	}
	if ((WSAGetLastError()) == 10013) {
		MessageBox("SOCKET����ʧ��!", "SOCKET", MB_OK);
		closesocket(Sock);
		return;
	}
	
	//��SeverSock����Ϊ�첽������ģʽ����Ϊ��ע����������첽�¼�
	//m_hWndΪӦ�ó�������Ի���������ڵľ��
	m_ipconn.GetAddress(serv_ip);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_conn_to_port);
	serv_addr.sin_addr.S_un.S_addr = htonl(serv_ip);

	if(WSAAsyncSelect(Sock, m_hWnd, NETWORK_EVENT, FD_CONNECT) == SOCKET_ERROR){//��ע��connect�¼����������ӣ����Ϻ���ע�������¼�
		MessageBox("ע��TCP�����첽�¼�ʧ��!");
		closesocket(Sock);
		return;
	}
		
	SendMessage(NETWORK_LOG, LOG_conn_ing, htonl(serv_ip));
	
	connect(Sock, (LPSOCKADDR)&serv_addr, sizeof(serv_addr));//�������������¼�����Ӧ��OnConnect�¼�
}

void CClientBySocketDlg::OnNetEvent(WPARAM wParam, LPARAM lParam)
{
	//�õ������¼�����
	int iEvent = WSAGETSELECTEVENT(lParam);
	//�õ��������¼��Ŀͻ����׽���
	SOCKET CurSock = (SOCKET)wParam;
	switch(iEvent)
	{
	case FD_CONNECT: //Connection or multi-point join operation initiated on socket completed
		OnConnect(CurSock, lParam != FD_CONNECT);
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

void CClientBySocketDlg::OnClose(SOCKET CurSock)//�Զ�Socket�Ͽ�
{
	/* ���������˵�ͨ�ţ��ͷ���Ӧ��Դ */
	UpdateData(TRUE); //��Ϊ���Ҫ���ַ������棬�����Ȱѽ�������ȡ�ر���
	closesocket(Sock);
	MessageBox("TCP�����ѶϿ�!");
	closesocket(Sock_U);
	m_curstat = "Զ�˷����ѶϿ�";
	UpdateData(FALSE);
	SendMessage(NETWORK_LOG, LOG_conn_cls, 0);
	OnResum();
}

void CClientBySocketDlg::OnSend(SOCKET CurSock)//���Է����������ݰ��ˣ�����Ӧsocket��д
{
	//�ڸ�����˷�����ʱ�����Ԥ����
	UpdateData(TRUE);//��Ϊ���Ҫ���ַ������棬�����Ȱѽ�������ȡ�ر���
	m_curstat = "��������";
	UpdateData(FALSE);
	if (!gotudp && CurSock == Sock && connected) OnGetudpPort(CurSock);//����Ҫ��ȡudp�˿ں�
	if ((CurSock == Sock_U) && gotudp) {
		recvfrom(CurSock, rubsh, sizeof(rubsh), 0, (PSOCKADDR)&rmot_udp_addr, &rmot_udp_addr_len);//��UDP������գ���Ϊ������Ϣ���¼���δע�ᣬ�����ڴ�����ǰ�ȫ��
		WSAAsyncSelect(CurSock, m_hWnd, NETWORK_EVENT, FD_READ);//������udp�ѿ�д����š���ע����Ϣ�����¼�
		GetDlgItem(IDC_UDPECHO)->EnableWindow(TRUE);//���ڻ��udp�˿ں�����²�ʹ��udp��Ϣ����ť����
	}
}

void CClientBySocketDlg::OnReceive(SOCKET CurSock)//���������ݰ�������Դ���
{
	/* �������绺�����е����ݰ� */
	UpdateData(TRUE); //��Ϊ���Ҫ���ַ������棬�����Ȱѽ�������ȡ�ر���
	m_curstat = "��������";
	if ((CurSock == Sock_U) && gotudp && connected) { //��UDP���ݽ���Զ��UDP�˿ںŻ�ú�������
		OnReceiveUDP(CurSock);
		m_curstat = "����UDP����";
	}
	if (CurSock == Sock && connected) {
		OnReceiveTCP(CurSock);
		recv(CurSock, rubsh, sizeof(rubsh), 0);//��֤���SOCKET��ʣ�໺��
		m_curstat = "����TCP����";
	}
	UpdateData(FALSE);
}

void CClientBySocketDlg::OnConnect(SOCKET CurSock, int error)//�ͻ�����������
{
	/* ���������˵ĵ�ͨ�ţ�ȡ��UDP�˿ں� */
	UpdateData(TRUE); //��Ϊ���Ҫ���ַ������棬�����Ȱѽ�������ȡ�ر���
	m_curstat = "����ʧ��";
	if (error) {
		closesocket(CurSock);
		SendMessage(NETWORK_LOG, LOG_conn_ing, 0);
		OnResum();
	}
	else {
		m_curstat = "���ӳɹ�";
		SendMessage(NETWORK_LOG, LOG_conn_suc, htonl(serv_ip));		
		SendMessage(NETWORK_LOG, LOG_send_itm, LPARAM(getUDP));//д��־
		GetDlgItem(IDC_TCPTIME)->EnableWindow(TRUE);//���ӳɹ������ܿ���
		GetDlgItem(IDC_DISCONN)->EnableWindow(TRUE);
		SendMessage(NETWORK_LOG, 555, 0);
		connected = 1;
		WSAAsyncSelect(Sock, m_hWnd, NETWORK_EVENT, FD_CLOSE | FD_READ | FD_WRITE);//���ӳɹ���ע�᣺�رգ�����Ϣ����д׼���¼�
	}
	UpdateData(FALSE);
}

void CClientBySocketDlg::OnLogDisp(WPARAM wParam, LPARAM lParam) //��־�������
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
			catlogAdd += "�������ӵ���������";
			catlogAdd += ipTostr(ulint(lParam));
			catlogAdd += " -> �˿ڣ�";
			itoa(usint(m_conn_to_port), port, 10);
			catlogAdd += port;
		}
		else {
			catlogAdd += "����ʧ��!�����ַ���˿ںŲ�ȷ�ϴ���ָ������";
			OnResum();
		}
		break;
	case LOG_conn_suc: //���ӳɹ�
		catlogAdd += "�ɹ����ӵ���������";
		catlogAdd += ipTostr(ulint(lParam));
		catlogAdd += " -> �˿ڣ�";
		itoa(usint(m_conn_to_port), port, 10);
		catlogAdd += port;
		m_curstat = "������";
		UpdateData(FALSE);
		break;
	case LOG_send_itm:	//��������
		catlogAdd += "����TCP����";
		catlogAdd += getITM;
		break;
	case LOG_send_str: //�����ַ���
		catlogAdd += "����UDP��Ϣ��";
		catlogAdd += *(CString*)lParam;
		break;
	case LOG_recv_upt: //��ȡUDP�˿ڡ�
		catlogAdd += "ͨ��TCP��÷�����UDP�˿ںţ�";
		itoa(usint(lParam), port, 10);
		catlogAdd += port;
		break;
	case LOG_recv_str://�����ַ���
		catlogAdd += "�յ���Ϣ";
		catlogAdd += *(CString*)lParam;
		break;
	case LOG_conn_cls: //�ر�����
		catlogAdd += "TCP�����ѶϿ���������������ֹ�˴˴�����";			
		break;
	case LOG_recv_tim: //ͬ��ʱ��
		catlogAdd += "ͨ��TCP��÷�����ʱ�䣺";
		catlogAdd += getITM;
		free(getITM); //����ֵ��ǰ������Ŀռ��ͷ�
		break;
	case LOG_recv_udp: //ȡ����ȷUDP����
		itoa(ntohs(serv_udp_addr.sin_port), port, 10);
		catlogAdd += "ͨ��UDP������ԣ�";
		catlogAdd += ipTostr(serv_udp_addr.sin_addr.S_un.S_addr);
		catlogAdd += ":";
		catlogAdd += port;
		catlogAdd += " ���͵���Ϣ��";
		catlogAdd += getITM;
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

void CClientBySocketDlg::On_Disconn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	closesocket(Sock);
	closesocket(Sock_U);
	m_curstat = "�Ͽ�����";
	UpdateData(FALSE);
	SendMessage(NETWORK_LOG, 555, 0);
	OnResum();
}

void CClientBySocketDlg::OnResum() //�����ж�����
{
	gettime = 0;
	getudp = 0;
	gotudp = 0;
	connected = 0;
	GetDlgItem(IDC_DISCONN)->EnableWindow(FALSE);
	GetDlgItem(IDC_TCPTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_UDPECHO)->EnableWindow(FALSE);
	GetDlgItem(IDC_UDPTXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_UDPSEND)->EnableWindow(FALSE);
	GetDlgItem(ID_CONN)->EnableWindow(TRUE);
	GetDlgItem(IDC_IPLIST)->EnableWindow(TRUE);
	GetDlgItem(IDC_IPCONN)->EnableWindow(TRUE);
	GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
}

void CClientBySocketDlg::On_Close() 
{
	// TODO: Add your control notification handler code here
	if (Sock) closesocket(Sock);
	if (Sock_U) closesocket(Sock_U);
	WSACleanup();
	SendMessage(WM_CLOSE);
}

void CClientBySocketDlg::OnGetudpPort(SOCKET s)//�����ö˿ں�
{
	recv(s, rubsh, sizeof(rubsh), 0); //���SOCKET��ʣ�໺��
	if (connected) {
		send(s, getUDP, strlen(getUDP) + 1, 0);
		getudp = 1;
	}
}

void CClientBySocketDlg::OnReceiveUDP(SOCKET s)//��UDP��
{
	memset(rubsh, 0, strlen(rubsh));
	CString err_msg = "";
	recvfrom(s, rubsh, sizeof(rubsh), 0, (PSOCKADDR)&rmot_udp_addr, &rmot_udp_addr_len);
	if ((rmot_udp_addr.sin_port == serv_udp_addr.sin_port) && (rmot_udp_addr.sin_addr.S_un.S_addr == serv_udp_addr.sin_addr.S_un.S_addr) && !(strcmp(m_udptxt, rubsh))){//�ж��Ƿ����ȷ��Դͷ���͵��뷢��һ�µ��û���
		SendMessage(NETWORK_LOG, LOG_recv_udp, LPARAM(rubsh));//д��־
	}
	else {
		err_msg = "(����UDP)��";
		err_msg += rubsh;
		SendMessage(NETWORK_LOG, LOG_recv_str, LPARAM(&err_msg));//д��־
	}
}

void CClientBySocketDlg::OnReceiveTCP(SOCKET s)//��TCP����
{
	if (getudp && !gotudp) OnGotudpPort(s);
	if (gettime) OnGetTime(s);
}

void CClientBySocketDlg::OnGetTime(SOCKET s)//��TCPʱ��
{
	gettime = 0;
	char* recvData = (char*)malloc(9);
	memset(recvData, 0, 9);
	recv(s, recvData, 8, 0);
	SendMessage(NETWORK_LOG, LOG_recv_tim, LPARAM(recvData));
}

void CClientBySocketDlg::OnGotudpPort(SOCKET s)//��֪UDP�˿ں�
{
	getudp = 0;
	char recvData[2];
	recv(s, recvData, sizeof(recvData), 0);
	memcpy(&servUDPort, &recvData, 2);
	SendMessage(NETWORK_LOG, LOG_recv_upt, ntohs(servUDPort));
	serv_udp_addr = serv_addr;
	serv_udp_addr.sin_port = servUDPort;
	rmot_udp_addr = serv_udp_addr;
	Sock_U = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	bind(Sock_U, (PSOCKADDR)&addr_in, sizeof(addr_in));
	if(WSAAsyncSelect(Sock_U, m_hWnd, NETWORK_EVENT, FD_WRITE) == SOCKET_ERROR){//��udp��ע�᣺��д׼���¼���������������
		MessageBox("ע��UDP�����첽�¼�ʧ��!");
		closesocket(Sock);
		closesocket(Sock_U);
		return;
	}
	gotudp = 1;
}