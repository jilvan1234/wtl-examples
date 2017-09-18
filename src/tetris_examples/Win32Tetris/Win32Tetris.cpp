// @author  Kaz Ozzy (id: fa11enprince)
// @update  2014/07/11
// @version 1.2
// ver 1.0  ����
// ver 1.1  ���`�४�`�Щ`�r�I��׷��
// ver 1.2  ���饹�OӋҊֱ��
//          public��isMovable()��Field���饹����Piece���饹���ƄӤ���private��
//          Tetris���饹��getNextPiece(), getField()����Ҫ�ʤΤ�����
//
// �ο�
// Tetris :
//    [�ƥȥꥹ�����귽]
//       http://www13.plala.or.jp/kymats/study/game_other/SPACE_TETRIS/st1.html
//
// GDI :
//    [BMP�ե������ʹ�ä����g�ʥѥ�ѥ饢�˥�`�����]
//       http://www13.plala.or.jp/kymats/study/MULTIMEDIA/bmp_animation.html
//    [�ե���ȤηN�]
//       http://wisdom.sakura.ne.jp/system/winapi/win32/win131.html
//    [������MSDN]
//       http://msdn.microsoft.com/ja-jp/library/cc428720.aspx
//       http://msdn.microsoft.com/ja-jp/library/cc410411.aspx
//
// Multi Thread:
//    [MSDN]
//       http://msdn.microsoft.com/ja-jp/library/kdzttdcb.aspx
//    [Win32�Υޥ������åɤ��[�֤���΂����h]
//       http://www.geocities.jp/debu0510/personal/kusosure.html#m2
//
// Windows API:
//    ��è�Ǥ�狼��Windows�ץ���ߥ� ��4�桻
//
// WM_APP
//       http://chokuto.ifdef.jp/advanced/message/WM_APP.html
//       
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Resource.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <tchar.h>
#include <ctime>
#include <process.h>
#pragma comment(lib, "winmm.lib")

#define MAX_LOADSTRING 100
// ���ץꥱ�`�������ФΥ�å��`�����x
#define WM_MUTEX       WM_APP
#define WM_RESTART     (WM_APP + 1)
#define MUTEX_NAME     _T("TetrisMutex")

namespace {
    const int PieceWidth = 4;
    const int PieceHeight = 4;
    const int FieldWidth = 14;
    const int FieldHeight = 24;
    const int SizeCellPixelX = 20;
    const int SizeCellPixelY = 20;
    const int SizeBmPixelX = 420;
    const int SizeBmPixelY = 520;
    HINSTANCE hInst;                            // �F�ڤΥ��󥿩`�ե�����
    TCHAR     szTitle[MAX_LOADSTRING];          // �����ȥ� �Щ`�Υƥ�����
    TCHAR     szWindowClass[MAX_LOADSTRING];    // �ᥤ�� ������ɥ� ���饹��
}

class Field
{
public:
    Field()
    {
        clear();
    }

    void clear()
    {
        for (int y = 0; y < FieldHeight; y++)
        {
            for (int x = 0; x < FieldWidth; x++)
            {
                block_[x][y] = 0;
                color_[x][y] = 0;
            }
        }
    }

    void fixPiece(POINT *pos, int block[][PieceHeight], int color[][PieceHeight])
    {
        for (int y = 0; y < PieceHeight; y++)
        {
            for (int x = 0; x < PieceWidth; x++)
            {
                // �Ϥ��w�ӳ��Ƥʤ���й̶�
                if (block[x][y] && pos->y + y >= 0)
                {
                    block_[pos->x + x][pos->y + y] = block[x][y];
                    color_[pos->x + x][pos->y + y] = color[x][y];
                }
            }
        }
    }

    int deleteLine()
    {
        int delCount = 0;
        for (int y = FieldHeight - 1; y >= 0; y--)
        {
            int xNumPerLine = 0;
            for (int x = 0; x < FieldWidth; x++)
            {
                if (block_[x][y])
                {
                    xNumPerLine++;
                }
            }
            if (xNumPerLine != FieldWidth)
            {
                continue;
            }
            delCount++;
            for (int x = 0; x < FieldWidth; x++)
            {
                block_[x][y] = 0;
            }
        }
        return delCount;
    }

