
// PlaySoundDlg.h : 头文件
//

#pragma once
#include "MCIPlayMusic.h"


// CPlaySoundDlg 对话框
class CPlaySoundDlg : public CDialogEx
{
// 构造
public:
	CPlaySoundDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PLAYSOUND_DIALOG };
	bool PlayMusic();
	void StopPlay();
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
protected:
	afx_msg void OnBnClickedButtonSelfile();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStop();
	LRESULT OnMCINotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnStopMusic(WPARAM wParam, LPARAM lParam);
	//void OpenMciDeveci(void);
	bool PlayMusic(CString &strMusicFilePath)  ;
public:
	int m_nPlayTime;
	HANDLE m_hAutoSTopPlay;
private:
	CString m_strFilePath;
	CMCIPlayMusic m_mciMusic;
	bool m_bPasuePlay;
	
	HANDLE m_hPlayMusic;//播放声音线程
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
