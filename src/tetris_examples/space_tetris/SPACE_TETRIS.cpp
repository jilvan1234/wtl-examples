#include<windows.h>
#include<time.h>
#include <stdlib.h>
#include <MMSystem.h>

// winmm.lib ���󥯤���
#pragma comment(lib,"winmm")

// ����`
#define ERR -1

// �ԩ`���κ�ȿk�Υޥ���
#define PIECE_WIDTH		4
#define PIECE_HEIGHT	4

/* �ե��`��ɤκ�ȿk�Υޥ��� */
#define FIELD_WIDTH		14
#define FIELD_HEIGHT	24

// �ޥ��Υԥ�������
#define CELL_WIDTH		20
#define CELL_HEIGHT		20

/* MovePiece �v�������� */
#define PIECE_LEFT		2
#define PIECE_RIGHT		4
#define PIECE_DOWN		8

BYTE field[FIELD_WIDTH][FIELD_HEIGHT]={0};		/* ���`��ե��`��� */
DWORD fColor[FIELD_WIDTH][FIELD_HEIGHT]={0};	// ���`��ե��`��ɤ�ɫ

BYTE piece[PIECE_WIDTH][PIECE_HEIGHT]={0};		/* �F���Ƅ��ФΥ֥�å� */
DWORD pColor[PIECE_WIDTH][PIECE_HEIGHT]={0};	// �F���Ƅ��ФΥ֥�å���ɫ
POINT location={0,0};							/* �F���Ƅ��ФΥ֥�å���λ�� */

BYTE next[PIECE_WIDTH][PIECE_HEIGHT]={0};		// �ΤΥ֥�å�
DWORD nColor[PIECE_WIDTH][PIECE_HEIGHT]={0};	// �ΤΥ֥�å���ɫ

DWORD score=0;			// �@�õ���
DWORD playTime=0;		// �ץ쥤�r�g
BOOL GameOver=FALSE;	// TRUE �Ȥʤ�Τϥ��`�४�`�Щ`�����ץ쥤����ޤ�

#define MUTEX_NAME	"MutexObject of SPACE TETRIS"	// �ߥ�`�ƥå������֥������Ȥ���ǰ

#define WM_MUTEX	WM_APP		// �ᥤ�󥹥�åɤ˥ߥ�`�ƥå��������И�ȡ�ä�Ҫ�󤹤��å��`��

/* piece[][] �ڤΥ֥�å������ϲ���λ�ä򷵤� */
int GetPieceTop(void)
{
	for(int y=0;y<PIECE_HEIGHT;y++){
		for(int x=0;x<PIECE_WIDTH;x++){
			if(piece[x][y]){
				return y;
			}
		}
	}
	return ERR;
}

/* piece[][]���ڤΥ֥�å������²���λ�ä򷵤� */
int GetPieceBottom(void)
{
	for(int y=PIECE_HEIGHT-1;y>=0;y--){
		for(int x=0;x<PIECE_WIDTH;x++){
			if(piece[x][y]){
				return y;
			}
		}
	}
	return ERR;
}

/* piece[][]���ڤΥ֥�å�����Ȥ�λ�ä򷵤� */
int GetPieceLeft(void)
{
	for(int x=0;x<PIECE_WIDTH;x++){
		for(int y=0;y<PIECE_HEIGHT;y++){
			if(piece[x][y]){
				return x;
			}
		}
	}
	return ERR;
}

/* piece[][]���ڤΥ֥�å����҂Ȥ�λ�ä򷵤� */
int GetPieceRight(void)
{
	for(int x=PIECE_WIDTH-1;x>=0;x--){
		for(int y=0;y<PIECE_HEIGHT;y++){
			if(piece[x][y]){
				return x;
			}
		}
	}
	return ERR;
}

