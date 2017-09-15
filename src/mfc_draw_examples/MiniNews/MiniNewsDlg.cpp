
// MiniNewsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MiniNews.h"
#include "MiniNewsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	
END_MESSAGE_MAP()


// CMiniNewsDlg �Ի���




CMiniNewsDlg::CMiniNewsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiniNewsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiniNewsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_webBrowser);
}

BEGIN_MESSAGE_MAP(CMiniNewsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCPAINT()
	ON_WM_ACTIVATE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_SYSCOMMAND()
	ON_WM_NCLBUTTONUP()
	//ON_WM_NCMOUSELEAVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_CTLCOLOR()


	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMiniNewsDlg ��Ϣ�������
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMiniNewsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect   rect;
	GetClientRect(&rect);
	//���öԻ��򱳾���ɫ
	dc.FillSolidRect(rect,m_bkColor);   //����Ϊ���ڱ���
	DrawDialog();
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}
BOOL CMiniNewsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != NULL)
	//{
	//	BOOL bNameValid;
	//	CString strAboutMenu;
	//	bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	ASSERT(bNameValid);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	//// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	////  ִ�д˲���
	//SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	//SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_bkColor = RGB(20,120,200);
	m_bMouseOnMinBtn = FALSE;
	m_bMouseOnCloseBtn =FALSE;
	m_strTitle = "������Ѷ��";
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	this->SetWindowText("������Ѷ��");
	this->m_webBrowser.Navigate("http://mini.37zone.com/miniportal/client/default/1.2.0.0/",NULL,NULL,NULL,NULL);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMiniNewsDlg::DrawDialog()
{
	m_nFrameCY = GetSystemMetrics(SM_CYFIXEDFRAME);//��ȡ�Ի���߿�ĸ߶�
	m_nFrameCX = GetSystemMetrics(SM_CXDLGFRAME);//��ȡ�Ի��߿�Ŀ��
	if(GetStyle()&WS_BORDER)//��ȡ�Ի����Ƿ��б߿�
	{
		m_nBorderCY = GetSystemMetrics(SM_CYBORDER) + m_nFrameCY;
		m_nBorderCX = GetSystemMetrics(SM_CXBORDER) +m_nFrameCX; 
	}
	else
	{		
		m_nBorderCY = m_nFrameCY;
		m_nBorderCX = m_nFrameCX;
	}
	m_nTitleBarCY = GetSystemMetrics(SM_CYCAPTION) + m_nBorderCY;//����������߶�
	m_nTitleBarCX =m_nBorderCX;

	CRect winRect,factRect;
	GetWindowRect(&winRect);
	factRect.CopyRect(CRect(0,0,winRect.Width(),winRect.Height()));
	CWindowDC windowsDC(this);//��ȡ�����豸������
	//��ȡ����MFC���ڵĸ߶ȺͿ��
	m_nWinWidth = winRect.Width();
	m_nWinHeight = winRect.Height();
	//���ƶԻ��������λͼ	
	DisplayBmp(0,0,m_nWinWidth,m_nTitleBarCY,IDB_LEFTTITLE);
	//���ƶԻ����������˵�logoͼ��
	DisplayBmp(3,0,26,m_nTitleBarCY,IDB_APPICON);

	//���ƶԻ�����߿�λͼ
	DisplayBmp(0,m_nTitleBarCY,m_nBorderCX,m_nWinHeight-m_nBorderCY,IDB_LEFTBAR);
	//���ƶԻ���ױ߿�λͼ
	DisplayBmp(m_nBorderCX,m_nWinHeight-m_nBorderCX,m_nWinWidth-2*m_nBorderCX,m_nBorderCX,IDB_BOTTOMBAR);
	//���ƶԻ�����߿�λͼ
	DisplayBmp(m_nWinWidth-m_nBorderCX,m_nTitleBarCY,m_nBorderCX,m_nWinHeight-m_nBorderCY,IDB_RIGHTBAR);
	//���Ի��������С����ť
	DisplayBmp(m_nWinWidth-26*2-5,0,26,26,IDB_MINBTN1);
	//���Ի�����ƹرհ�ť
	DisplayBmp(m_nWinWidth-26-5,0,26,26,IDB_CLOSEBTN1);

	DrawTitleText();
	ReleaseDC(&windowsDC);
}
//=====================��ָ��λ����ʾbmpλͼͼ��
void CMiniNewsDlg::DisplayBmp(int x,int y,int w,int h,int nID)
{
	CRect winRC;
	CDC* pDC=GetWindowDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	BITMAPINFO bmpInfo;
	CBitmap bmp;	
	GetWindowRect(&winRC);
	bmp.LoadBitmap(nID);
	bmp.GetObject(sizeof(BITMAPINFO),&bmpInfo);
	int nBmpCX = bmpInfo.bmiHeader.biWidth;
	int nBmpCY = bmpInfo.bmiHeader.biHeight;
	memDC.SelectObject(bmp);
	pDC->StretchBlt(x,y,w,h,
		&memDC,0,0,nBmpCX,nBmpCY,SRCCOPY);//�ڴ����л���λͼ
	bmp.DeleteObject();
	ReleaseDC(pDC);
}
void CMiniNewsDlg::SetTitleText(CString strTitle)
{
	this->m_strTitle = strTitle + "-С�̵�������";
}
//���Ʊ����������ı�����
void CMiniNewsDlg::DrawTitleText()
{

	CDC* pDC= GetWindowDC();
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->SetTextAlign(TA_CENTER);
	CRect rect;
	GetClientRect(&rect);
	CSize szText = pDC->GetTextExtent(m_strTitle);
	CFont* font,*fOldFont;
	font = new CFont;
	font->CreateFont(12,0,0,0,FW_BOLD,FALSE,FALSE,0,ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,_T("����"));
	fOldFont = pDC->SelectObject(font);
	CRect winRect;
	GetWindowRect(&winRect);
	pDC->TextOut(winRect.Width()/2,6.5,m_strTitle);
	pDC->SelectObject(fOldFont);
	ReleaseDC(pDC);
	delete font;
	delete fOldFont;
	
}

