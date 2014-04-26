
// PassGameProtectionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PassGameProtection.h"
#include "PassGameProtectionDlg.h"
#include "afxdialogex.h"
#include "winsvc.h"
#include <winioctl.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CPassGameProtectionDlg 对话框
CPassGameProtectionDlg::CPassGameProtectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassGameProtectionDlg::IDD, pParent)
	, m_strSysPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPassGameProtectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SYSPATH, m_strSysPath);
	DDX_Control(pDX, IDC_BTN_START, m_btnStartSys);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStopSys);
	DDX_Control(pDX, IDC_BTN_START_SERVICE, m_btnStartService);
	DDX_Control(pDX, IDC_BTN_STOP_SERVICE, m_btnStopService);
	DDX_Control(pDX, IDC_BTN_STARTDEBUG, m_btnStartDebug);
	DDX_Control(pDX, IDC_BTN_STOPDEBUG, m_btnStopDebug);
}

BEGIN_MESSAGE_MAP(CPassGameProtectionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_FIND_SYSPATH, &CPassGameProtectionDlg::OnBnClickedBtnFindSyspath)
	ON_BN_CLICKED(IDC_BTN_START, &CPassGameProtectionDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CPassGameProtectionDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_START_SERVICE, &CPassGameProtectionDlg::OnBnClickedBtnStartService)
	ON_BN_CLICKED(IDC_BTN_STOP_SERVICE, &CPassGameProtectionDlg::OnBnClickedBtnStopService)
	ON_BN_CLICKED(IDC_BTN_STARTDEBUG, &CPassGameProtectionDlg::OnBnClickedBtnStartdebug)
	ON_BN_CLICKED(IDC_BTN_STOPDEBUG, &CPassGameProtectionDlg::OnBnClickedBtnStopdebug)
END_MESSAGE_MAP()


// CPassGameProtectionDlg 消息处理程序
BOOL CPassGameProtectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPassGameProtectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPassGameProtectionDlg::OnPaint()
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
HCURSOR CPassGameProtectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//查找驱动文件
void CPassGameProtectionDlg::OnBnClickedBtnFindSyspath()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(
		TRUE, 
		NULL, 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		NULL, 
		this);
	if (dlg.DoModal() == IDOK)
	{
		m_strSysPath = dlg.GetPathName();
		m_strSysName = dlg.GetFileName();
		UpdateData(FALSE);
	}
}

//开启驱动
void CPassGameProtectionDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_strSysPath = m_strSysPath.Trim();
	if (!m_strSysPath.IsEmpty() && LoadDriver())
	{
		hDevice = GetDevice();
		if (hDevice != INVALID_HANDLE_VALUE)
		{
			//改变按钮状态
			m_btnStartSys.EnableWindow(FALSE);
			m_btnStartService.EnableWindow(TRUE);
			m_btnStopService.EnableWindow(FALSE);
			m_btnStopSys.EnableWindow(TRUE);
			return;
		}		
	}

	MessageBox("请选择正确的驱动程序","警告",MB_OK|MB_ICONHAND);
}

//加载驱动
BOOL CPassGameProtectionDlg::LoadDriver()
{
	BOOL bRet = FALSE;
	SC_HANDLE hServiceDDK = NULL; //驱动程序的服务句柄
	SC_HANDLE hServiceMgr = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);//服务管理器句柄
	if (hServiceMgr == NULL)
	{
		return bRet;
	}
	hServiceDDK=CreateService(
		hServiceMgr,//管理器句柄
		m_strSysName.LockBuffer(),	//驱动在注册表中的名字
		m_strSysName.LockBuffer(),	//注册表驱动程序的 DisplayName值
		SERVICE_ALL_ACCESS,			//加载驱动的访问权限
		SERVICE_KERNEL_DRIVER,		//表示加载的是驱动
		SERVICE_DEMAND_START,		// 注册表驱动程序的 Start 值  
		SERVICE_ERROR_IGNORE,		// 注册表驱动程序的 ErrorControl 值  
		m_strSysPath.LockBuffer(),	// 注册表驱动程序的 ImagePath 值  
		NULL,	//要开启服务的 用户组
		NULL,	//输出验证标签
		NULL,	//所依赖的服务的名称
		NULL,   //用户账户名称
		NULL);
	if (hServiceDDK == NULL)
	{
		//如果创建失败,则直接打开
		hServiceDDK = OpenService(hServiceMgr, m_strSysName.LockBuffer(), SERVICE_ALL_ACCESS);
		if (hServiceDDK == NULL)
		{
			CloseServiceHandle(hServiceMgr);
			return bRet;
		}
	}
	if (StartService(hServiceDDK,NULL,NULL) != NULL)
	{
		bRet = TRUE;
	}

	CloseServiceHandle(hServiceDDK);
	CloseServiceHandle(hServiceMgr);
	return bRet;
}

