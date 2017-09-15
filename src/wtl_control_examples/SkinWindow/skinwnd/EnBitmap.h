// EnBitmap.h: interface for the CEnBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENBITMAP_H__76F269F6_895A_48EC_BA09_7D3BEF0438E7__INCLUDED_)
#define AFX_ENBITMAP_H__76F269F6_895A_48EC_BA09_7D3BEF0438E7__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

class CEnBitmap : public CBitmap
{
public:
    CEnBitmap();
    virtual ~CEnBitmap();

    BOOL LoadImage(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst = NULL, COLORREF crBack = 0);
    BOOL LoadImage(LPCTSTR lpctImagePath, COLORREF crBack = 0);
    BOOL LoadImage2(UINT uIDRes, LPCTSTR lpctResType, HMODULE hInst = NULL, COLORREF crBack = 0);
    int GetWidth()
    {
        BITMAP bm;
        memset(&bm, 0, sizeof(bm));
        GetBitmap(&bm);
        return bm.bmWidth;
    }
    int GetHeight()
    {
        BITMAP bm;
        memset(&bm, 0, sizeof(bm));
        GetBitmap(&bm);
        return bm.bmHeight;
    }
    CRect GetRect()
    {
        return CRect(0, 0, GetWidth(), GetHeight());
    }
    BOOL ExtendDraw(CDC *pDC, CRect rc, int nX, int nY);
    BOOL ExtendDrawImage(CEnBitmap &bmp, CRect rc, int nX, int nY);
    BOOL StretchDraw(CDC *pDC, LPRECT r, LPRECT sr);
    BOOL Draw(CDC *pDC, LPRECT r);

private:
    static int GetImageType(LPCTSTR lpctImagePath);
    static HBITMAP ExtractBitmap(IPicture* pPicture, COLORREF crBack = 0);
    static HBITMAP LoadImageFile(LPCTSTR lpctImagePath, COLORREF crBack = 0);
    static HBITMAP LoadImageResource(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst = NULL, COLORREF crBack = 0);

    static BOOL GetResource(LPCTSTR lpctName, LPCTSTR lpctType, HMODULE hInst, void* pRes, int& nBufSize);
    static IPicture* LoadFromBuffer(BYTE* pBuff, int nSize);
    BOOL AttachEx(IPicture* pPicture, COLORREF crBack);
};

#endif // !defined(AFX_ENBITMAP_H__76F269F6_895A_48EC_BA09_7D3BEF0438E7__INCLUDED_)