void CMiniNewsDlg::OnNcPaint()
{
	// TODO: �ڴ˴������Ϣ����������
	DrawDialog();
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnNcPaint()
}

void CMiniNewsDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	OnNcPaint();
	// TODO: �ڴ˴������Ϣ����������
}

void CMiniNewsDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect minRC,moreRC,closeRC,winRC,cleanRC,toolkitRC,shoppingRC;
	GetWindowRect(&winRC);
	closeRC.CopyRect(CRect(m_nWinWidth-26,0,m_nWinWidth-26+26,26));		
	minRC.CopyRect(CRect(m_nWinWidth-26*2,0,m_nWinWidth-26*2+26,26));

	point.Offset(-winRC.left,-winRC.top);//����pointΪ��Ļ����,���ｫ��ת��Ϊ��������
	if(closeRC.PtInRect(point))//����ڹرհ�ť��ʱ�����İ�ť��ʾ��λͼ
	{
		m_bMouseOnCloseBtn = TRUE;
		m_bMouseOnMinBtn =FALSE;
		m_bMouseOnMoreBtn = FALSE;
		DisplayBmp(m_nWinWidth-26-5,0,26,26,IDB_CLOSEBTN2);
		DisplayBmp(m_nWinWidth-26*2-5,0,26,26,IDB_MINBTN1);
	}
	else if(minRC.PtInRect(point))//�������С����ť��ʱ�����İ�ť��ʾ��λͼ
	{
		m_bMouseOnMinBtn =TRUE;	
		m_bMouseOnCloseBtn = FALSE;
		DisplayBmp(m_nWinWidth-26-5,0,26,26,IDB_CLOSEBTN1);
		DisplayBmp(m_nWinWidth-26*2-5,0,26,26,IDB_MINBTN2);		
	}
	CDialog::OnNcMouseMove(nHitTest, point);
}
void CMiniNewsDlg::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_bMouseOnCloseBtn)
	{
		this->SendMessage(WM_CLOSE);
	}
	if(m_bMouseOnMinBtn)
	{
		this->ShowWindow(SW_MINIMIZE);
		m_bMouseOnMinBtn = FALSE;
	}
	CDialog::OnNcLButtonUp(nHitTest, point);
}

void CMiniNewsDlg::OnNcMouseLeave()
{
	// �ù���Ҫ��ʹ�� Windows 2000 ����߰汾��
	// ���� _WIN32_WINNT �� WINVER ���� >= 0x0500��
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DisplayBmp(m_nWinWidth-26*2-5,0,26,26,IDB_MINBTN1);
	DisplayBmp(m_nWinWidth-26-5,0,26,26,IDB_CLOSEBTN1);
	m_bMouseOnMinBtn =FALSE;	
	m_bMouseOnCloseBtn = FALSE;
//	CDialog::OnNcMouseLeave();
}

void CMiniNewsDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!(m_bMouseOnMinBtn||m_bMouseOnCloseBtn))
		CDialog::OnNcLButtonDown(nHitTest, point);
}

HBRUSH CMiniNewsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	HBRUSH brush=CreateSolidBrush(m_bkColor);
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT); 
		//���ñ���Ϊ͸��
		pDC->SetTextColor(RGB(255,255,255)); //����������ɫ
		pDC->SetBkColor(m_bkColor);
		//pWnd->SetFont(cFont); //��������
		hbr = (HBRUSH)brush;
		//������ˢ
		break;
	case CTLCOLOR_BTN:
		pDC->SetBkMode(TRANSPARENT); 
		//���ñ���Ϊ͸��
		pDC->SetTextColor(RGB(255,255,255)); //����������ɫ
		pDC->SetBkColor(m_bkColor);
		//pWnd->SetFont(cFont); //��������
		hbr = (HBRUSH)brush;
		//������ˢ
		break;

	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
void CMiniNewsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}



//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMiniNewsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

