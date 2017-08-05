
#include "stdafx.h"
#include "ArpByWinpcap.h"
#include "ArpByWinpcapDlg.h"
#include "arp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CArpByWinpcapDlg dialog

CArpByWinpcapDlg::CArpByWinpcapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArpByWinpcapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArpByWinpcapDlg)
	m_Dev_No = 0;
	m_sending = _T("");
	m_count = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CArpByWinpcapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArpByWinpcapDlg)
	DDX_Control(pDX, IDC_MAC_LIST, m_Mac_list);
	DDX_Control(pDX, IDC_DEV_LIST, m_Dev);
	DDX_Text(pDX, IDC_EDIT_DEVNo, m_Dev_No);
	DDV_MinMaxInt(pDX, m_Dev_No, 0, 9);
	DDX_Text(pDX, IDC_SEND_ARP, m_sending);
	DDX_Text(pDX, IDC_COUNT, m_count);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CArpByWinpcapDlg, CDialog)
	//{{AFX_MSG_MAP(CArpByWinpcapDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCLOSE, OnClose)
	ON_BN_CLICKED(IDC_ANALYSIS, OnGetDev)
	ON_BN_CLICKED(IDC_GET_MAC, OnGetMac)
	ON_LBN_SELCHANGE(IDC_DEV_LIST, OnSelChangeDevList)
	ON_BN_CLICKED(IDC_STOP_CAP, OnStopCap)
	ON_BN_CLICKED(IDC_ANALYSIS, OnGetDev)
	ON_MESSAGE(WM_PACKET, OnPacket)		//������Ϣӳ��
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArpByWinpcapDlg message handlers

BOOL CArpByWinpcapDlg::OnInitDialog()
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

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_Dev_No = 0;
	UpdateData(FALSE);  
	GetDlgItem(IDC_GET_MAC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_CAP)->EnableWindow(FALSE);
	return TRUE;  
}

void CArpByWinpcapDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CArpByWinpcapDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


HCURSOR CArpByWinpcapDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CArpByWinpcapDlg::OnClose() 
{
	SendMessage( WM_CLOSE );
}

void CArpByWinpcapDlg::OnGetDev() 
{
	GetDlgItem(IDC_ANALYSIS)->EnableWindow(FALSE);
	
	int i = 1;
	string strDev = "";
	m_Dev.AddString("����������ϵͳ�����������б����£�");
	pcap_if_t* alldevs = 0; 
	pcap_if_t* pDev = 0;
	pcap_addr_t* pAdr = 0;
	char errbuf[PCAP_ERRBUF_SIZE+1]; 
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{ 								// ����豸�б� 
		MessageBox(errbuf, NULL, MB_OK | MB_ICONINFORMATION);					// ��û���豸�򵯳�����
		exit(1);
	} 
	for(pDev = alldevs; pDev; pDev = pDev->next)
	{								// �������г�Ա
		if (pDev->description)
		{
			strDev = char(i + 48);
			strDev += ". ";
			strDev += DelSpace(pDev->description);								//ȥ��������������Ŀո�
			pAdr = pDev->addresses;
			if (pAdr!=NULL)
			{		
				if (pAdr->addr->sa_family == AF_INET)
				{							//pAdr->addr�Ƿ�IP��ַ����
					strDev += " -> ";
					strDev += IpToStr(((struct sockaddr_in *)pAdr->addr)->sin_addr.s_addr);
					if(IpToStr(((struct sockaddr_in *)pAdr->addr)->sin_addr.s_addr)[0] != '0')
					{
						m_Dev_No = i;
						UpdateData(FALSE);										//���ݱ���ֵȥ����
					}
					strDev += " & [";
					strDev += IpToStr(((struct sockaddr_in *)pAdr->netmask)->sin_addr.s_addr);
					strDev += "]";
					GetDlgItem(IDC_GET_MAC)->EnableWindow(TRUE);				//��������IP��ַ����ʹץ����ť����
				}
			}
			m_Dev.InsertString(i++, strDev.c_str());
		}
	}
	pcap_freealldevs(alldevs);													//������Ҫ�б�, �ͷ�
}

