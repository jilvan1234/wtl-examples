// PatientListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "XListControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPatientListCtrl

IMPLEMENT_DYNAMIC(CXListControl, CListCtrl)

CXListControl::CXListControl(
) {
}

CXListControl::~CXListControl(
) {
}


BEGIN_MESSAGE_MAP(CXListControl, CListCtrl)
END_MESSAGE_MAP()



// CXListControl message handlers

LRESULT CXListControl::WindowProc(
    UINT message, 
    WPARAM wParam, 
    LPARAM lParam
) {
    if(message==WM_VSCROLL||message==WM_HSCROLL)
    {
        WORD sbCode=LOWORD(wParam);
        if(sbCode==SB_THUMBTRACK
            ||sbCode==SB_THUMBPOSITION)
        {
            SCROLLINFO siv={0};
            siv.cbSize=sizeof(SCROLLINFO);
            siv.fMask=SIF_ALL;
            SCROLLINFO sih=siv;
            int nPos=HIWORD(wParam);
            CRect rcClient;
            GetClientRect(&rcClient);
            GetScrollInfo(SB_VERT,&siv);
            GetScrollInfo(SB_HORZ,&sih);
            SIZE sizeAll;
            if(sih.nPage==0) 
                sizeAll.cx=rcClient.right;
            else
                sizeAll.cx=rcClient.right*(sih.nMax+1)/sih.nPage ;
            if(siv.nPage==0)
                sizeAll.cy=rcClient.bottom;
            else
                sizeAll.cy=rcClient.bottom*(siv.nMax+1)/siv.nPage ;

            SIZE size={0,0};
            if(WM_VSCROLL==message)
            {
                //size.cx=sizeAll.cx*sih.nPos/(sih.nMax+1);
                size.cy=sizeAll.cy*(nPos-siv.nPos)/(siv.nMax+1);
            }else
            {
                size.cx=sizeAll.cx*(nPos-sih.nPos)/(sih.nMax+1);
                //size.cy=sizeAll.cy*siv.nPos/(siv.nMax+1);
            }
            Scroll(size);
            return 1;
        }
    }
    return CListCtrl::WindowProc(message, wParam, lParam);

}

void CXListControl::PreSubclassWindow() 
{
	//use our custom CHeaderCtrl
	m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);//�滻ԭ�ȵı�ͷ
	CListCtrl::PreSubclassWindow();
}