
// MFC_ClientDlg.h: 头文件
//
#include"CClientSocket.h"
#pragma once


// CMFCClientDlg 对话框
class CMFCClientDlg : public CDialogEx
{
// 构造
public:
	CMFCClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_CLIENT_DIALOG };
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
public:
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangePort();
	afx_msg void OnBnClickedConnbtn();
	afx_msg void OnBnClickedDisconnbtn();
	afx_msg void OnEnChangeSendbox();
	afx_msg void OnBnClickedSend();
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtexport();
	void AddMsg(CString msg);
	void MyEnableBtn();
private:
	CButton m_connBtn;
	CClientSocket m_clientSocket;
	CButton m_disconnBtn;
	CIPAddressCtrl m_ip;
	CListCtrl m_listCtrl;
	CButton m_sendBtn;
	CString m_sendBuf;
	UINT m_port;
};