/* �֥�å����Ƅ��ж� */
// ���ꂎ��TURE(�ƄӤ���) or FALSE(�ƄӲ���)
BOOL MovePiece(int move)	// move���ƄӤ���������
{
	int x,y,left,right,bottom;
	switch(move){
		case PIECE_LEFT:
			left=GetPieceLeft();

			if((location.x)+left <= 0) return FALSE;

			for(y=0;y<PIECE_HEIGHT;y++){		// ��(location.x)+x-1>=0 , (location.y)+y>=0
				for(x=0;x<PIECE_WIDTH;x++){		// �����֤��Є��Ԥ��{�٤Ƥ���
					if(piece[x][y] && (location.x)+x-1>=0 && (location.y)+y>=0
						&& field[(location.x)+x-1][(location.y)+y]){	// һ����˥֥�å�������
                            return FALSE;
					}
				}
			}
			location.x--;
			return TRUE;
		case PIECE_RIGHT:
            right=GetPieceRight();

			if((location.x)+right >= FIELD_WIDTH-1) return FALSE;

			for(y=0;y<PIECE_HEIGHT;y++){		// ��(location.x)+x+1<FIELD_WIDTH , (location.y)+y>=0
				for(x=0;x<PIECE_WIDTH;x++){		// �����֤��Є��Ԥ��{�٤Ƥ���
					if(piece[x][y] && (location.x)+x+1<FIELD_WIDTH && (location.y)+y>=0
						&& field[(location.x)+x+1][(location.y)+y]){	// һ���Ҥ˥֥�å�������
                            return FALSE;
					}
				}
			}
			location.x++;
			return TRUE;
		case PIECE_DOWN:
            bottom=GetPieceBottom();

			if((location.y)+bottom >= FIELD_HEIGHT-1) return FALSE;

			for(y=0;y<PIECE_HEIGHT;y++){		// ��(location.y)+y+1>=0 , (location.y)+y+1<FIELD_HEIGHT
				for(x=0;x<PIECE_WIDTH;x++){		// �����֤��Є��Ԥ��{�٤Ƥ���
					if(piece[x][y] && (location.y)+y+1>=0 && (location.y)+y+1<FIELD_HEIGHT
						&& field[(location.x)+x][(location.y)+y+1]){	// һ���¤˥֥�å�������
                            return FALSE;
					}
				}
			}
			location.y++;
			return TRUE;
	}
	return FALSE;
}

/* �֥�å����ܞ������ */
// ���ꂎ��TURE(��ܞ����) or FALSE(��ܞ����)
BOOL TurnPiece(void)
{
	int x,y,offsetX,offsetY;
	BYTE pTurn[PIECE_WIDTH][PIECE_HEIGHT];
	DWORD pcTurn[PIECE_WIDTH][PIECE_HEIGHT];

	/* ��ܞ�����֥�å������ɤ��� */
	for(y=0;y<PIECE_HEIGHT;y++){
		for(x=0;x<PIECE_WIDTH;x++){
			pTurn[(PIECE_HEIGHT-1)-y][x]=piece[x][y];
			pcTurn[(PIECE_HEIGHT-1)-y][x]=pColor[x][y];
		}
	}
	/* ��ܞ���ܤ��ɤ������{�٤� */
	for(y=0;y<PIECE_HEIGHT;y++){
		for(x=0;x<PIECE_WIDTH;x++){
			if(pTurn[x][y]){
				offsetX=(location.x)+x;
				offsetY=(location.y)+y;
				if(offsetX<0 || offsetX>=FIELD_WIDTH
					|| offsetY>=FIELD_HEIGHT	// ��offsetY>=0 �����֤��Є��Ԥ��{�٤Ƥ���
					|| (offsetY>=0 && field[offsetX][offsetY])){	//�Ȥ˥֥�å�������
                        return FALSE;
					}
			}
		}
	}
	for(y=0;y<PIECE_HEIGHT;y++){
		for(x=0;x<PIECE_WIDTH;x++){
			piece[x][y]=pTurn[x][y];
			pColor[x][y]=pcTurn[x][y];
		}
	}
	return TRUE;
}

/* ���Ф��{�١��Ф���ޤäƤ�����Ϥ��Ф��������� */
// ���ꂎ��������������
int DeleteLine(void)
{
	int x,y,delCount=0;
	for(y=FIELD_HEIGHT-1;y>=0;y--){
		int lineCount=0;
		for(x=0;x<FIELD_WIDTH;x++){
			lineCount+=field[x][y];
		}

		if(lineCount==0) break;		/* �������Ϥ˥֥�å��Ϥʤ� */
		if(lineCount!=FIELD_WIDTH) continue;

		/* һ���������� */
		delCount++;
		for(x=0;x<FIELD_WIDTH;x++){
			field[x][y]=0;
		}
	}
	return delCount;
}

