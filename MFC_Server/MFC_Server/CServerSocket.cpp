#include "pch.h"
#include "CServerSocket.h"
//#include "CClientSocket.h"
#include "MFC_Server.h"
#include "MFC_ServerDlg.h"

//初始化，将对话框指针dlg指向m_dlg
CServerSocket::CServerSocket(CMFCServerDlg* dlg)
{
	//初始化时调用构造函数
	m_dlg = dlg;
}
//析构函数，用于释放内存
CServerSocket::~CServerSocket()
{
	
}
//服务器接受客户端的连接
void CServerSocket::OnAccept(int nErrorCode)
{
	// 由框架调用，通知监听套接字现在可以调用Accept成员函数来接收悬挂的（pending）连接请求。
	CClientSocket* client = new CClientSocket(m_dlg); 
	// 新建CClientSocket对象，构造函数连接对话框
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	int addrLen = sizeof(addr);

	// 获取通信套接字
	Accept(*client, (SOCKADDR*)&addr, &addrLen);
	char* ip = inet_ntoa(addr.sin_addr);
	client->SetClientAddr(CString(ip), addr.sin_port);
	//加入列表
	m_clientList.push_back(client);
	CString msg;
	//msg.Format(_T("客户端(%d): 数量"), m_clientList.size());
	//m_dlg->AddMsg(msg);
	// 界面添加连接消息
	//CString msg;
	//SYSTEMTIME t;
	//GetLocalTime(&t);
	msg.Format(_T("客户端(%s:%d): 连接成功!"),CString(ip), addr.sin_port);
	m_dlg->AddMsg(msg);

	//清除之前的combo
	m_dlg->CleanCombo();
	//更新combo
	UpdateCombo();
	//更新连接数量
	m_dlg->AddCount(m_clientList.size());

	CSocket::OnAccept(nErrorCode);
}
//某一客户端关闭后，清除服务器中的相关的信息
void CServerSocket::CloseSpecialClient(CClientSocket*  pointer) {
	//USHORT m_port1
	CString msg;
	//msg.Format(_T("服务器收到删除命令"));
	//m_dlg->AddMsg(msg);
	list<CClientSocket*>::iterator it = m_clientList.begin();
	//遍历寻找对应指针以清除
	for (; it != m_clientList.end();it++ )
	{
		//(*it).m_port == m_port1
		if (*it==pointer) {
			CString msg;
			//msg.Format(_T("查找到了嘿嘿"));
			//m_dlg->AddMsg(msg);
			(*it)->Close();
			delete (*it);
			it = m_clientList.erase(it); //删除
			break;
		}
	}
	//清除之前的combo
	m_dlg->CleanCombo();
	//更新combo
	UpdateCombo();
	//更新连接数量
	m_dlg->AddCount(m_clientList.size());
}
//服务器关闭，使所有连接断开
void CServerSocket::CloseAllClient()
{
	// 关闭监听套接字，先关闭连接的套接字
	list<CClientSocket*>::iterator it = m_clientList.begin();
	//遍历清除
	for (; it != m_clientList.end(); )
	{
		
		(*it)->Close();
		delete (*it);
		it = m_clientList.erase(it);
	}

	//清除之前的combo
	m_dlg->CleanCombo();
	//更新combo
	UpdateCombo();

	//更新连接数量
	m_dlg->AddCount(m_clientList.size());
	this->Close();
}
/*
//某一客户端关闭，清除服务器中的socket
void CServerSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString msg;
	msg.Format(_T("客户端: 服务端已关闭!"));
	m_dlg->AddMsg(msg);
	m_dlg->AddCount(m_clientList.size());
	CSocket::OnClose(nErrorCode);
}
*/
//更新下拉式框的信息
void CServerSocket::UpdateCombo()
{
	//清除cmobo之前的信息
	//m_dlg->CleanCombo();
	//m_dlg->m_combo.ResetContent();
	list<CClientSocket*>::iterator it = m_clientList.begin();
	for (; it != m_clientList.end(); it++)
	{
		//将客户端信息添加到combo
		m_dlg->AddClientAddrs(*it);
	}
	
}
//服务器向指定客户端发送消息
bool CServerSocket::SendMsg(CString str,char* send)
{
	list<CClientSocket*>::iterator it = m_clientList.begin();
	for (; it != m_clientList.end(); it++)
	{
		CString msg;
		msg.Format(_T("%s:%d"), (*it)->m_ip, (*it)->m_port);
		if (msg == str) {
			(*it)->Send(send,strlen(send));
			return 1;
		}
	}
	m_dlg->MessageBox(_T("找不到客户端，发送失败！"));
	return 0;
}

