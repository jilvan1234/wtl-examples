
// MiniNewsDlg.h : ͷ�ļ�
//

#pragma once
#include "explorer1.h"


// CMiniNewsDlg �Ի���
class CMiniNewsDlg : public CDialog
{
// ����
public:
	CMiniNewsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MININEWS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
	//caizhiming

	//�Ի���������߿���С����ť����󻯰�ť�͹رհ�ť�Ƚ����������
	int m_nWinWidth;//���ڿ�ȣ������߿�
	int m_nWinHeight;//���ڸ߶ȣ�
	int m_nFrameCY;//��ȡ�Ի��߿�ĸ߶�
	int m_nFrameCX;//��ȡ�Ի��߿�Ŀ��
	int m_nBorderCY;//�Ի���߿�߶�
	int m_nBorderCX;//�Ի���߿���
	int m_nTitleBarCY ;//�Ի���������߶�
	int m_nTitleBarCX;//�Ի�����������
	bool m_bMouseOnMinBtn;//����Ƿ�����С����ť������
	bool m_bMouseOnCloseBtn;//����Ƿ��ڹرհ�ť������
	bool m_bMouseOnMoreBtn;//����Ƿ��ڸ��ఴť������
	int m_nWinTitleHeight;//�������߶ȣ����˵����߰�ť��

	void DrawDialog();//���ƶԻ�������
	CString m_strTitle;
	COLORREF m_bkColor;//�Ի��򱳾���ɫ
	void SetTitleText(CString strTitle);
	void DisplayBmp(int x,int y,int w,int h,int nID); //��ָ��λ����ʾbmpλͼͼ��
	void DrawTitleText();
public:
	CExplorer1 m_webBrowser;
// ʵ��
protected:
	HICON m_hIcon;


	afx_msg void OnNcPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);

	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseLeave();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

};