// ���������Ф�ԑ���
void ShiftLine(int delCount)	// delCount��������������
{
	int x,y;
	for(y=FIELD_HEIGHT-1;y>=0 && delCount>0; ){
		int lineCount=0;
		for(x=0;x<FIELD_WIDTH;x++){
			lineCount+=field[x][y];
		}

		if(lineCount!=0){
			y--;
			continue;
		}

		// һ��ԑ���
		delCount--;
		for(int iy=y;iy>=0;iy--){
			for(x=0;x<FIELD_WIDTH;x++){
				if(iy-1>=0){
					field[x][iy]=field[x][iy-1];
					fColor[x][iy]=fColor[x][iy-1];
				}else{
					field[x][0]=0;		/* 0 �Ф���ϤϤʤ��Τ� 0 ������ */
					fColor[x][0]=0;
				}
			}
		}
	}
}

// �ΤΥ֥�å��򤢤餫�������äƤ���
void CreatePiece(void)
{
	for(int y=0;y<PIECE_HEIGHT;y++){
		for(int x=0;x<PIECE_WIDTH;x++){
			next[x][y]=0;
			nColor[x][y]=0;
		}
	}
	switch(rand()%7){
        case 0:
			next[1][1]=1; next[2][1]=1; next[1][2]=1; next[2][2]=1;
			nColor[1][1]=nColor[2][1]=nColor[1][2]=nColor[2][2]=0x000000ff;
			return;
		case 1:
			next[1][0]=1; next[1][1]=1; next[1][2]=1; next[1][3]=1;
			nColor[1][0]=nColor[1][1]=nColor[1][2]=nColor[1][3]=0x0000ff00;
			return;
		case 2:
			next[1][1]=1; next[1][2]=1; next[2][2]=1; next[1][3]=1;
			nColor[1][1]=nColor[1][2]=nColor[2][2]=nColor[1][3]=0x0000ffff;
			return;
		case 3:
			next[1][1]=1; next[2][1]=1; next[1][2]=1; next[1][3]=1;
			nColor[1][1]=nColor[2][1]=nColor[1][2]=nColor[1][3]=0x00ff0000;
			return;
		case 4:
			next[1][1]=1; next[2][1]=1; next[2][2]=1; next[2][3]=1;
			nColor[1][1]=nColor[2][1]=nColor[2][2]=nColor[2][3]=0x00ff00ff;
			return;
		case 5:
			next[2][1]=1; next[1][2]=1; next[2][2]=1; next[1][3]=1;
			nColor[2][1]=nColor[1][2]=nColor[2][2]=nColor[1][3]=0x00ffff00;
			return;
		case 6:
			next[1][1]=1; next[1][2]=1; next[2][2]=1; next[2][3]=1;
			nColor[1][1]=nColor[1][2]=nColor[2][2]=nColor[2][3]=0x00ffffff;
			return;
	}
}

/* �ΤΥ֥�å��� */
void NextPiece(BOOL first)		// first�����`���_ʼ��������κ��ӳ�������
{
	if(first) CreatePiece();

	for(int y=0;y<PIECE_HEIGHT;y++){
		for(int x=0;x<PIECE_WIDTH;x++){
			piece[x][y]=next[x][y];
			pColor[x][y]=nColor[x][y];
		}
	}
	location.x=5;
	location.y=-3;

	CreatePiece();
}

/* �֥�å���λ�����ˏ��äƥե��`��ɤ˥��ԩ`���� */
void PieceToField(void)
{
	for(int y=0;y<PIECE_HEIGHT;y++){
		for(int x=0;x<PIECE_WIDTH;x++){		// ��(location.y)+y>=0 �����֤��Є��Ԥ��{�٤Ƥ���
			if(piece[x][y] && (location.y)+y>=0){
				field[(location.x)+x][(location.y)+y]=piece[x][y];
				fColor[(location.x)+x][(location.y)+y]=pColor[x][y];
			}
		}
	}
}

