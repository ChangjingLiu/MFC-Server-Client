
// MFC_ServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "afxsock.h"
#include "framework.h"
#include "MFC_Server.h"
#include "MFC_ServerDlg.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCServerDlg 对话框



CMFCServerDlg::CMFCServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_SERVER_DIALOG, pParent)
	, m_port(0)
	, m_serverSocket(this)
	, m_clientSocket(this)
	, m_sendBuf(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTONSERVER, m_onBtn);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Control(pDX, IDC_BTOFFSERVER, m_offBtn);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Control(pDX, IDC_BTSEND, m_sendBtn);
	DDX_Text(pDX, IDC_SENDBOX, m_sendBuf);
	//DDX_Control(pDX, IDC_COMBO1, m_clientaddrs);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_COUNT, m_clientcount);
}

BEGIN_MESSAGE_MAP(CMFCServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTONSERVER, &CMFCServerDlg::OnBnClickedBtonserver)
	ON_BN_CLICKED(IDC_BTOFFSERVER, &CMFCServerDlg::OnBnClickedBtoffserver)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMFCServerDlg::OnLvnItemchangedList1)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CMFCServerDlg::OnIpnFieldchangedIpaddress1)
	ON_EN_CHANGE(IDC_PORT, &CMFCServerDlg::OnEnChangePort)
	ON_EN_CHANGE(IDC_SENDBOX, &CMFCServerDlg::OnEnChangeSendbox)
	ON_BN_CLICKED(IDC_BTSEND, &CMFCServerDlg::OnBnClickedBtsend)
	//ON_CBN_SELCHANGE(IDC_COMBO1, &CMFCServerDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMFCServerDlg::OnCbnSelchangeCombo1)
	//ON_EN_CHANGE(IDC_EDITCOUNT, &CMFCServerDlg::OnEnChangeEditcount)
	ON_EN_CHANGE(IDC_COUNT, &CMFCServerDlg::OnEnChangeCount)
	ON_BN_CLICKED(IDC_BTEXPORT, &CMFCServerDlg::OnBnClickedBtexport)
END_MESSAGE_MAP()


// CMFCServerDlg 消息处理程序

BOOL CMFCServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//设定默认的IP和Port
	//m_combo.AddString(_T("请选择客户端"));
	//SetWindowTextW(_T("0"));
	//初始化report
	m_listCtrl.InsertColumn(0, _T("消息列表"));
	CRect rcListCtrl;
	m_listCtrl.GetClientRect(&rcListCtrl);
	m_listCtrl.SetColumnWidth(0, rcListCtrl.Width() );

	m_combo.SetCurSel(0);
	m_ip.SetAddress(127, 0, 0, 1);
	m_port = 8080;
	m_offBtn.EnableWindow(FALSE);
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//启动Server
void CMFCServerDlg::OnBnClickedBtonserver()
{
	UpdateData(TRUE);
	BYTE a, b, c, d;
	m_ip.GetAddress(a, b, c, d);
	CString ip;
	ip.Format(_T("%d.%d.%d.%d"), a, b, c, d);
	BOOL ret = m_serverSocket.Create(m_port, SOCK_STREAM, ip);
	if (ret)
	{

		ret = m_serverSocket.Listen();
		if (ret)
		{
			//m_serverSocket.a
			m_onBtn.EnableWindow(FALSE);
			m_offBtn.EnableWindow(TRUE);
			AddMsg(_T("服务器建立成功！"));
			return;
		}
		else
		{
			MessageBox(_T("监听套接字监听失败"));
		}
	}
	else
	{
		MessageBox(_T("创建监听套接字失败"));
	}
	m_onBtn.EnableWindow(TRUE);
	m_offBtn.EnableWindow(FALSE);
}

//停止Server //更新客户端列表
void CMFCServerDlg::OnBnClickedBtoffserver()
{
	m_serverSocket.CloseAllClient();
	m_onBtn.EnableWindow(TRUE);
	m_offBtn.EnableWindow(FALSE);
	AddMsg(_T("服务器已关闭！"));
}

void CMFCServerDlg::OnIpnFieldchangedIpaddress1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	*pResult = 0;
}


void CMFCServerDlg::OnEnChangePort()
{
}


void CMFCServerDlg::OnEnChangeSendbox()
{
}


void CMFCServerDlg::OnBnClickedBtsend()
{
	//获得当前combo的信息
	CString combostr = GetComboText();
	UpdateData(TRUE);
	USES_CONVERSION;
	//函数T2A和W2A均支持ATL和MFC中的字符
	char* send = T2A(m_sendBuf);
	if (0 < m_serverSocket.SendMsg(combostr,send))
	{
	}
	else
	{
		return;
	}
	//清空发送窗口
	SetDlgItemText(IDC_SENDBOX, _T(""));
}


void CMFCServerDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//1某个客户端下线时删除 2更新客户端列表
void CMFCServerDlg::CloseClient(CClientSocket* client) 
{
	AddMsg(_T("对话框已接收！"));
	m_serverSocket.CloseSpecialClient(client);
}


//添加client的信息进入combo
void CMFCServerDlg::AddClientAddrs(CClientSocket* oneclient)
{
	CString addr;
	addr.Format(_T("%s:%d"), oneclient->m_ip, oneclient->m_port);
	m_combo.AddString(addr);
}

void CMFCServerDlg::AddMsg(CString msg)
{
	int nRow = m_listCtrl.GetItemCount();
	m_listCtrl.InsertItem(nRow, msg);
}

//更新combo中的客户端信息
void CMFCServerDlg::OnCbnSelchangeCombo1()
{
}

//清除combo信息
void CMFCServerDlg::CleanCombo()
{
	m_combo.ResetContent();
}
//获取combo中选中的信息
CString CMFCServerDlg::GetComboText()
{
	int i = m_combo.GetCurSel();//获得当前下拉框的选择项的位置
	if (i < 0)
	{
		return 0;
	}
	else
	{
		CString str;
		m_combo.GetLBText(i, str);//根据位置获取信息
		return str;
	}
}

CClientSocket* CMFCServerDlg::GetSendClient(CClientSocket* client)
{
	return client;
}

void CMFCServerDlg::OnEnChangeCount()
{
}

void CMFCServerDlg::AddCount(int count)
{
	CString str;
	str.Format(_T("%d"), count);
	m_clientcount.SetWindowTextW(str);
	//m_listCtrl.InsertItem(0, msg);
}

void CMFCServerDlg::OnBnClickedBtexport()
{
	CString str;
	CFile file;
	file.Open(_T("D://temp_Server.txt"), CFile::modeCreate | CFile::modeWrite);
	int nRow = m_listCtrl.GetItemCount();
	int nCol = m_listCtrl.GetHeaderCtrl()->GetItemCount();
	file.SeekToBegin();
	file.Write("\xff\xfe", 2);
	for (int i = 0; i < nRow; i++)
	{
		str.Empty();
		for (int j = 0; j < nCol; j++)
		{
			CString tmp;
			tmp = m_listCtrl.GetItemText(i, j);
			str += tmp + _T(" ");
		}
		str += "\r\n";
		file.Write(str, str.GetLength() * sizeof(wchar_t));
	}
	file.Flush();
	file.Close();
}
