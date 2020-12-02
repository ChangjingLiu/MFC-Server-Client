#pragma once
#include <afxsock.h>
class CMFCClientDlg;
class CClientSocket :
    public CSocket //继承CSocket类
{
public:
    CClientSocket(CMFCClientDlg* dlg = NULL); //构造函数
    virtual~CClientSocket();                //析构函数
    virtual void OnClose(int nErrorCode);     //服务器关闭而关闭客户端 
    virtual void OnReceive(int nErrorCode);   //接收来自服务器的消息
    bool Connect(LPCTSTR lpszHostAddress, UINT nHostPort);//连接服务器
private:
    UINT m_port;
    CString m_ip;
    CMFCClientDlg* m_dlg;
};

