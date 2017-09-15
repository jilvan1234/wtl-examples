// downloadlistdlg.cpp : implementation of the CDownloadListDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "downloadlistdlg.h"

LRESULT CDownloadListDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());
    m_DownloadListView.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | LVS_REPORT, LVS_EX_FULLROWSELECT, (HMENU)IDC_DOWNLOAD_LISTVIEW);
//  InitListView( &m_DownloadListView , DOWNLOADLISTVIEW);
//  m_DownloadListView.Attach(GetDlgItem(IDC_LIST_DOWNLOAD));
    m_DownloadListView.InsertColumn(0, L"״̬", LVCFMT_LEFT, 100, 0);
    m_DownloadListView.InsertColumn(1, L"ר����", LVCFMT_LEFT, 100, 0);
    m_DownloadListView.InsertColumn(2, L"ר������", LVCFMT_LEFT, 200, 0);
    m_DownloadListView.InsertColumn(3, L"����", LVCFMT_LEFT, 100, 0);
    m_DownloadListView.InsertColumn(4, L"��Դ", LVCFMT_LEFT, 50, 0);
    m_DownloadListView.InsertColumn(5, L"ҳ��", LVCFMT_LEFT, 50, 0);
    m_DownloadListView.InsertColumn(6, L"������", LVCFMT_LEFT, 100, 0);
    m_DownloadListView.InsertColumn(7, L"������", LVCFMT_LEFT, 100, 0);
    m_DownloadListView.InsertColumn(8, L"������", LVCFMT_LEFT, 100, 0);
    DWORD dwStyle = m_DownloadListView.GetExtendedListViewStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_DownloadListView.SetExtendedListViewStyle(dwStyle);
    m_DownloadListView.InsertItem(0, L"");
    m_DownloadListView.SetItemText(0, 1, L"hello");
    m_DownloadListView.SetItemText(0, 2, L"world");
    return TRUE;
}

LRESULT CDownloadListDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    //EndDialog(wID);
    AtlMessageBox(NULL, L"hello");
    return 0;
}

LRESULT CDownloadListDlg::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    RECT rect;
    //  GetClientRect(&rect);
    GetWindowRect(&rect);
    int height = rect.bottom - rect.top;
    int width = rect.right - rect.left;
    //GetDlgItem(IDC_SEARCH_LISTVIEW).MoveWindow(0, 50, 2800, height-100);
    GetDlgItem(IDC_DOWNLOAD_LISTVIEW).MoveWindow(0, 50, 2800, height);
    return 0;
}

LRESULT CDownloadListDlg::OnLvnItemchangedListDownload(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    /*
    �жϣ�
    ���������ѡ�У� ɾ����ť��ʾ
        ������������ص� ֹͣ��ť ��ʾ
        �������ֹͣ�����񣬿�ʼ��ť ��ʾ
    ��û������ѡ�У�������ť��Ϊfalse

    */
    CString status_name;

    if (m_DownloadListView.GetSelectedCount()) {
        int nItemCount = m_DownloadListView.GetItemCount(); //��������

        for (int i = nItemCount - 1; i >= 0; i--) {
            int iState = m_DownloadListView.GetItemState(i, LVIS_SELECTED);

            if (iState != 0) {
                m_DownloadListView.GetItemText(i, 0, status_name);

                if (status_name == _T("��ֹͣ")) {
                    GetDlgItem(ID_DOWNLOAD_START).EnableWindow(TRUE);
                } else if (status_name == _T("��������")) {
                    GetDlgItem(ID_DOWNLOAD_STOP).EnableWindow(TRUE);
                } else {
                    GetDlgItem(ID_DOWNLOAD_START).EnableWindow(TRUE);
                    GetDlgItem(ID_DOWNLOAD_STOP).EnableWindow(TRUE);
                }
            }
        }

        GetDlgItem(ID_DOWNLOAD_DEL).EnableWindow(TRUE);
    } else {
        GetDlgItem(ID_DOWNLOAD_START).EnableWindow(FALSE);
        GetDlgItem(ID_DOWNLOAD_STOP).EnableWindow(FALSE);
        GetDlgItem(ID_DOWNLOAD_DEL).EnableWindow(FALSE);
    }

    return 0;
}

LRESULT CDownloadListDlg::OnNMRclickListDownload(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CMenu obMenu ;
    HMENU m_hMenu = NULL;
    CPoint obCursorPoint = (0, 0);
    GetCursorPos(&obCursorPoint);
    obMenu.LoadMenu(IDR_DOWNLOAD_POP);
    m_hMenu = obMenu.GetSubMenu(0);

    // Track the popup menu
    if (m_hMenu != NULL)
        TrackPopupMenu(m_hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, obCursorPoint.x, obCursorPoint.y, 0, this->m_hWnd, &rcDefault);

    return 0;
}

LRESULT CDownloadListDlg::OnSelectAll(WORD , WORD , HWND , BOOL&)
{
    int nItemCount = m_DownloadListView.GetItemCount();//��������

    for (int i = nItemCount - 1; i >= 0 ; i--) {
        m_DownloadListView.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
    }

    return 0;
}

LRESULT CDownloadListDlg::OnReSelect(WORD , WORD , HWND , BOOL&)
{
    int nItemCount = m_DownloadListView.GetItemCount();//��������

    for (int i = nItemCount - 1; i >= 0 ; i--) {
        int istate = m_DownloadListView.GetItemState(i, LVIS_SELECTED);

        if (0 != istate) {
            // �����ǰѡ�У�����ѡ��
            m_DownloadListView.SetItemState(i, 0, LVIS_SELECTED);
        } else
            m_DownloadListView.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
    }

    return 0;
}
