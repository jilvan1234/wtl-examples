// GifControlSampleDlg.h : ͷ�ļ�
//

#pragma once

//
#include"GifControl.h"

// CGifControlSampleDlg �Ի���
class CGifControlSampleDlg : public CDialog
{
// ����
public:
    CGifControlSampleDlg(CWnd* pParent = NULL); // ��׼���캯��

// �Ի�������
    enum { IDD = IDD_GIFCONTROLSAMPLE_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    //
    CGIFControl m_Gif;

// ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnPlay();
    afx_msg void OnBnClickedBtnStop();
};
