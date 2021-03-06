// SkinManager.cpp: implementation of the CSkinManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkinManager.h"

CSkinManager* CSkinManager::m_pInstance;
CIni CSkinManager::m_skinINI;
CString CSkinManager::m_pszCurrentSkin;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkinManager::CSkinManager()
{
}

CSkinManager::~CSkinManager()
{
}
//////////////////////////////////////////////////////////////////////////
//
//  Init : 初始化实例句柄，保证唯一性
//
//  参数 :
//      lpctSkinName - 为空时，载入默认皮肤（\\Skin\\Default\\theme.ini）
//                   - 不为空时，载入指定的皮肤
//
//  返回值 : 实例指针
//
//////////////////////////////////////////////////////////////////////////
CSkinManager* CSkinManager::Init(LPCTSTR lpctSkinName/* = NULL*/)
{
    if (m_pInstance == NULL) {
        CString pszSkinFilePath(_T("Default"));
        m_pInstance = new CSkinManager;

        if (lpctSkinName != NULL && _tcslen(lpctSkinName) != 0)
            pszSkinFilePath = lpctSkinName;

        m_pszCurrentSkin = pszSkinFilePath;
        pszSkinFilePath = g_pszModulePath + _T("Skin\\") + pszSkinFilePath + _T("\\theme.ini");
        m_skinINI.SetPathName(pszSkinFilePath);
    }

    return m_pInstance;
}
//////////////////////////////////////////////////////////////////////////
//
//  ChangeSkin : 更换皮肤主题
//
//  参数 :
//      lpctSkinName - 皮肤名称，即皮肤文件夹名称
//
//  返回值 :
//      布尔值，TRUE表示成功，FALSE表示失败
//
//////////////////////////////////////////////////////////////////////////
BOOL CSkinManager::ChangeSkin(LPCTSTR lpctSkinName)
{
    if (_tcslen(lpctSkinName) == 0) return FALSE;

    CString pszSkinFilePath = lpctSkinName;
    m_pszCurrentSkin = lpctSkinName;
    pszSkinFilePath = g_pszModulePath + _T("Skin\\") + pszSkinFilePath + _T("\\theme.ini");
    m_skinINI.SetPathName(pszSkinFilePath);
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//
//  GetCurrentSkinName : 获得当前皮肤名称
//
//  参数 : 无
//
//  返回值 : 当前皮肤名称（文件夹名）
//
//////////////////////////////////////////////////////////////////////////
LPCTSTR CSkinManager::GetCurrentSkinName()
{
    return m_pszCurrentSkin;
}
//////////////////////////////////////////////////////////////////////////
//
//  GetColor : 根据皮肤参数设置文件读取一个颜色值
//
//  参数 :
//      lpctSection - INI文件的节(Section)名
//      lpctKey - lpctSection下的某个键值(Key)名
//      clrDefault - 如果读取内容为空，则返回默认的颜色值
//
//  返回值 : 读取的颜色值
//
//////////////////////////////////////////////////////////////////////////
COLORREF CSkinManager::GetColor(LPCTSTR lpctSection, LPCTSTR lpctKey, COLORREF clrDefault)
{
    return m_skinINI.GetColor(lpctSection, lpctKey, clrDefault);
}
//////////////////////////////////////////////////////////////////////////
//
//  GetBool : 根据皮肤参数设置文件读取一个布尔值
//
//  参数 :
//      lpctSection - INI文件的节(Section)名
//      lpctKey - lpctSection下的某个键值(Key)名
//      bDefault - 如果读取内容为空，则返回默认的布尔值
//
//  返回值 : 读取的布尔值
//
//////////////////////////////////////////////////////////////////////////
BOOL CSkinManager::GetBool(LPCTSTR lpctSection, LPCTSTR lpctKey, BOOL bDefault/* =TRUE */)
{
    return m_skinINI.GetBool(lpctSection, lpctKey, bDefault);
}
//////////////////////////////////////////////////////////////////////////
//
//  GetInt : 根据皮肤参数设置文件读取一个整数值
//
//  参数 :
//      lpctSection - INI文件的节(Section)名
//      lpctKey - lpctSection下的某个键值(Key)名
//      iDefault - 如果读取内容为空，则返回默认的整数值
//
//  返回值 : 读取的整数值
//
//////////////////////////////////////////////////////////////////////////
int CSkinManager::GetInt(LPCTSTR lpctSection, LPCTSTR lpctKey, int iDefault)
{
    return m_skinINI.GetInt(lpctSection, lpctKey, iDefault);
}
//////////////////////////////////////////////////////////////////////////
//
//  GetPicturePath : 根据皮肤参数设置文件读取一个图片文件的完整路径
//
//  参数 :
//      lpctSection - INI文件的节(Section)名
//      lpctKey - lpctSection下的某个键值(Key)名
//      lptValue - 用于存放路径的字符串缓冲
//      iValueLen - lptValue的内存空间长度
//      lpctDefault - 如果读取内容为空，则返回该默认值
//
//  返回值 : 读取的字符串长度
//
//////////////////////////////////////////////////////////////////////////
DWORD CSkinManager::GetPicturePath(LPCTSTR lpctSection, LPCTSTR lpctKey, LPTSTR lptValue, int iValueLen, LPCTSTR lpctDefault/* =NULL */)
{
    TCHAR* tszTemp = new TCHAR[_MAX_PATH];
    memset(tszTemp, 0, _MAX_PATH);
    m_skinINI.GetString(lpctSection, lpctKey, tszTemp, _MAX_PATH, lpctDefault);
    CString pszValue = tszTemp;
    delete[] tszTemp;
    pszValue = g_pszModulePath + _T("Skin\\") + m_pszCurrentSkin + _T("\\") + pszValue;

    if (pszValue.GetLength() > iValueLen)
        return 0;

    _tcscpy(lptValue, pszValue);
    return (DWORD)_tcslen(lptValue);
}

