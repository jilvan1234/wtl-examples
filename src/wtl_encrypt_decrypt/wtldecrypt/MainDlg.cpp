/**************************************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.
   Author: Leon Finker  7/2003
**************************************************************************/

// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"

#define RETURN_ON_ERROR(hr) if(hr != S_OK) { check_last_error();  return 0;}
const int MAX_PASS_LEN = 256;

void check_last_error()
{
	LPVOID lpMsgBuf;
	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf, 0, NULL ))
	{
		return;
	}
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION );
	LocalFree( lpMsgBuf );
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	SendDlgItemMessage(IDC_EDIT_PASSWORD, EM_LIMITTEXT, MAX_PASS_LEN, 0);

	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CWaitCursor waitcur;
	CString sFileName;
	if(GetModuleFileName(NULL, sFileName.GetBufferSetLength(MAX_PATH+1), MAX_PATH) == 0)
		return 0;
	int nPos = sFileName.ReverseFind(_T('\\'));
	if(nPos == -1)
		return 0;
	sFileName.Delete(nPos, sFileName.GetLength() - nPos);
	sFileName += _T("\\");

	HWND hWndPwd = ::GetDlgItem(m_hWnd, IDC_EDIT_PASSWORD);
	if(!hWndPwd)
		return 0;

	HRSRC resAlgID = FindResource(NULL, _T("1"), RT_RCDATA);
	HRSRC resFileName = FindResource(NULL, _T("2"), RT_RCDATA);
	HRSRC resData = FindResource(NULL, _T("3"), RT_RCDATA);
    HRSRC resIV = FindResource(NULL, _T("4"), RT_RCDATA);
	if(!resAlgID || !resFileName || !resData || !resIV)
		return 0;
	
	HGLOBAL hGlobalAlgID = LoadResource(NULL, resAlgID);
	HGLOBAL hGlobalFileName = LoadResource(NULL, resFileName);
	HGLOBAL hGlobalData = LoadResource(NULL, resData);
    HGLOBAL hGlobalIV = LoadResource(NULL, resIV);
	if(!hGlobalAlgID || !hGlobalFileName || !hGlobalData || !hGlobalIV)
		return 0;
	
	int resAlgIDLen = SizeofResource(NULL, resAlgID);
	int resFileNameLen = SizeofResource(NULL, resFileName);
	DWORD resDataLen = SizeofResource(NULL, resData);
    DWORD resIVLen = SizeofResource(NULL, resIV);

	TCHAR* pALG = (TCHAR*)LockResource(hGlobalAlgID);
	TCHAR* pFN  = (TCHAR*)LockResource(hGlobalFileName);
	BYTE* pData = (BYTE*)LockResource(hGlobalData);
    BYTE* pIV = (BYTE*)LockResource(hGlobalIV);

	if(!pFN || resFileNameLen <= 0 || !pData || resDataLen <= 0 || !pIV || resIVLen <= 0)
		return 0;

	CString sAlgID(pALG, resAlgIDLen/sizeof(TCHAR));
	DWORD dwProviderType = PROV_RSA_FULL;
	LPCTSTR szProvider   = MS_ENHANCED_PROV;
	ALG_ID	algid = CALG_3DES;
	if(sAlgID == _T("1"))
	{
		dwProviderType = PROV_RSA_FULL;
		szProvider     = MS_ENHANCED_PROV;
		algid = CALG_3DES;
	}
	else if(sAlgID == _T("2"))
	{
		dwProviderType = PROV_RSA_AES;
		szProvider     = MS_ENH_RSA_AES_PROV;
		algid          = CALG_AES_256;
	}
	else
		return 0;

	CCryptProv prov;
	HRESULT hr = prov.Initialize(dwProviderType, NULL, szProvider);
	if(hr == NTE_BAD_KEYSET)
		hr = prov.Initialize(dwProviderType, NULL, szProvider, CRYPT_NEWKEYSET);
	RETURN_ON_ERROR(hr);
	
	CCryptSHAHash sha;    
	hr = sha.Initialize(prov);
	RETURN_ON_ERROR(hr);

	TCHAR pass[MAX_PASS_LEN]={0};
	int passLen = ::GetWindowText(hWndPwd, pass, MAX_PASS_LEN);
	::SetWindowText(hWndPwd, _T("********************"));
	::SetWindowText(hWndPwd, _T(""));
    
	hr = sha.AddData((BYTE*)pass, passLen*sizeof(TCHAR));
    RETURN_ON_ERROR(hr);
	SecureZeroMemory(pass, MAX_PASS_LEN*sizeof(TCHAR));
	BYTE noise[] ={ 0xca, 0xed, 0x87, 0xfb, 0x92, 0x61, 0xa3, 0x09, 0x41, 0x0d, 0xaf, 0xbc, 0x9d, 
		            0x02, 0x42, 0x79, 0x7a, 0x50, 0x56, 0xd5, 0xa1, 0x56, 0x60, 0x1f, 0xaf, 0x42, 
					0x7f, 0x26, 0xc7, 0x81, 0xa3, 0x97, 0xe6, 0xae, 0xba, 0xba, 0x57, 0xfd, 0x0a, 
					0x35, 0xed, 0xca, 0x37, 0x4d, 0xdf, 0x78, 0xb9, 0xd6, 0xd0, 0x29, 0x55, 0xe7, 
					0xa7, 0x13, 0xec, 0xbf, 0x97, 0x81, 0x1a, 0x86};
	hr = sha.AddData(noise, sizeof(noise));
    RETURN_ON_ERROR(hr);

	BYTE tmpHash[20];
	DWORD dw=sizeof(tmpHash);
	for(int i=0; i<101; ++i)
	{
		hr = sha.GetValue(tmpHash, &dw);
		RETURN_ON_ERROR(hr);
		sha.Destroy();
		hr = sha.Initialize(prov);
		RETURN_ON_ERROR(hr);
		hr = sha.AddData(tmpHash, dw);
		RETURN_ON_ERROR(hr);
	}
	SecureZeroMemory(tmpHash, dw);

	CCryptDerivedKey derKey;
	hr = derKey.Initialize(prov,sha,algid);
	RETURN_ON_ERROR(hr);

    hr = derKey.SetIV(pIV);
    RETURN_ON_ERROR(hr);
    hr = derKey.SetMode(CRYPT_MODE_CBC);
    RETURN_ON_ERROR(hr);
    hr = derKey.SetPadding(PKCS5_PADDING);
    RETURN_ON_ERROR(hr);

	CHeapPtr<BYTE, CComAllocator> ptrData;
	if(!ptrData.Allocate(resDataLen))
		return 0;
	memcpy(ptrData.m_pData, pData, resDataLen);
	hr = derKey.Decrypt(TRUE, ptrData.m_pData, &resDataLen);
	RETURN_ON_ERROR(hr);
	sha.Destroy();

	BOOL bRet = FALSE;
	sFileName.Append(pFN, resFileNameLen/sizeof(TCHAR));
	CAtlFile file;
	if(file.Create(sFileName,GENERIC_WRITE, 0, CREATE_NEW) == S_OK)
	{
		DWORD dwWritten=0;
		if(file.Write(ptrData.m_pData, resDataLen, &dwWritten) == S_OK && dwWritten == resDataLen)
			bRet = TRUE;
		file.Close();
	}

	if(bRet)
		MessageBox(_T("Operation Successful"), _T("Status"), MB_OK|MB_ICONINFORMATION);
	EndDialog(wID);
	return 0;
}