// ������������å�
DWORD WINAPI ThreadProc(LPVOID lpParameter)		// lpParameter��������ɥ��Υϥ�ɥ�
{
	HWND hWnd=(HWND)lpParameter;
	HANDLE hMutex=OpenMutex(MUTEX_ALL_ACCESS,FALSE,MUTEX_NAME);
	DWORD beforeTime=timeGetTime();
	DWORD sleep=1000;
	DWORD minute=0;
	DWORD progress,signal=1;
	int x,y,line;

	while(1){								// ���ᥤ�󥹥�åɤ���ν��뤬�ʤ��ޤ�
		progress=timeGetTime()-beforeTime;	// �����ॢ���Ȥˤ�äƴ��C��������
		if(progress<sleep) signal=WaitForSingleObject(hMutex,sleep-progress);
		progress=timeGetTime()-beforeTime;	// ���ᥤ�󥹥�åɤ���ν���ˤ��
		playTime+=progress;					// �����ॢ���Ȥ���Ĥ��Ȥʤ����C�������������Ԥ򿼑]
		minute+=progress;
		if(minute>=60*1000 && sleep>100){
			sleep-=100;		// һ�֤��Ȥ˴��C�r�g��p�餷�Ƥ椯
			minute=0;
		}
		beforeTime=timeGetTime();

		if(!MovePiece(PIECE_DOWN)){		// �F���Ƅ��ФΥ֥�å����¶Τ��_�������
			PieceToField();
			line=DeleteLine();
			if(line>0){
				for(y=0;y<PIECE_HEIGHT;y++){
					for(x=0;x<PIECE_WIDTH;x++){
						piece[x][y]=0;
					}
				}
				if(line==4) score+=1000;
				else score+=(line*100);
				InvalidateRect(hWnd,NULL,FALSE);
				Sleep(500);
				ShiftLine(line);
			}
			if(field[6][0] || field[7][0]){		/* ���`��K�� */
				GameOver=TRUE;
				ReleaseMutex(hMutex);	// �ߥ�`�ƥå��������Иؤ���
				CloseHandle(hMutex);	// ȫ�ƤΥϥ�ɥ���]���ʤ��ޤ�ߥ�`�ƥå������Ɨ�����ʤ�
				MessageBox(hWnd,"���Υ�å��`���ܥå������]�������\n"
					"    Enter���`��Ѻ���С����¤������`�बʼ�ޤ�ޤ�\n"
					"    ���ܥ����Ѻ���С����K�ˤ��ޤ�","GAME OVER",MB_OK);
				return 0;		// ���`��K�ˤǥ���åɤ��Ɨ�����
			}
			NextPiece(FALSE);
		}
		InvalidateRect(hWnd,NULL,FALSE);

		if(signal==WAIT_OBJECT_0){				// �ᥤ�󥹥�åɤ���ν���ˤ����C��������
			ReleaseMutex(hMutex);				// �ߥ�`�ƥå��������Иؤ���
			SendMessage(hWnd,WM_MUTEX,0,0);		// �ᥤ�󥹥�åɤ˥ߥ�`�ƥå��������И�ȡ�ä�Ҫ��
		}
	}
	return 0;
}

