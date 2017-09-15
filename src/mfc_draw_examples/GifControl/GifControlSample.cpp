// GifControlSample.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "GifControlSample.h"
#include "GifControlSampleDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif


// CGifControlSampleApp

BEGIN_MESSAGE_MAP(CGifControlSampleApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGifControlSampleApp ����

CGifControlSampleApp::CGifControlSampleApp()
{
    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGifControlSampleApp ����

CGifControlSampleApp theApp;


// CGifControlSampleApp ��ʼ��

BOOL CGifControlSampleApp::InitInstance()
{
    //Init GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Status state = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
    // ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
    //����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
    // �����ؼ��ࡣ
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);
    CWinApp::InitInstance();
    AfxEnableControlContainer();
    // ��׼��ʼ��
    // ���δʹ����Щ���ܲ�ϣ����С
    // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
    // ����Ҫ���ض���ʼ������
    // �������ڴ洢���õ�ע�����
    // TODO: Ӧ�ʵ��޸ĸ��ַ�����
    // �����޸�Ϊ��˾����֯��
    SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
    CGifControlSampleDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();

    if (nResponse == IDOK) {
        // TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
        //  �Ի���Ĵ���
    } else if (nResponse == IDCANCEL) {
        // TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
        //  �Ի���Ĵ���
    }

    //
    //GdiplusShutdown(gdiplusToken);
    // ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
    //  ����������Ӧ�ó������Ϣ�á�
    return FALSE;
}