void CArpByWinpcapDlg::OnGetMac()
{
	GetDlgItem(IDC_GET_MAC)->EnableWindow(FALSE);								//ʹ�����ظ����£���ȷ���߳���ȷ��
	UpdateData(TRUE);															//���ݽ���ı������
	int nDevNo = m_Dev_No;														//��ʱȡ�ò����������
	
	int i = 0;
	int nDev = 0;
	unsigned long chLocalIp = 0;														//��ű���ip��ַ
	pcap_if_t* alldevs = 0; 
	pcap_if_t* pDev = 0;
	pcap_addr_t* pAdr = 0;
	char errbuf[PCAP_ERRBUF_SIZE+1]; 
	pcap_findalldevs(&alldevs, errbuf);											// ����豸�б� 
	for(pDev = alldevs; pDev; pDev = pDev->next)
	{
		nDev++;							// ȡ����������
	}
	if ((nDevNo < 1) || (nDevNo > nDev))
	{
		MessageBox("����������Խ��!", "Get", MB_OK);
		pcap_freealldevs(alldevs);												// �ͷ��豸�б�
		GetDlgItem(IDC_GET_MAC)->EnableWindow(TRUE);							//ʹ��ť���ٰ�������
		return;
	}
	for(pDev = alldevs, i = 0; i < nDevNo - 1; pDev = pDev->next, i++);			// ͨ��ָ��ת���ϲ���ѡ����豸
	pAdr = pDev->addresses;
	if(!pAdr)
	{																	//��û�а�IP��ַ�����˳�
		MessageBox("��������û�а�IP��ַ!", "Get.Note", MB_OK);
		pcap_freealldevs(alldevs);
		GetDlgItem(IDC_GET_MAC)->EnableWindow(TRUE);							//ʹ��ť���ٰ�������
		return;
	}
	chLocalIp = ((struct sockaddr_in *)pAdr->addr)->sin_addr.s_addr;			//�õ�����ip
	if(IpToStr(chLocalIp)[0] == '0')
	{
		MessageBox("��ȷ����������������������!", "Get.Note", MB_OK);
		pcap_freealldevs(alldevs);
		GetDlgItem(IDC_GET_MAC)->EnableWindow(TRUE);							//ʹ��ť���ٰ�������
		return;
	}
	if (!GetMacSignal)
	{
		bLocalMac = GetSelfMac(pDev->name, chLocalIp);
	}
	if (!GetMacSignal)
	{
		MessageBox("��ȷ������������������!", "Get.Note", MB_OK);
		pcap_freealldevs(alldevs);
		GetDlgItem(IDC_GET_MAC)->EnableWindow(TRUE);							//ʹ��ť���ٰ�������
		return;
	}
	pDevGlobalHandle = pDev;
	strSelDeviceName = pDev->name;
	nThreadSignal = 1;
	GetDlgItem(IDC_STOP_CAP)->EnableWindow(TRUE);
	AfxBeginThread(WaitForArpRepeatPacket, this);
	Sleep(100);																	//���غ��߳���ʱ����ɳ�ʼ������20061025
	AfxBeginThread(StartArpScan, this);
}

void CArpByWinpcapDlg::OnSelChangeDevList() 
{
	// TODO: Add your control notification handler code here
	m_Dev_No = m_Dev.GetCurSel();
	UpdateData(FALSE);
}


void CArpByWinpcapDlg::OnStopCap() 
{
	nThreadSignal = 0;
	GetDlgItem(IDC_GET_MAC)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_CAP)->EnableWindow(FALSE);
	m_sending = "�û���ֹ!";
	UpdateData(FALSE);
	return;
}

LRESULT CArpByWinpcapDlg::OnPacket(WPARAM wParam, LPARAM lParam)
{
	string* transPack = (string*)wParam;
	//�����񵽵����ݰ�
	if (lParam == 0){
		m_Mac_list.AddString(transPack->c_str());
		m_count = "����  ";
		char buffer[5];
		itoa(m_Mac_list.GetCount(), buffer, 10);	//������ת��Ϊ10�����ַ�����
		m_count += buffer;
		m_count += "  ̨�����";
	}
	else if (lParam == 1)
	{
		m_sending = "���ڷ���ARP�����!";
	}
	else if (lParam == 2)
	{
		if (nThreadSignal)
		{
			m_sending = "ȫ��ARP������������!";	//�ж������з�����ϻ����û���ֹ�ģ�
		};
	}
	UpdateData(FALSE);
	return 0;
}