// �軭����
void Paint(HDC hdc)		// hdc���ǥХ�������ƥ����ȤΥϥ�ɥ�
{
	int x,y,ptx,pty;
	HBRUSH hBrush,hOldBrush;

	SelectObject(hdc,GetStockObject(NULL_PEN));
	for(y=0;y<FIELD_HEIGHT;y++){		// ���`��ե��`��ɤΥ֥�å�
		for(x=0;x<FIELD_WIDTH;x++){
			ptx=(x+1)*CELL_WIDTH;
			pty=(y+1)*CELL_HEIGHT;
			if(field[x][y]){
				hBrush=CreateSolidBrush(fColor[x][y]);
				hOldBrush=(HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,ptx,pty,ptx+CELL_WIDTH,pty+CELL_HEIGHT);
				SelectObject(hdc,hOldBrush);
				DeleteObject(hBrush);
			}
		}
	}
	for(y=0;y<PIECE_HEIGHT;y++){			// �F���Ƅ��ФΥ֥�å�
		if((location.y)+y < 0) continue;	// ���`��ե��`��ɤΖ�����Ϥ��褫�ʤ�
		for(x=0;x<PIECE_WIDTH;x++){
			ptx=((location.x)+x+1)*CELL_WIDTH;
			pty=((location.y)+y+1)*CELL_HEIGHT;
			if(piece[x][y]){
				hBrush=CreateSolidBrush(pColor[x][y]);
				hOldBrush=(HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,ptx,pty,ptx+CELL_WIDTH,pty+CELL_HEIGHT);
				SelectObject(hdc,hOldBrush);
				DeleteObject(hBrush);
			}
		}
	}
	for(y=0;y<PIECE_HEIGHT;y++){		// �ΤΥ֥�å�
		for(x=0;x<PIECE_WIDTH;x++){
			ptx=(FIELD_WIDTH+2+x)*CELL_WIDTH;
			pty=(y+1)*CELL_HEIGHT;
			if(next[x][y]){
				hBrush=CreateSolidBrush(nColor[x][y]);
				hOldBrush=(HBRUSH)SelectObject(hdc,hBrush);
				Rectangle(hdc,ptx,pty,ptx+CELL_WIDTH,pty+CELL_HEIGHT);
				SelectObject(hdc,hOldBrush);
				DeleteObject(hBrush);
			}
		}
	}
	SelectObject(hdc,GetStockObject(WHITE_PEN));
	SelectObject(hdc,GetStockObject(NULL_BRUSH));
	Rectangle(hdc,CELL_WIDTH,CELL_HEIGHT,					// ���`��ե��`��ɤΖ�
		(FIELD_WIDTH+1)*CELL_WIDTH,(FIELD_HEIGHT+1)*CELL_HEIGHT);
	Rectangle(hdc,(FIELD_WIDTH+2)*CELL_WIDTH,CELL_HEIGHT,	// �ΤΥ֥�å��Ζ�
		(FIELD_WIDTH+2+PIECE_WIDTH)*CELL_WIDTH,(PIECE_HEIGHT+1)*CELL_HEIGHT);

	char buf[32];
	SetTextColor(hdc,RGB(255,255,255));
	SetBkMode(hdc,TRANSPARENT);
	// �@�õ���
	wsprintf(buf,"SCORE");
	TextOut(hdc,(FIELD_WIDTH+2)*CELL_WIDTH,(PIECE_HEIGHT+2)*CELL_HEIGHT,buf,(int)strlen(buf));
	wsprintf(buf,"%d",score);
	TextOut(hdc,(FIELD_WIDTH+2)*CELL_WIDTH,(PIECE_HEIGHT+3)*CELL_HEIGHT,buf,(int)strlen(buf));
	// �ץ쥤�r�g
	wsprintf(buf,"PLAY TIME");
	TextOut(hdc,(FIELD_WIDTH+2)*CELL_WIDTH,(PIECE_HEIGHT+5)*CELL_HEIGHT,buf,(int)strlen(buf));
	wsprintf(buf,"%02d:%02d:%03d",(playTime/1000)/60,(playTime/1000)%60,playTime%1000);
	TextOut(hdc,(FIELD_WIDTH+2)*CELL_WIDTH,(PIECE_HEIGHT+6)*CELL_HEIGHT,buf,(int)strlen(buf));
}

// �ٳ��ڻ�����
// ���ꂎ���¤�������åɤΥϥ�ɥ�
HANDLE ReInitialize(HWND hWnd)		// hWnd��������ɥ��Υϥ�ɥ�
{
	for(int y=0;y<FIELD_HEIGHT;y++){
		for(int x=0;x<FIELD_WIDTH;x++){
			field[x][y]=0;
			fColor[x][y]=0;
		}
	}
	score=0;
	playTime=0;
	GameOver=FALSE;

	DWORD dwID;			// piece[][] , pColor[][] , location , next[][] , nColor[][]
	NextPiece(TRUE);	// �� NextPiece() �ǳ��ڻ������
	return CreateThread(NULL,0,ThreadProc,hWnd,0,&dwID);		// ���`��K����˥���åɤ��Ɨ�����Ƥ���
}

