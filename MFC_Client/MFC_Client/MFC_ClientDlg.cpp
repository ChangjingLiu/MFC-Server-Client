
// MFC_ClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFC_Client.h"
#include "MFC_ClientDlg.h"
#include "afxdialogex.h"
//设置保存为ASCII编码文件
#include "locale.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCClientDlg 对话框



CMFCClientDlg::CMFCClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_CLIENT_DIALOG, pParent)
	, m_port(0)
	, m_sendBuf(_T(""))
	, m_clientSocket(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONNBTN, m_connBtn);
	DDX_Control(pDX, IDC_DISCONNBTN, m_disconnBtn);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_SEND, m_sendBtn);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Text(pDX, IDC_SENDBOX, m_sendBuf);
	DDX_Text(pDX, IDC_PORT, m_port);
}

BEGIN_MESSAGE_MAP(CMFCClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CMFCClientDlg::OnIpnFieldchangedIpaddress1)
	ON_EN_CHANGE(IDC_PORT, &CMFCClientDlg::OnEnChangePort)
	ON_BN_CLICKED(IDC_CONNBTN, &CMFCClientDlg::OnBnClickedConnbtn)
	ON_BN_CLICKED(IDC_DISCONNBTN, &CMFCClientDlg::OnBnClickedDisconnbtn)
	ON_EN_CHANGE(IDC_SENDBOX, &CMFCClientDlg::OnEnChangeSendbox)
	ON_BN_CLICKED(IDC_SEND, &CMFCClientDlg::OnBnClickedSend)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMFCClientDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BTEXPORT, &CMFCClientDlg::OnBnClickedBtexport)
END_MESSAGE_MAP()


// CMFCClientDlg 消息处理程序

BOOL CMFCClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//设定默认的IP和Port
	m_listCtrl.InsertColumn(0, _T("消息列表"));
	CRect rcListCtrl;
	m_listCtrl.GetClientRect(&rcListCtrl);
	m_listCtrl.SetColumnWidth(0, rcListCtrl.Width());

	m_ip.SetAddress(127, 0, 0, 1);
	m_port = 8080;
	UpdateData(FALSE);
	m_disconnBtn.EnableWindow(FALSE);
	m_sendBtn.EnableWindow(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCClientDlg::OnPaint()
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
HCURSOR CMFCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCClientDlg::OnIpnFieldchangedIpaddress1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMFCClientDlg::OnEnChangePort()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

//连接服务器
void CMFCClientDlg::OnBnClickedConnbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BYTE a, b, c, d;
	m_ip.GetAddress(a, b, c, d);
	CString ip;
	ip.Format(_T("%d.%d.%d.%d"), a, b, c, d);
	BOOL ret = m_clientSocket.Create();
	if (ret)
	{
		ret = m_clientSocket.Connect(ip, m_port);
		if (ret)
		{
			AddMsg(_T("服务器连接成功!"));
			m_connBtn.EnableWindow(FALSE);
			m_disconnBtn.EnableWindow(TRUE);
			m_sendBtn.EnableWindow(TRUE);
		}
		else
		{
			MessageBox(_T("连接服务器失败"));
			m_clientSocket.Close();
		}
	}
	else
	{
		MessageBox(_T("创建socket失败"));
	}
}

//断开服务器
void CMFCClientDlg::OnBnClickedDisconnbtn()
{
	m_connBtn.EnableWindow(TRUE);
	m_disconnBtn.EnableWindow(FALSE);
	m_sendBtn.EnableWindow(FALSE);
	m_clientSocket.Close();
	AddMsg(_T("服务器断开连接!"));
}


void CMFCClientDlg::OnEnChangeSendbox()
{

}

void CMFCClientDlg::OnBnClickedSend()
{
	UpdateData(TRUE);
	USES_CONVERSION;
	//函数T2A和W2A均支持ATL和MFC中的字符
	char* send = T2A(m_sendBuf);
	BOOL ret = m_clientSocket.Send(send, strlen(send));
	if (ret>0)
	{
		AddMsg(_T("发送的消息为:") + m_sendBuf);
	}
	else
	{
		MessageBox(_T("发送失败"));
	}
	//清空发送窗口
	SetDlgItemText(IDC_SENDBOX, _T(""));
	
}


void CMFCClientDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CMFCClientDlg::AddMsg(CString msg)
{
	int nRow = m_listCtrl.GetItemCount();
	m_listCtrl.InsertItem(nRow, msg);
}

void CMFCClientDlg::MyEnableBtn()
{
	m_connBtn.EnableWindow(TRUE);
	m_disconnBtn.EnableWindow(FALSE);
	m_sendBtn.EnableWindow(FALSE);
}


void CMFCClientDlg::OnBnClickedBtexport()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	//文件打开
	/*
	CStdioFile file;
	CFileException exp;
	*/
	CFile file;
	//char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	//setlocale(LC_CTYPE, "chs");
	file.Open(_T("D://temp_client.txt"), CFile::modeCreate | CFile::modeWrite);
	int nRow = m_listCtrl.GetItemCount();
	int nCol = m_listCtrl.GetHeaderCtrl()->GetItemCount();

	//
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
		file.Write(str, str.GetLength()*sizeof(wchar_t));
		
	}
	file.Flush();
	file.Close();
	/*setlocale(LC_CTYPE, old_locale); //还原语言区域的设置
	free(old_locale);//还原区域设定*/
	
}
