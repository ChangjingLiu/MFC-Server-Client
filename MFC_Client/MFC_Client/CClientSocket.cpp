#include "pch.h"
#include "CClientSocket.h"
#include "MFC_Client.h"
#include "MFC_ClientDlg.h"


CClientSocket::CClientSocket(CMFCClientDlg* dlg)
{
	m_dlg = dlg;
}

//析构函数，用于释放内存
CClientSocket::~CClientSocket()
{
}

//服务器关闭时关闭客户端
void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString msg;
	msg.Format(_T("服务器(%s:%d): 服务器关闭!"), m_ip, m_port);
	m_dlg->AddMsg(msg);
	m_dlg->MyEnableBtn();

	this->Close();
	CSocket::OnClose(nErrorCode);
}

//客户端接收来自服务器的消息
void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	char recvBuf[512] = { 0 };
	int recvLen = this->Receive(recvBuf, sizeof(recvBuf));
	CString msg;
	//SYSTEMTIME t;
	//GetLocalTime(&t);
	msg.Format(_T("服务器(%s:%d): %s"), m_ip, m_port, CString(recvBuf));
	//msg.Format(_T("服务器: %s"),CString(recvBuf));
	m_dlg->AddMsg(msg);

	CSocket::OnReceive(nErrorCode);
}

//客户端连接服务器
bool CClientSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	// TODO: 在此处添加实现代码.
	m_ip = lpszHostAddress;
	m_port = nHostPort;
	return CAsyncSocket::Connect(lpszHostAddress, nHostPort);
}