LRESULT CALLBACK WindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	BOOL bl;
	DWORD beforeTime,dwID;
	HBITMAP hBitmap;
	static BITMAP bitmap;
	static HDC hBackDC,hMemDC;
	static HANDLE hThread;		// ������������åɤΥϥ�ɥ�
	static HANDLE hMutex;		// �ߥ�`�ƥå������֥������ȤΥϥ�ɥ�

	switch(uMsg) {
		case WM_CREATE:
			// ����������`�ɤ���
			hBitmap=(HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE),
				"andromeda.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
			if(hBitmap==NULL){
				MessageBox(hWnd,"�ץ�����K�ˤ��ޤ�","�������񤬤���ޤ���",MB_OK);
				SendMessage(hWnd,WM_DESTROY,0,0);
				return 0;
			}
			GetObject(hBitmap,sizeof(BITMAP),&bitmap);
			hdc=GetDC(hWnd);
			hBackDC=CreateCompatibleDC(hdc);
			SelectObject(hBackDC,hBitmap);
			DeleteObject(hBitmap);

			// ����ǥХ�������ƥ����Ȥ�����
			hBitmap=CreateCompatibleBitmap(hdc,bitmap.bmWidth,bitmap.bmHeight);
			hMemDC=CreateCompatibleDC(hdc);
			ReleaseDC(hWnd,hdc);
			SelectObject(hMemDC,hBitmap);
			DeleteObject(hBitmap);

			srand((unsigned)time(NULL));
			hMutex=CreateMutex(NULL,TRUE,MUTEX_NAME);	//����������ߤϥᥤ�󥹥�å�

			NextPiece(TRUE);
			hThread=CreateThread(NULL,0,ThreadProc,hWnd,0,&dwID);	// ����åɤ�����
			return 0;
		case WM_DESTROY:
			DeleteDC(hBackDC);
			DeleteDC(hMemDC);
			CloseHandle(hThread);
			CloseHandle(hMutex);
			PostQuitMessage(0);
			return 0;
		case WM_MUTEX:
			WaitForSingleObject(hMutex,INFINITE);	// �ߥ�`�ƥå��������Иؤ�ȡ�ä���
			return 0;								// (������������åɤ���C������)
		case WM_PAINT:
			hdc=BeginPaint(hWnd,&ps);
			BitBlt(hMemDC,0,0,bitmap.bmWidth,bitmap.bmHeight,hBackDC,0,0,SRCCOPY);
			Paint(hMemDC);
			BitBlt(hdc,0,0,bitmap.bmWidth,bitmap.bmHeight,hMemDC,0,0,SRCCOPY);
			EndPaint(hWnd,&ps);
			return 0;
		case WM_KEYDOWN:
			bl=NULL;
			switch(wParam){
				case VK_LEFT:
                    bl=MovePiece(PIECE_LEFT);
					break;
				case VK_RIGHT:
                    bl=MovePiece(PIECE_RIGHT);
					break;
				case VK_DOWN:
                    bl=MovePiece(PIECE_DOWN);
					if(!bl) ReleaseMutex(hMutex);
					break;	// ���F���Ƅ��ФΥ֥�å���̶��������˄�����������åɤδ��C���
				case VK_SPACE:
					bl=TurnPiece();
					break;
				case VK_ESCAPE:		// һ�rֹͣ
					SuspendThread(hThread);
					beforeTime=timeGetTime();
					MessageBox(hWnd,"���`������_���ޤ�����","һ�rֹͣ��",MB_OK);
					ResumeThread(hThread);
					playTime-=(timeGetTime()-beforeTime);
					break;
				case VK_RETURN:		// ���`�४�`�Щ`���Ƥ�����¤������`���ʼ���
					if(GameOver){
						WaitForSingleObject(hMutex,INFINITE);	// �¤������������������åɤ���C������
						CloseHandle(hThread);					// �ϥ�ɥ���]���Ƥ⥹��åɤϽK�ˤ��ʤ�
						hThread=ReInitialize(hWnd);				// �ٳ��ڻ�(������������åɤ��¤�������)
					}
					break;
			}
			if(bl) InvalidateRect(hWnd,NULL,FALSE);
			break;
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
	PSTR lpCmdLine,int nCmdShow)
{
	WNDCLASS wc;
	MSG msg;

	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	 = WindowProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName	 = NULL;
	wc.lpszClassName = __FILE__;

	if(!RegisterClass(&wc)) return 0;

	// ָ���������饤������I���_�����뤿��˱�Ҫ�ʥ�����ɥ����ˤ�Ӌ�㤹��
	RECT rc={0,0,(FIELD_WIDTH+7)*CELL_WIDTH,(FIELD_HEIGHT+2)*CELL_HEIGHT};
	AdjustWindowRect(&rc,WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,FALSE);

	HWND hWnd=CreateWindow(
		__FILE__,"SPACE TETRIS",
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX) | WS_VISIBLE,
		CW_USEDEFAULT,CW_USEDEFAULT,
		rc.right-rc.left,rc.bottom-rc.top, 
		NULL,NULL,hInstance,NULL);
	if(hWnd==NULL) return 0;

	BOOL bRet;
	while((bRet=GetMessage(&msg,NULL,0,0))!=0){
		if(bRet==-1) break;
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}