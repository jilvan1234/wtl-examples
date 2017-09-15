// infolistdlg.cpp : implementation of the CInfoListDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "infolistdlg.h"

LRESULT CInfoListDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());
//  m_InfoListView.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | LVS_REPORT, LVS_EX_FULLROWSELECT,(HMENU)IDC_INFO_LISTVIEW);
//  InitListView( &m_InfoListView , DOWNLOADLISTVIEW);
    m_InfoListView.Attach(GetDlgItem(IDC_LIST_INFO));
    m_InfoListView.InsertColumn(0, L"���", LVCFMT_LEFT, 50, 0);
    m_InfoListView.InsertColumn(1, L"ר����", LVCFMT_LEFT, 100, 0);
    m_InfoListView.InsertColumn(2, L"ר������", LVCFMT_LEFT, 240, 0);
    m_InfoListView.InsertColumn(3, L"��Դ", LVCFMT_LEFT, 50, 0);
    m_InfoListView.InsertColumn(4, L"ҳ��", LVCFMT_LEFT, 50, 0);
    m_InfoListView.InsertColumn(5, L"ȫ��", LVCFMT_LEFT, 50, 0);
    m_InfoListView.InsertColumn(6, L"����״̬", LVCFMT_LEFT, 80, 2);
    m_InfoListView.InsertColumn(7, L"����ʱ��", LVCFMT_LEFT, 100, 2);
    DWORD dwStyle = m_InfoListView.GetExtendedListViewStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_InfoListView.SetExtendedListViewStyle(dwStyle);
    return TRUE;
}

LRESULT CInfoListDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    //EndDialog(wID);
    AtlMessageBox(NULL, L"hello");
    return 0;
}

LRESULT CInfoListDlg::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    RECT rect;
    GetWindowRect(&rect);
    int height = rect.bottom - rect.top;
    int width = rect.right - rect.left;
    GetDlgItem(IDC_LIST_INFO).MoveWindow(0, 50, 2800, height);
    return 0;
}

LRESULT CInfoListDlg::OnLvnItemchangedListDownload(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    /*
    �жϣ�
    ���������ѡ�У� ����ѡ�� ��ť��ʾ
    ����ǵ�һҳ�� �� ��ҳ����һҳ ��ťΪFALSE
    �����βҳ�� �� ��һҳ��βҳ ��ťΪFALSE
    */
    CString status_name;

    if (m_InfoListView.GetSelectedCount()) {
        GetDlgItem(ID_INFO_OPEN).EnableWindow(TRUE);
        GetDlgItem(ID_EXPORT).EnableWindow(TRUE);
        GetDlgItem(ID_INFO_DEL).EnableWindow(TRUE);
    } else {
        GetDlgItem(ID_INFO_OPEN).EnableWindow(FALSE);
        GetDlgItem(ID_EXPORT).EnableWindow(FALSE);
        GetDlgItem(ID_INFO_DEL).EnableWindow(FALSE);
    }

    return 0;
}

LRESULT CInfoListDlg::OnNMRclickListDownload(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CMenu obMenu ;
    HMENU m_hMenu = NULL;
    CPoint obCursorPoint = (0, 0);
    GetCursorPos(&obCursorPoint);
    obMenu.LoadMenu(IDR_INFO_POP);
    m_hMenu = obMenu.GetSubMenu(0);

    // Track the popup menu
    if (m_hMenu != NULL)
        TrackPopupMenu(m_hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, obCursorPoint.x, obCursorPoint.y, 0, this->m_hWnd, &rcDefault);

    return 0;
}

LRESULT CInfoListDlg::OnSelectAll(WORD , WORD , HWND , BOOL&)
{
    int nItemCount = m_InfoListView.GetItemCount();//��������

    for (int i = nItemCount - 1; i >= 0 ; i--) {
        m_InfoListView.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
    }

    return 0;
}

LRESULT CInfoListDlg::OnReSelect(WORD , WORD , HWND , BOOL&)
{
    int nItemCount = m_InfoListView.GetItemCount();//��������

    for (int i = nItemCount - 1; i >= 0 ; --i) {
        int istate = m_InfoListView.GetItemState(i, LVIS_SELECTED);

        if (0 != istate) {
            // �����ǰѡ�У�����ѡ��
            m_InfoListView.SetItemState(i, 0, LVIS_SELECTED);
        } else
            m_InfoListView.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
    }

    return 0;
}