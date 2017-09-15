#pragma once


// CIconBtn

class CIconBtn : public CButton
{
    DECLARE_DYNAMIC(CIconBtn)

public:
    CIconBtn();
    virtual ~CIconBtn();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void PreSubclassWindow();
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

    enum {
        IBTN_ICON_LEFT_TEXT_RIGHT = 0,  // Left:Icon   Right:Text
        IBTN_ICON_RIGHT_TEXT_LEFT,      // Right:Icon  Left:Text
        IBTN_ICON_TOP_TEXT_BOTTOM,      // Top:Icon    Bottom:Text
        IBTN_ICON_BOTTOM_TEXT_TOP       // Bottom:Icon Top:Text
    };

protected:
    void DrawBackground();
    void DrawBorder();
    void Draw3DBorderForRect(CDC *pDC, CRect *pRect);
    void DrawFocusRect();
    void DrawIcon();
    void DrawText();
    void PrepareDrawIconAndText();

public:
    BOOL SetIcon(int nIcon, int nCxDesired = 0, int nCyDesired = 0);
    BOOL SetAlign(BYTE byAlign);

private:
    BYTE  m_byAlign;            // text��icon�Ķ��뷽ʽ
    CDC*  m_pDC;                // dc handle pointer
    CRect m_rcBtn;              // ��ťrect
    bool  m_bIsFocused;         // ��ť�Ƿ��ý���
    bool  m_bIsPressed;         // ��ť�Ƿ񱻰���
    bool  m_bIsDisabled;        // ��ť�Ƿ�Disable

    HICON m_hIcon;              // Icon handle
    int   m_nIconWidth;         // Icon width
    int   m_nIconHeight;        // Icon height

    int   m_nFocusRectOffset;   // ������ε�ƫ����
    int   m_nCxIconStart;       // Draw icon����ʼ��x����
    int   m_nCyIconStart;       // Draw icon����ʼ��y����
    RECT  m_rcText;             // Draw text�ľ�������

protected:
    DECLARE_MESSAGE_MAP()
};


