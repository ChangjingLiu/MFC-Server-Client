#pragma once
//#include <afxsock.h>
#include"CClientSocket.h"

#include<list>
using namespace std;
// CServerSocket 命令目标
class CMFCServerDlg;
class CServerSocket :
    public CSocket
{
public:
    CServerSocket(CMFCServerDlg* dlg = NULL); // 构造函数
    virtual ~CServerSocket();
    //接受到客户端连接的 回调函数
    virtual void OnAccept(int nErrorCode);
    //关闭所有连接Client的socket
    void CloseAllClient();
    void CloseSpecialClient(CClientSocket* pointer);
    void UpdateCombo();
    bool SendMsg(CString str, char* send);
    //virtual void OnClose(int nErrorCode);
private:
    CMFCServerDlg* m_dlg;
    list<CClientSocket*> m_clientList;
    //客户端列表
};

