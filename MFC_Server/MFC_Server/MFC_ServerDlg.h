
// MFC_ServerDlg.h: 头文件
//

#pragma once
//#include "afxwin.h"
//#include "afxcmn.h"
#include "CServerSocket.h"


// CMFCServerDlg 对话框
class CMFCServerDlg : public CDialogEx
{
// 构造
public:
	CMFCServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	UINT m_port;
	CButton m_onBtn;
	CButton m_offBtn;
	CListCtrl m_listCtrl;
	CComboBox m_combo;
	CServerSocket m_serverSocket;
	CClientSocket m_clientSocket; //测试
	CButton m_sendBtn;
	CString m_sendBuf;
	//CComboBox m_clientaddrs;
	CIPAddressCtrl m_ip;
public:
	afx_msg void OnBnClickedBtonserver();
	afx_msg void OnBnClickedBtoffserver();
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangePort();
	afx_msg void OnEnChangeSendbox();
	afx_msg void OnBnClickedBtsend();
	//afx_msg void OnCbnSelchangeCombo1();
	void AddMsg(CString msg);
	void AddCount(int count);
    void AddClientAddrs(CClientSocket* oneclient);
	void CloseClient(CClientSocket* client);
	afx_msg void OnCbnSelchangeCombo1();
	void CleanCombo();
	CString GetComboText();
	CClientSocket* GetSendClient(CClientSocket* client);
	//afx_msg void OnEnChangeEditcount();
	afx_msg void OnEnChangeCount();
private:
	CEdit m_clientcount;
public:
	afx_msg void OnBnClickedBtexport();
};

