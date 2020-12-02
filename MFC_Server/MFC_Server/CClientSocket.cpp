#include "pch.h"
#include "CClientSocket.h"
#include "MFC_Server.h"
#include "MFC_ServerDlg.h"
//初始化，将对话框指针dlg指向m_dlg
CClientSocket::CClientSocket(CMFCServerDlg* dlg)
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
	CString msg;
	//从客户端列表中删除
	msg.Format(_T("客户端(%s:%d): 已关闭!"),m_ip, m_port);
	m_dlg->AddMsg(msg);
	CSocket::OnClose(nErrorCode);
	m_dlg->CloseClient(this);
}

//服务器接收来自客户端的消息
void CClientSocket::OnReceive(int nErrorCode)
{
	// 本函数由框架调用，通知套接字缓冲中有数据，可以调用Receive成员函数取出
	char recvBuf[512] = { 0 };
	int recvLen = this->Receive(recvBuf, sizeof(recvBuf));
	CString msg;
	SYSTEMTIME t;
	GetLocalTime(&t);
	msg.Format(_T("客户端(%s:%d): %s"), m_ip, m_port,CString(recvBuf));
	m_dlg->AddMsg(msg);
	// 回射信息
	/*
	CharUpperA(recvBuf);
	this->Send(recvBuf, strlen(recvBuf));
	*/
}

//服务器与客户端建立连接后回送消息
void CClientSocket::OnSend(int nErrorCode)
{
	// 本函数由框架调用，通知套接字现在可以调用Send成员函数发送数据了。
	char buf[] = "服务器连接成功";
	Send(buf, strlen(buf)); //给客户端发送信息

	CSocket::OnSend(nErrorCode);
}


void CClientSocket::SetClientAddr(CString ip, USHORT port)
{
	m_ip = ip;
	m_port = port;
}