    void shiftLine(int delCount)
    {
        // �¤����Ϥ��ߖ�
        for (int y = FieldHeight - 1; y >= 0 && delCount > 0;)
        {
            int xNumPerLine = 0;
            for (int x = 0; x < FieldWidth; x++)
            {
                if (block_[x][y])
                {
                    xNumPerLine++;
                }
            }
            // �Ф�����äƤʤ� ����äƤ���Ф��Εr���0�ˤʤ�
            if (xNumPerLine != 0)
            {
                y--;       // ����äƤʤ��Ȥ������Ϥ�
                continue;
            }
            // һ��ԑ���
            delCount--;
            for (int ty = y; ty >= 0; ty--)
            {
                for (int tx = 0; tx < FieldWidth; tx++)
                {
                    if (ty - 1 >= 0)
                    {
                        block_[tx][ty] = block_[tx][ty - 1];
                        color_[tx][ty] = color_[tx][ty - 1];
                    }
                    // 0��Ŀ����ϤΈ���
                    else
                    {
                        block_[tx][0] = 0;
                        color_[tx][0] = 0;
                    }
                }
            }
        }
    }

    bool isStacked()
    {
        if (block_[6][0] || block_[7][0])
        {
            return true;
        }
        return false;
    }

    int getBlockValue(int x, int y) const
    {
        return block_[x][y];
    }

    int getColorValue(int x, int y) const
    {
        return color_[x][y];
    }

private:
    int  block_[FieldWidth][FieldHeight];
    int  color_[FieldWidth][FieldHeight];
};

class Piece
{
public:
    enum Dir
    {
        Left,
        Right,
        Down,
    };

    Piece(Field *field)
    {
        clear();
        initPos();
        field_ = field;
    }

    virtual ~Piece() { }

    void clear()
    {
        for (int y = 0; y < PieceHeight; y++)
        {
            for (int x = 0; x < PieceWidth; x++)
            {
                block_[x][y] = 0;
                color_[x][y] = 0;
            }
        }
    }

public:
    bool move(Dir dir)
    {
        switch (dir)
        {
            case Left:
            {
                POINT pos = { pos_.x - 1, pos_.y };
                if (isMovable(&pos, block_))
                {
                    pos_ = pos;
                }
                else
                {
                    return false;
                }
                break;
            }
        case Right:
            {
                POINT pos = { pos_.x + 1, pos_.y };
                if (isMovable(&pos, block_))
                {
                    pos_ = pos;
                }
                else
                {
                    return false;
                }
                break;
            }
        case Down:
            {
                POINT pos = { pos_.x, pos_.y + 1 };
                if (isMovable(&pos, block_))
                {
                pos_ = pos;
                }
                else
                {
                    field_->fixPiece(&pos_, block_, color_);
                    return false;
                }
                break;
            }
        }
        return true;
    }

