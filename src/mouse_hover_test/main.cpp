#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hStatic;
    static HWND hButton;
    static BOOL bEventRegistered;
    switch (msg) {
        case WM_CREATE:
            hStatic = CreateWindow(TEXT("STATIC"), TEXT(" �����ƥ��å� "), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
            hButton = CreateWindow(TEXT("BUTTON"), TEXT(" �ܥ��� "), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            LPCTSTR lpszText = bEventRegistered ? TEXT(" �ޥ����τӤ��Ƥ��ޤ� ") : TEXT(" �ޥ�����һ���r�gֹ�ޤ�ޤ��� ");
            TextOut(hdc, 10, 10, lpszText, lstrlen(lpszText));
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_SIZE:
            MoveWindow(hStatic, 100, 100, 200, 200, TRUE);
            MoveWindow(hButton, 400, 100, 200, 200, TRUE);
            break;
        case WM_MOUSEMOVE:
            if (!bEventRegistered) {
                bEventRegistered = TRUE;
                InvalidateRect(hWnd, 0, 1);
                TRACKMOUSEEVENT	trackmouseevent = { sizeof(trackmouseevent) };
                trackmouseevent.dwFlags = TME_HOVER | TME_LEAVE;
                trackmouseevent.hwndTrack = hWnd;
                trackmouseevent.dwHoverTime = HOVER_DEFAULT;
                TrackMouseEvent(&trackmouseevent);
            }
            break;
        case WM_MOUSEHOVER:
            bEventRegistered = FALSE;
            InvalidateRect(hWnd, 0, 1);
            break;
        case WM_MOUSELEAVE: // ���饤������I����˥ޥ����������Ȥ��� TRACKMOUSEEVENT ���������Ƥ��ޤ��褦��
            bEventRegistered = FALSE;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASS wndclass = {
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0,
        0,
        hInstance,
        0,
        LoadCursor(0, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        0,
        szClassName
    };
    RegisterClass(&wndclass);
    HWND hWnd = CreateWindow(
                    szClassName,
                    TEXT(" �ޥ�����һ���r�gֹ�᤿�Ȥ��˺Τ���I���� "),
                    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                    CW_USEDEFAULT,
                    0,
                    CW_USEDEFAULT,
                    0,
                    0,
                    0,
                    hInstance,
                    0
                );
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}