//停止驱动
void CPassGameProtectionDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	UnLoadDriver();
	CloseHandle(hDevice);
	//改变按钮状态
	m_btnStartSys.EnableWindow(TRUE);
	m_btnStartService.EnableWindow(FALSE);
	m_btnStopService.EnableWindow(FALSE);
	m_btnStopSys.EnableWindow(FALSE);
}

//卸载驱动
void CPassGameProtectionDlg::UnLoadDriver()
{
	SERVICE_STATUS SvrSta;
	SC_HANDLE hServiceDDK = NULL; //驱动程序的服务句柄
	SC_HANDLE hServiceMgr = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);	//服务管理器句柄
	if (hServiceMgr == NULL)
	{
		return;
	}
	//打开所要关闭的驱动所对应的服务
	hServiceDDK=OpenService(hServiceMgr,m_strSysName,SERVICE_ALL_ACCESS);
	if (hServiceDDK==NULL)
	{
		return;
	}
	//停止服务
	if (!ControlService(hServiceDDK,SERVICE_CONTROL_STOP,&SvrSta))
	{
		return;
	}
	//删除服务
	if(!DeleteService(hServiceDDK))
	{
		return;
	}
	if (hServiceDDK!=NULL)
	{
		//关闭打开的驱动句柄
		CloseServiceHandle(hServiceDDK);
	}
	if (hServiceMgr!=NULL)
	{
		//关闭服务管理器句柄
		CloseServiceHandle(hServiceMgr);
	}
}

//获取驱动句柄
HANDLE CPassGameProtectionDlg::GetDevice()
{
	hDevice = CreateFile("\\\\.\\MyDriverLinkName",//\\??\\MyDriverLinkName指向文件名的指针 
		GENERIC_READ|GENERIC_WRITE,	//访问模式（写/读） 
		0,							//共享模式
		NULL,						//指向安全属性的指针
		OPEN_EXISTING,				//如何创建
		FILE_ATTRIBUTE_NORMAL,		//文件属性
		NULL);						//用于复制文件句柄 
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("获取驱动句柄失败，Eorror code is :%d\n",GetLastError());
		getchar();
		return INVALID_HANDLE_VALUE;
	}
	return hDevice;
}

//向驱动文件发送命令
BOOL CPassGameProtectionDlg::SendIRP(int code,LPVOID inBuffer,DWORD intBufferSize,LPVOID outBuffer,DWORD outBufferSize)
{
	DWORD lpBytesReturned;

	return DeviceIoControl(hDevice,code,inBuffer,intBufferSize,outBuffer,outBufferSize,&lpBytesReturned,NULL);
}

//开启服务
void CPassGameProtectionDlg::OnBnClickedBtnStartService()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!SendIRP(hook_tp,NULL,0,NULL,0))
	{
		MessageBox("操作失败","Error!",MB_OK);
		return;
	}
	m_btnStartSys.EnableWindow(FALSE);
	m_btnStartService.EnableWindow(FALSE);
	m_btnStopService.EnableWindow(TRUE);
	m_btnStopSys.EnableWindow(TRUE);
}

//停止服务
void CPassGameProtectionDlg::OnBnClickedBtnStopService()
{
	// TODO: 在此添加控件通知处理程序代码
	if (hDevice==INVALID_HANDLE_VALUE)
	{
		MessageBox( "连接驱动设备失败","Error",MB_OK|MB_ICONHAND);
		return;
	}
	if (!SendIRP(unhook_tp,NULL,0,NULL,0))
	{
		MessageBox("操作失败","Error",MB_OK);
		return;
	}
	m_btnStartSys.EnableWindow(FALSE);
	m_btnStartService.EnableWindow(TRUE);
	m_btnStopService.EnableWindow(FALSE);
	m_btnStopSys.EnableWindow(TRUE);
}

//开启双机调试模式
void CPassGameProtectionDlg::OnBnClickedBtnStartdebug()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!SendIRP(hook_windbg,NULL,0,NULL,0))
	{
		MessageBox("操作失败","Error",MB_OK);
		return;
	}
}

//停止双机调试
void CPassGameProtectionDlg::OnBnClickedBtnStopdebug()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!SendIRP(unhook_windbg,NULL,0,NULL,0))
	{
		MessageBox("操作失败","Error",MB_OK);
		return;
	}
}
