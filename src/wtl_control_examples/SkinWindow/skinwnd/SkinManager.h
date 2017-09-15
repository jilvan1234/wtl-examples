// SkinManager.h: interface for the CSkinManager class.
//
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//
// SkinManager.h
// ��Ȩ���У�G-Net
// ���ߣ�XuYu
// �������ڣ�2007-5-28
// �������ڣ�2007-5-28
// ������Ƥ�������������ڳ�ʼ��Ƥ�������ļ�·�������Ƥ�������Ȳ�����
// ���¼�¼��
//  2007-5-28 - �����˻����Ĺ���
//
///////////////////////////////////////////////////////////////////
#if !defined(AFX_SKINMANAGER_H__5239F6E5_2013_4928_9160_4B9598B35F9C__INCLUDED_)
#define AFX_SKINMANAGER_H__5239F6E5_2013_4928_9160_4B9598B35F9C__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000


class CSkinManager
{
public:
    CSkinManager();
    virtual ~CSkinManager();

    static CSkinManager* Init(LPCTSTR lpctSkinName = NULL);
    BOOL ChangeSkin(LPCTSTR lpctSkinName);
    COLORREF GetColor(LPCTSTR lpctSection, LPCTSTR lpctKey, COLORREF clrDefault);
    DWORD GetPicturePath(LPCTSTR lpctSection, LPCTSTR lpctKey, LPTSTR lptValue, int iValueLen, LPCTSTR lpctDefault = NULL);
    int GetInt(LPCTSTR lpctSection, LPCTSTR lpctKey, int iDefault);
    BOOL GetBool(LPCTSTR lpctSection, LPCTSTR lpctKey, BOOL bDefault = TRUE);
    LPCTSTR GetCurrentSkinName();

private:
    static CIni m_skinINI;              //Ƥ������INI�ļ�
    static CSkinManager* m_pInstance;   //Ψһʵ�����
    static CString m_pszCurrentSkin;    //��ǰƤ������

};

#endif // !defined(AFX_SKINMANAGER_H__5239F6E5_2013_4928_9160_4B9598B35F9C__INCLUDED_)
