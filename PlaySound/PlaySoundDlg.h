
// PlaySoundDlg.h : ͷ�ļ�
//

#pragma once
#include "MCIPlayMusic.h"


// CPlaySoundDlg �Ի���
class CPlaySoundDlg : public CDialogEx
{
// ����
public:
	CPlaySoundDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PLAYSOUND_DIALOG };
	bool PlayMusic();
	void StopPlay();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	
	HANDLE m_hPlayMusic;//���������߳�
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
