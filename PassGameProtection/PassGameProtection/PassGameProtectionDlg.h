
// PassGameProtectionDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#define hook_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define unhook_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

// CPassGameProtectionDlg 对话框
class CPassGameProtectionDlg : public CDialogEx
{
// 构造
public:
	CPassGameProtectionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PASSGAMEPROTECTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	HANDLE GetDevice();
	BOOL LoadDriver();	
	void UnLoadDriver();
	BOOL SendIRP(int code,LPVOID inBuffer,DWORD intBufferSize,LPVOID outBuffer,DWORD outBufferSize);

private:
	HANDLE hDevice;
public:
	afx_msg void OnBnClickedBtnFindSyspath();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	CString m_strSysPath;
	CString m_strSysName;
	CButton m_btnStartSys;
	CButton m_btnStopSys;
	CButton m_btnStartService;
	CButton m_btnStopService;
	afx_msg void OnBnClickedBtnStartService();
	afx_msg void OnBnClickedBtnStopService();
};
