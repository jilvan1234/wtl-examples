// GifControlSample.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
    #error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CGifControlSampleApp:
// �йش����ʵ�֣������ GifControlSample.cpp
//

class CGifControlSampleApp : public CWinApp
{
public:
    CGifControlSampleApp();

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
};

extern CGifControlSampleApp theApp;