    bool turn()
    {
        int tBlock[PieceWidth][PieceHeight];
        int tColor[PieceWidth][PieceHeight];

        // �һ�ܞ���������ˤ�����
        for (int y = 0; y < PieceHeight; y++)
        {
            for (int x = 0; x < PieceWidth; x++)
            {
                tBlock[PieceHeight - y - 1][x] = block_[x][y];
                tColor[PieceHeight - y - 1][x] = color_[x][y];
            }
        }
        if (isMovable(&pos_, tBlock))
        {
            for (int y = 0; y < PieceHeight; y++)
            {
                for (int x = 0; x < PieceWidth; x++)
                {
                    block_[x][y] = tBlock[x][y];
                    color_[x][y] = tColor[x][y];
                }
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    int getBlockValue(int x, int y) const
    {
        return block_[x][y];
    }

    int getColorValue(int x, int y) const
    {
        return color_[x][y];
    }

    int getPosX() const
    {
        return pos_.x;
    }

    int getPosY() const
    {
        return pos_.y;
    }

private:
    POINT pos_;        // ���Ϥ�����
    Field *field_;

    void initPos()
    {
        pos_.x = 5;
        pos_.y = -3;
    }

    bool isMovable(POINT *pos, int block[][PieceHeight])
    {
        for (int y = 0; y < PieceHeight; y++)
        {
            for (int x = 0; x < PieceWidth; x++)
            {
                // 4��4�ڤǥ֥�å��Τ���ޥ��Τ��{�٤�
                if (block[x][y])
                {
                    int ofsX = pos->x + x;
                    int ofsY = pos->y + y;
                    bool validPos = (0 <= ofsX && ofsX <= FieldWidth - 1)
                        && (0 <= ofsY && ofsY <= FieldHeight - 1);
                    if (ofsX < 0 || ofsX >= FieldWidth
                        || ofsY >= FieldHeight
                        || (validPos && field_->getBlockValue(ofsX, ofsY)))
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

protected:
    int   block_[PieceWidth][PieceHeight];
    int   color_[PieceWidth][PieceHeight];
};

// ��������
// ��������
// ��������
// ��������
class Piece0 : public Piece
{
public:
    Piece0(Field *field)
        : Piece(field)
    {
        block_[1][1] = block_[2][1]
            = block_[1][2] = block_[2][2] = 1;
        color_[1][1] = color_[2][1]
            = color_[1][2] = color_[2][2] = RGB(0x00, 0x8f, 0xff);
    }
};

// ��������
// ��������
// ��������
// ��������
class Piece1 : public Piece
{
public:
    Piece1(Field *field)
        : Piece(field)
    {
        block_[1][0] = block_[1][1]
            = block_[1][2] = block_[1][3] = 1;
        color_[1][0] = color_[1][1]
            = color_[1][2] = color_[1][3] = RGB(0x4f, 0xaf, 0xdf);
    }
};

// ��������
// ��������
// ��������
// ��������
class Piece2 : public Piece
{
public:
    Piece2(Field *field)
        : Piece(field)
    {
        block_[1][1] = block_[1][2]
            = block_[2][2] = block_[1][3] = 1;
        color_[1][1] = color_[1][2]
            = color_[2][2] = color_[1][3] = RGB(0x00, 0xff, 0x8f);
    }
};

// ��������
// ��������
// ��������
// ��������
class Piece3 : public Piece
{
public:
    Piece3(Field *field)
        : Piece(field)
    {
        block_[1][1] = block_[2][1]
            = block_[1][2] = block_[1][3] = 1;
        color_[1][1] = color_[2][1]
            = color_[1][2] = color_[1][3] = RGB(0x00, 0xdf, 0xaf);
    }
};

// ��������
// ��������
// ��������
// ��������
class Piece4 : public Piece
{
public:
    Piece4(Field *field)
        : Piece(field)
    {
        block_[1][1] = block_[2][1]
            = block_[2][2] = block_[2][3] = 1;
        color_[1][1] = color_[2][1]
            = color_[2][2] = color_[2][3] = RGB(0xff, 0x00, 0x8f);
    }
};

// ��������
// ��������
// ��������
// ��������
class Piece5 : public Piece
{
public:
    Piece5(Field *field)
        : Piece(field)
    {
        block_[2][1] = block_[1][2]
            = block_[2][2] = block_[1][3] = 1;
        color_[2][1] = color_[1][2]
            = color_[2][2] = color_[1][3] = RGB(0xdf, 0x00, 0xaf);
    }
};

// ��������
// ��������
// ��������
// ��������
class Piece6 : public Piece
{
public:
    Piece6(Field *field)
        : Piece(field)
    {
        block_[1][1] = block_[1][2]
            = block_[2][2] = block_[2][3] = 1;
        color_[1][1] = color_[1][2]
            = color_[2][2] = color_[2][3] = RGB(0xff, 0x4f, 0x4f);
    }
};

class Tetris
{
public:
    explicit Tetris(HWND hWnd)
        : field_(nullptr), curPiece_(nullptr), nextPiece_(nullptr), hWnd_(hWnd),
        playTime_(0), score_(0), isGameover_(false)
    {
        field_ = new Field();
        nextPiece_ = createPiece();
        curPiece_ = createPiece();
    }

    ~Tetris()
    {
        releasePiece(nextPiece_);
        releasePiece(curPiece_);
        delete field_;
    }

    void  releasePiece(Piece *piece)
    {
        if (piece != nullptr)
        {
            delete piece;
            piece = nullptr;
        }
    }

    Piece *createPiece()
    {
        switch (rand() % 7)
        {
        case 0:
            return new Piece0(field_);
        case 1:
            return new Piece1(field_);
        case 2:
            return new Piece2(field_);
        case 3:
            return new Piece3(field_);
        case 4:
            return new Piece4(field_);
        case 5:
            return new Piece5(field_);
        case 6:
            return new Piece6(field_);
        }
        return nullptr;
    }

    Piece *getPiece()
    {
        return curPiece_;
    }

    static UINT WINAPI threadProc(void* parameter)
    {
        return (reinterpret_cast<Tetris*>(parameter))->executeThread();
    }

    void paint(HDC hdc)
    {
        SelectObject(hdc, GetStockObject(NULL_PEN));
        // ���`��ե��`��ɤΥ֥�å�
        for (int y = 0; y < FieldHeight; y++)
        {
            for (int x = 0; x < FieldWidth; x++)
            {
                int pixelX = (x + 1) * SizeCellPixelX;
                int pixelY = (y + 1) * SizeCellPixelY;
                if (field_->getBlockValue(x, y))
                {
                    HBRUSH hBrush = CreateSolidBrush(field_->getColorValue(x, y));
                    HBRUSH hOldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
                    Rectangle(hdc,
                        pixelX,
                        pixelY,
                        pixelX + SizeCellPixelX,
                        pixelY + SizeCellPixelY);
                    SelectObject(hdc, hOldBrush);
                    DeleteObject(hBrush);
                }
            }
        }

        // �F���Ƅ��ФΥ֥�å�
        for (int y = 0; y < PieceHeight; y++)
        {

            if (curPiece_->getPosY() + y < 0)
            {
                continue;
            }
            for (int x = 0; x < PieceWidth; x++)
            {
                int pixelX = (curPiece_->getPosX() + x + 1) * SizeCellPixelX;
                int pixelY = (curPiece_->getPosY() + y + 1) * SizeCellPixelY;
                if (curPiece_->getBlockValue(x, y))
                {
                    HBRUSH hBrush = CreateSolidBrush(curPiece_->getColorValue(x, y));
                    HBRUSH hOldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
                    Rectangle(hdc,
                        pixelX,
                        pixelY,
                        pixelX + SizeCellPixelX,
                        pixelY + SizeCellPixelY);
                    SelectObject(hdc, hOldBrush);
                    DeleteObject(hBrush);
                }
            }
        }

        // �ΤΥ֥�å�
        for (int y = 0; y < PieceHeight; y++)
        {
            for (int x = 0; x < PieceWidth; x++)
            {
                int pixelX = (FieldWidth + 2 + x) * SizeCellPixelX;
                int pixelY = (y + 1) * SizeCellPixelY;
                if (nextPiece_->getBlockValue(x, y))
                {
                    HBRUSH hBrush = CreateSolidBrush(nextPiece_->getColorValue(x, y));
                    HBRUSH hOldBrush = reinterpret_cast<HBRUSH>(SelectObject(hdc, hBrush));
                    Rectangle(hdc,
                        pixelX,
                        pixelY,
                        pixelX + SizeCellPixelX,
                        pixelY + SizeCellPixelY);
                    SelectObject(hdc, hOldBrush);
                    DeleteObject(hBrush);
                }
            }
        }

        SelectObject(hdc, GetStockObject(WHITE_PEN));
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc,
            (FieldWidth + 2) * SizeCellPixelX, SizeCellPixelY,  // �ΤΥ֥�å��Ζ�
            (FieldWidth + 2 + PieceWidth) * SizeCellPixelX, (PieceHeight + 1) * SizeCellPixelY);
        Rectangle(hdc,
            SizeCellPixelX, SizeCellPixelY,                 // ���`��ե��`��ɤΖ�
            (FieldWidth + 1) * SizeCellPixelX, (FieldHeight + 1) * SizeCellPixelY);

        drawLine(hdc);

        drawString(hdc);
    }

    bool isGameover()
    {
        return isGameover_;
    }

    void resetState()
    {
        score_ = 0;
        playTime_ = 0;
        isGameover_ = false;
    }


private:
    void drawLine(HDC hdc)
    {
        // DC�ڥ��ɫ��������
        SetDCPenColor(hdc, RGB(127, 127, 127));
        SelectObject(hdc, GetStockObject(DC_PEN));
        // �ڂ��軭 ����ǥå�����ע��
        for (int x = 1; x < FieldWidth; x++)
        {
            int pixelX = (x + 1) * SizeCellPixelX;
            int pixelY = 1 * SizeCellPixelY;
            MoveToEx(hdc, pixelX, pixelY, NULL);
            LineTo(hdc, pixelX, (FieldHeight + 1) * SizeCellPixelY);
        }
        for (int y = 1; y < FieldHeight; y++)
        {
            int pixelX = 1 * SizeCellPixelX;
            int pixelY = (y + 1) * SizeCellPixelX;
            MoveToEx(hdc, pixelX, pixelY, NULL);
            LineTo(hdc, (FieldWidth + 1) * SizeCellPixelX, pixelY);
        }
    }

    void drawString(HDC hdc)
    {
        TCHAR    buf[16];
        HFONT    hFont;

        SetTextColor(hdc, RGB(255, 255, 255));
        SetBkMode(hdc, TRANSPARENT);    // ������Ф�

        // �ե���Ȥ��O��
        hFont = CreateFont(
            18,            // �ե���ȥ�����
            0, 0, 0,
            FW_NORMAL, FALSE, FALSE, FALSE,
            SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH, _T("Meiryo UI")
            );
        SelectObject(hdc, hFont);

        // �@�õ���
        _stprintf(buf, _T("SCORE"));
        TextOut(hdc, (FieldWidth + 2) * SizeCellPixelX,
            (PieceHeight + 2) * SizeCellPixelY, buf, (int)_tcslen(buf));
        _stprintf(buf, _T("%d"), score_);
        TextOut(hdc, (FieldWidth + 2) * SizeCellPixelX,
            (PieceHeight + 3) * SizeCellPixelY, buf, (int)_tcslen(buf));
        // �ץ쥤�r�g
        _stprintf(buf, _T("PLAY TIME"));
        TextOut(hdc, (FieldWidth + 2) * SizeCellPixelX,
            (PieceHeight + 5) * SizeCellPixelY, buf, (int)_tcslen(buf));
        _stprintf(buf, _T("%02d:%02d"),
            (playTime_ / 1000) / 60, (playTime_ / 1000) % 60);
        TextOut(hdc, (FieldWidth + 2) * SizeCellPixelX,
            (PieceHeight + 6) * SizeCellPixelY, buf, (int)_tcslen(buf));

        DeleteObject(hFont);
    }

    DWORD WINAPI executeThread()
    {
        // �ߥ�`�ƥå������֥������ȤΥϥ�ɥ��ȡ��
        HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);

        DWORD  signal = -1;
        DWORD  prevTime = timeGetTime();
        DWORD  sleeptime = 1000;
        DWORD  minute = 0;

        while (true)
        {
            // �ᥤ�󥹥�å�(=���֤��¤˄Ӥ���������)����ν��뤬�ʤ��ޤ�
            // �����ॢ���Ȥˤ�äƴ��C��������
            DWORD difftime = timeGetTime() - prevTime;
            // ����`�וr�g���U�^���Ƥ��ʤ��ʤ��
            if (difftime < sleeptime)
            {
                // ָ���r�g�U�^����ȥ��֥������Ȥ򥷥��ʥ�״�B(=�K��)�ˤ��롣
                // ���Иؤ�֤ĤޤǴ��ġ�
                OutputDebugString(_T("Mutex Wait Start ...\n"));
                signal = WaitForSingleObject(hMutex, sleeptime - difftime);
                // �����ޤ�����Τ˺Τ⤷�ʤ���1�뤫����
                OutputDebugString(_T("Mutex Wait End ...\n"));
            }
            difftime = timeGetTime() - prevTime;
            playTime_ += difftime;
            minute += difftime;
            // 1�֤��Ȥ˴��C�r�g��p�餷�Ƥ椯
            if (minute >= 60 * 1000 && sleeptime > 100)
            {
                sleeptime -= 100;
                minute = 0;
            }
            prevTime = timeGetTime();     

            bool isSucceeded = curPiece_->move(Piece::Down);
            if (!isSucceeded)
            {
                int line = field_->deleteLine();
                if (line > 0)
                {
                    // �Ƅ��ФΥ֥�å������0�ˤ��Ƥ��
                    curPiece_->clear();
                    if (line == 4)
                    {
                        score_ += 1000;
                    }
                    else
                    {
                        score_ += (line * 100);
                    }
                    InvalidateRect(hWnd_, NULL, FALSE);
                    // ������˲�g���ݳ�
                    Sleep(500);
                    field_->shiftLine(line);
                }
                if (field_->isStacked())
                {
                    isGameover_ = true;
                    ReleaseMutex(hMutex);   // �ߥ�`�ƥå��������Иؤ���
                    CloseHandle(hMutex);    // ȫ�ƤΥϥ�ɥ���]���ʤ��ޤ�ߥ�`�ƥå������Ɨ�����ʤ�
                    int ans = MessageBox(hWnd_, _T("Continue ?"), _T("Gameover"), MB_YESNO);
                    if (ans == IDYES)
                    {
                        SendMessage(hWnd_, WM_RESTART, 0, 0);
                    }
                    else if (ans == IDNO)
                    {
                        SendMessage(hWnd_, WM_DESTROY, 0, 0);
                    }
                    return 0;       // ���`��K�ˤǥ���åɤ��Ɨ�����
                }
                // �¤����ԩ`��������
                releasePiece(curPiece_);
                curPiece_ = nextPiece_;
                nextPiece_ = createPiece();
            }
            InvalidateRect(hWnd_, NULL, FALSE);
            // ���֥������Ȥ������ʥ�״�B(=�K��)�ʤ�
            if (signal == WAIT_OBJECT_0)
            {
                // �ߥ�`�֥������֥������Ȥ����Иؤ��Ť���
                ReleaseMutex(hMutex);
                SendMessage(hWnd_, WM_MUTEX, 0, 0);
            }

        }
        return 0;
    }

    HWND      hWnd_;
    Piece     *nextPiece_;
    Piece     *curPiece_;
    Field     *field_;
    int       playTime_;
    int       score_;
    bool      isGameover_;

};

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HANDLE              ReInitialize(HWND hWnd);
Tetris              *tetris;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MSG msg;
    HACCEL hAccelTable;

    // ����`�Х������Ф���ڻ����Ƥ��ޤ���
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_WIN32TETRIS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���ץꥱ�`�����γ��ڻ���g�Ф��ޤ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32TETRIS));

    // �ᥤ�� ��å��`�� ��`��:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32TETRIS));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_WIN32TETRIS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;

    hInst = hInstance; // ����`�Х�����˥��󥹥��󥹄I����{���ޤ���

    RECT rc = {
        0, 0,
        (FieldWidth + 7) * SizeCellPixelX, (FieldHeight + 2) * SizeCellPixelY
    };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, FALSE);

    hWnd = CreateWindow(szWindowClass, szTitle,
        (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX) | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HANDLE  hMutex;
    static HANDLE  hThread;
    static HDC     hMemDC;
    static HDC     hTmpDC;
    static HBITMAP hBackBitmap;
    static HBITMAP hFrontBitmap;

    // �����ǄӤ��������򤹤�
    // �ե饰�ˤ�ä��ж�moveFlag�ȤǤ⤹�뤫��
    // tetris->getPiece()->isMoving()
    switch (message)
    {
    case WM_CREATE:
        {
            HDC     hdc = nullptr;
            UINT    uId;

            tetris = new Tetris(hWnd);
            hdc = GetDC(hWnd);      // ������ɥ���HDC��ȡ��
            // �������� �\ɫ���������(����״�B��DDB���\�ǉT��Ĥ֤���Ƥ���)
            // ������ɥ���HDC�Ȼ��Q�ԤΤ���DDB����
            hBackBitmap = CreateCompatibleBitmap(hdc, SizeBmPixelX, SizeBmPixelY);

            hMemDC = CreateCompatibleDC(hdc);   // ������ɥ���HDC�Ȼ��Q�ԤΤ���DC������
            SelectObject(hMemDC, hBackBitmap);  // DDB��ǥХ�������ƥ����Ȥ��O��

            // ���»��� �\ɫ���������(����״�B��DDB���\�ǉT��Ĥ֤���Ƥ���)
            // ������ɥ���HDC�Ȼ��Q�ԤΤ���DDB����
            hFrontBitmap = CreateCompatibleBitmap(hdc, SizeBmPixelX, SizeBmPixelY);
            hTmpDC = CreateCompatibleDC(hdc);    // ������ɥ���HDC�Ȼ��Q�ԤΤ���DC������ 
            SelectObject(hTmpDC, hFrontBitmap);  // DDB��ǥХ�������ƥ����Ȥ��O��
            ReleaseDC(hWnd, hdc);   // ������ɥ���HDC����

            srand(static_cast<UINT>(time(NULL)));
            hMutex = CreateMutex(NULL, TRUE, MUTEX_NAME);
            hThread = reinterpret_cast<HANDLE>(
                _beginthreadex(
                NULL,                // security
                0,                   // stack_size for a new thread, or 0
                Tetris::threadProc,  // start_address
                reinterpret_cast<void *>(tetris),  // arglist
                0,                   // initflag 0 for running
                &uId)                // thrdaddr (identifier)
                );
            break;
        }
    case WM_COMMAND:
        {
            int wmId, wmEvent;
            wmId = LOWORD(wParam);
            wmEvent = HIWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // ���������軭 ���֥�Хåե����
            BitBlt(hTmpDC, 0, 0, SizeBmPixelX, SizeBmPixelY,
                hMemDC, 0, 0, SRCCOPY);
            tetris->paint(hTmpDC);
            // ǰ�沿���軭 ���֥�Хåե����
            BitBlt(hdc, 0, 0, SizeBmPixelX, SizeBmPixelY,
                hTmpDC, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
            break;
        }
    case WM_DESTROY:
        {
            DeleteObject(hBackBitmap);
            DeleteObject(hFrontBitmap);
            DeleteDC(hMemDC);
            DeleteDC(hTmpDC);
            CloseHandle(hThread);
            CloseHandle(hMutex);
            delete tetris;
            PostQuitMessage(0);
            break;
        }
    case WM_KEYDOWN:
        {
            bool isSucceeded = false;
            switch (wParam)
            {
            case VK_LEFT:
                isSucceeded = tetris->getPiece()->move(Piece::Left);
                break;
            case VK_RIGHT:
                isSucceeded = tetris->getPiece()->move(Piece::Right);
                break;
            case VK_DOWN:
                isSucceeded = tetris->getPiece()->move(Piece::Down);
                if (!isSucceeded)
                {
                    ReleaseMutex(hMutex);
                }
                break;
            case VK_SPACE:
                isSucceeded = tetris->getPiece()->turn();
                break;
            }
            if (isSucceeded)
            {
                InvalidateRect(hWnd, NULL, FALSE);
            }
            break;
        }
    case WM_MUTEX:
        {
            // ���֥������Ȥ������ʥ�״�B(=�K��)�ˤʤ�ޤǴ��C
            WaitForSingleObject(hMutex, INFINITE);
            break;
        }
    case WM_RESTART:
        {
            if (tetris->isGameover())
            {
                WaitForSingleObject(hMutex, INFINITE);
                CloseHandle(hThread);
                hThread = ReInitialize(hWnd);
            }
            break;
        }
    default:
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}

HANDLE ReInitialize(HWND hWnd)
{
    delete tetris;
    tetris = new Tetris(hWnd);
    UINT uId;
    return reinterpret_cast<HANDLE>(
        _beginthreadex(
        NULL,                // security
        0,                   // stack_size for a new thread, or 0
        Tetris::threadProc,  // start_address
        reinterpret_cast<void *>(tetris),  // arglist
        0,                   // initflag 0 for running
        &uId)                // thrdaddr (identifier)
    );
}

// �Щ`��������ܥå����Υ�å��`�� �ϥ�ɥ�`�Ǥ���
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return static_cast<INT_PTR>(TRUE);

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return static_cast<INT_PTR>(TRUE);
        }
        break;
    }
    return static_cast<INT_PTR>(FALSE);
}