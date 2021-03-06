
// PlaySoundDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaySound.h"
#include "PlaySoundDlg.h"
#include "afxdialogex.h"

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


// CPlaySoundDlg 对话框

#define WM_STOPMUSIC WM_USER + 0x01


CPlaySoundDlg::CPlaySoundDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlaySoundDlg::IDD, pParent)
	, m_strFilePath(_T("Z:\\works\\tickets\\pygames\\Alison Krauss - When You Say Nothing at All.mp3"))
	, m_bPasuePlay(false)
	, m_nPlayTime(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlaySoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SOUNDPATH, m_strFilePath);
	DDX_Text(pDX, IDC_EDIT_TIME, m_nPlayTime);
}

BEGIN_MESSAGE_MAP(CPlaySoundDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SELFILE, &CPlaySoundDlg::OnBnClickedButtonSelfile)
	ON_BN_CLICKED(IDC_BUTTON_START, &CPlaySoundDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CPlaySoundDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CPlaySoundDlg::OnBnClickedButtonStop)
	ON_MESSAGE(MM_MCINOTIFY, &CPlaySoundDlg::OnMCINotify)
	ON_MESSAGE(WM_STOPMUSIC, &CPlaySoundDlg::OnStopMusic)
END_MESSAGE_MAP()


// CPlaySoundDlg 消息处理程序

BOOL CPlaySoundDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码 ,无信号
	m_hAutoSTopPlay = CreateEvent(NULL, TRUE, FALSE, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPlaySoundDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPlaySoundDlg::OnPaint()
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
HCURSOR CPlaySoundDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CPlaySoundDlg::OnBnClickedButtonSelfile()
{
	//"Chart Files (*.xlc)|*.xlc|Worksheet Files (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls|All Files (*.*)|*.*||";
	static TCHAR BASED_CODE szFilter[] = _T("mp3 Files (*.mp3)|*.mp3|Mid Files (*.mid)|*.mid|wav Files (*.wav|*.wav|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	if (IDOK == fileDlg.DoModal() )
	{
		m_strFilePath = fileDlg.GetPathName();
		UpdateData(FALSE);
	}
}

UINT __stdcall PlayThread(void *pvoid)
{
	CPlaySoundDlg *pDlag = (CPlaySoundDlg *)pvoid;
	if (NULL== pDlag) return 0;

	DWORD dwRet = WaitForSingleObject(pDlag->m_hAutoSTopPlay, pDlag->m_nPlayTime * 1000 );
	if (dwRet ==  WAIT_OBJECT_0)
	{
		//直接停止不行,需要发送消息
		PostMessage(pDlag->m_hWnd, WM_STOPMUSIC, NULL, NULL);
		//pDlag->StopPlay();
		//重置为无信号状态
		//调用 SetEvent设置有信号之后,需要调用reset设置为无信号
		ResetEvent(pDlag->m_hAutoSTopPlay);
	}
	else //超时停止 或 异常 都停止
	{
		PostMessage(pDlag->m_hWnd, WM_STOPMUSIC, NULL, NULL);
	}

	return 1;
}

void CPlaySoundDlg::OnBnClickedButtonStart()
{
	UpdateData(TRUE);
	if (m_strFilePath.IsEmpty())
	{
		AfxMessageBox(_T("请选择需要播放的文件!"));

		return;
	}

	//创建播放声音线程
	m_hPlayMusic = (HANDLE)_beginthreadex(NULL, 0, PlayThread, this, 0, 0);
	PlayMusic();
	CloseHandle(m_hPlayMusic);
}


void CPlaySoundDlg::OnBnClickedButtonPause()
{
	CString strButtonCaption;
	GetDlgItemText(IDC_BUTTON_PAUSE, strButtonCaption);
	if ( _T("暂停") == strButtonCaption )
	{
		SetDlgItemText(IDC_BUTTON_PAUSE, _T("播放"));
		SetDlgItemText(IDC_STATIC_STATUS, _T("声音文件播放已暂停!!!"));
		m_mciMusic.pause();
		m_bPasuePlay = true;
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_PAUSE, _T("暂停"));
		PlayMusic();
		SetDlgItemText(IDC_STATIC_STATUS, _T("声音文件正在播放..."));
		m_bPasuePlay = false;
	}
	
}


void CPlaySoundDlg::OnBnClickedButtonStop()
{
	/*int nRet = AfxMessageBox(_T("是否停止播放声音?"), MB_YESNO);
	if( IDNO == nRet)
	{
	return ;
	}*/

	DWORD dwRet = m_mciMusic.stop();
	if (0 == dwRet)
	{
		SetDlgItemTextA(m_hWnd, IDC_STATIC_STATUS, "声音文件播放已停止!!!");
	}
	else
	{
		CString strErrMsg = m_mciMusic.getErrorMsg(dwRet) ;
		SetDlgItemText(IDC_STATIC_STATUS, strErrMsg);
	}
}

void CPlaySoundDlg::StopPlay()
{
	m_mciMusic.stop();
	SetDlgItemTextA(m_hWnd, IDC_STATIC_STATUS, "声音文件播放已停止!!!");
}


LRESULT CPlaySoundDlg::OnMCINotify(WPARAM wParam, LPARAM lParam)  
{  
	if (wParam == MCI_NOTIFY_SUCCESSFUL )  
	{  
		PlayMusic(m_strFilePath);  
	}  
	return 0;  
}  


LRESULT CPlaySoundDlg::OnStopMusic( WPARAM wParam, LPARAM lParam )
{
	StopPlay();  

	return 1;
}

//void CPlaySoundDlg::OpenMciDeveci(void)  
//{  
//	DWORD dwResult = 0;  
//
//	dwResult = m_mciMusic.openDevice();  
//	if(0 != dwResult)  
//	{  
//		MessageBox(m_mciMusic.getErrorMsg(dwResult));  
//	}  
//
//}  


bool CPlaySoundDlg::PlayMusic()
{
	return PlayMusic(m_strFilePath);
}

bool CPlaySoundDlg::PlayMusic(CString &strMusicFilePath)  
{  
	if (!m_bPasuePlay)
	{
		m_mciMusic.seekToStart();  
	}
	DWORD dwResult = m_mciMusic.play(this, strMusicFilePath.GetBuffer() );  
	if (dwResult != 0)  
	{  
		MessageBox(m_mciMusic.getErrorMsg(dwResult));  
		SetDlgItemText(IDC_STATIC_STATUS, m_mciMusic.getErrorMsg(dwResult));
		return false;
	}  
	else
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("正在播放声音...") );
	}
	return true;
}  


BOOL CPlaySoundDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return FALSE;
		}
		if (pMsg->wParam == 'S')
		{
			//设置信号
			SetEvent(m_hAutoSTopPlay);
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}