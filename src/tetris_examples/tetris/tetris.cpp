///////////////////////////////////////////////////////////////////////////////
//	�e�g���X

#include "el.h"

#define MAIN_SCREEN 1

void MainScreen(void);

DDOBJ block;			// �u���b�N�摜�̃r�b�g�}�b�v
int field[21][12];		// ��ʃf�[�^

// �����Ă���s�[�X�i�V��j
struct Piece {
	int x, y;
} piece[7][4][4] = {
	// �����_
	0, 2, 1, 2, 2, 2, 3, 2,
	1, 0, 1, 1, 1, 2, 1, 3,
	0, 2, 1, 2, 2, 2, 3, 2,
	1, 0, 1, 1, 1, 2, 1, 3,
	// �����`
	1, 1, 2, 1, 1, 2, 2, 2,
	1, 1, 2, 1, 1, 2, 2, 2,
	1, 1, 2, 1, 1, 2, 2, 2,
	1, 1, 2, 1, 1, 2, 2, 2,
	// �t�k�̎�
	0, 1, 0, 2, 1, 2, 2, 2,
	0, 3, 1, 1, 1, 2, 1, 3,
	0, 2, 1, 2, 2, 2, 2, 3,
	1, 1, 1, 2, 1, 3, 2, 1,
	// �k�̎�
	0, 2, 1, 2, 2, 1, 2, 2,
	0, 1, 1, 1, 1, 2, 1, 3,
	0, 2, 0, 3, 1, 2, 2, 2,
	1, 1, 1, 2, 1, 3, 2, 3,
	// �E������K�i
	1, 1, 2, 1, 2, 2, 3, 2,
	2, 1, 2, 2, 3, 0, 3, 1,
	1, 1, 2, 1, 2, 2, 3, 2,
	2, 1, 2, 2, 3, 0, 3, 1,
	// ��������K�i
	1, 2, 2, 1, 2, 2, 3, 1,
	2, 0, 2, 1, 3, 1, 3, 2,
	1, 2, 2, 1, 2, 2, 3, 1,
	2, 0, 2, 1, 3, 1, 3, 2,
	// �C��
	0, 2, 1, 1, 1, 2, 2, 2,
	0, 2, 1, 1, 1, 2, 1, 3,
	0, 2, 1, 2, 1, 3, 2, 2,
	1, 1, 1, 2, 1, 3, 2, 2,
};

///////////////////////////////////////////////////////////////////////////////
//	���C���֐�

int elMain("�e�g���X");
{
	elWindow(192, 416, FALSE);
	elLoop()
	{
		elSetScreen(MAIN_SCREEN, MainScreen());
	}
	elExitMain();
}

///////////////////////////////////////////////////////////////////////////////
//	�E�C���h�E�����֐�

void elCreate(void)
{
	elDraw::Screen(192, 416);
	// �u���b�N�摜�ǂݍ���
	block = elDraw::LoadObject("block.bmp");
	// ��ʃf�[�^������
	for (int y = 0; y < 21; y++) {
		for (int x = 0; x < 12; x++) {
			// ��ʂ̍��E�Ɖ��̋����u���b�N�ɂ��Ē��͋��
			if (x == 0 || x == 11 || y == 20) field[y][x] = 8;
			else field[y][x] = 0;
		}
	}
	elCallScreen(MAIN_SCREEN);
}

///////////////////////////////////////////////////////////////////////////////
//	�L�[�{�[�h�֐�

void elKeyboard(void)
{
	case VK_ESCAPE:
	{
		elDraw::Exit();
		break;
	}
	elExitKeyboard();
}

///////////////////////////////////////////////////////////////////////////////
//	�C�x���g�֐�

long elEvent(void)
{
	elExitEvent();
}

///////////////////////////////////////////////////////////////////////////////
//	���C�����

void MainScreen(void)
{
	int i, x, y;
	bool downFlag = false;			// �������Ȃ�true
	static int pnext;				// �l�N�X�g�s�[�X�ԍ��i�O�`�U�j
	static int pno;					// ���݂̃s�[�X�ԍ��i�O�`�U�j
	static int pr;					// �s�[�X��]�i�O�`�R�j
	static int px, py;				// �s�[�X�̂w�A�x���W
	static int keyLeft = FREE_KEY;	// ���L�[
	static int keyRight = FREE_KEY;	// �E�L�[
	static int keyDown = FREE_KEY;	// ���L�[
	static int keySpace = FREE_KEY;	// �X�y�[�X�L�[
	static bool nextFlag = true;	// �l�N�X�g�s�[�X���o���t���O
	static bool overFlag = false;	// �Q�[���I�[�o�[�t���O
	static int score = 0;			// ���_
	static DWORD rpt = 0;			// �L�[���s�[�g����(ms)
	static DWORD downTime = 1000;	// 1�u���b�N�������鎞��(ms)
	static DWORD downCount;			// �������ԃJ�E���^(ms)

	if (elChangeScreen()) pnext = rand() % 7;
	elDraw::Clear();
	// �l�N�X�g�s�[�X���o�鏈��
	if (nextFlag) {
		px = 4; py = 0; pr = 0;
		pno = pnext;
		pnext = rand() % 7;
		nextFlag = false;
		downCount = timeGetTime();
		if (downTime > 100) downTime -= 10;
		else downTime--;
		if (downTime < 10) downTime = 10;
		// �o���u�ԂɃu���b�N�ɂԂ�������Q�[���I�[�o�[
		for (i = 0; i < 4; i++) {
			if (field[py + piece[pno][pr][i].y][px + piece[pno][pr][i].x]) overFlag = true;
		}
	}
	// �ړ�
	int kx, ky, kr;		// ���̂w�A�x���W�A��]
	kx = px; ky = py; kr = pr;
	elSystem::GetKey(VK_LEFT, &keyLeft);
	elSystem::GetKey(VK_RIGHT, &keyRight);
	elSystem::GetKey(VK_DOWN, &keyDown);
	elSystem::GetKey(VK_SPACE, &keySpace);
	DWORD nowTime = timeGetTime();
	if (keySpace == PUSH_KEY) {
		if (++kr > 3) kr = 0;
	} else if (keyDown == PUSH_KEY || keyDown == HOLD_KEY && nowTime > rpt) {
		ky++; downFlag = true; rpt = nowTime + 10;
	} else if (keyLeft == PUSH_KEY) {
		kx--; rpt = nowTime + 180;
	} else if (keyLeft == HOLD_KEY && nowTime > rpt) {
		kx--; rpt = nowTime + downTime / 10;
	} else if (keyRight == PUSH_KEY) {
		kx++; rpt = nowTime + 180;
	} else if (keyRight == HOLD_KEY && nowTime > rpt) {
		kx++; rpt = nowTime + downTime / 10;
	} else if (nowTime - downCount > downTime) {
		ky++; downFlag = true; downCount = nowTime;
	}
	// �����蔻��
	for (i = 0; i < 4; i++) {
		if (field[ky + piece[pno][kr][i].y][kx + piece[pno][kr][i].x]) break;
	}
	if (i >= 4) {
		px = kx; py = ky; pr = kr;
	} else if (downFlag) {
		// �s�[�X���ł߂鏈��
		for (i = 0; i < 4; i++) {
			field[py + piece[pno][pr][i].y][px + piece[pno][pr][i].x] = pno + 1;
		}
		nextFlag = true;
		// �u���b�N����������
		int sc = 0;
		for (y = 19; y >= 0; y--) {
			for (x = 1; x < 11; x++) if (field[y][x] == 0) break;
			if (x == 11) {
				for (int y2 = y - 1; y2 >= 0; y2--) {
					for (int x2 = 1; x2 < 11; x2++) field[y2 + 1][x2] = field[y2][x2];
				}
				y++;
				sc = sc ? sc * 4 : 10;
			}
		}
		score += sc;
		Sleep(400);
	}
	// ��ʕ\��
	for (y = 0; y < 21; y++) {
		for (x = 0; x < 12; x++) {
			if (field[y][x]) {
				elDraw::Layer(x * 16, y * 16 + 80, block,
					(field[y][x] - 1) * 16, 0, field[y][x] * 16, 16);
			}
		}
	}
	for (i = 0; i < 4; i++) {
		// ���݂̃s�[�X�\��
		elDraw::Layer((px + piece[pno][pr][i].x) * 16,
			(py + piece[pno][pr][i].y) * 16 + 80,
			block, pno * 16, 0, pno * 16 + 16, 16);
		// �l�N�X�g�s�[�X�\��
		elDraw::Layer(piece[pnext][0][i].x * 16 + 64,
			piece[pnext][0][i].y * 16,
			block, pnext * 16, 0, pnext * 16 + 16, 16);
	}
	// �X�R�A�\��
	SHOW2(0, 0, "SCORE : %d", score);
	elDraw::Refresh();
	// �Q�[���I�[�o�[����
	if (overFlag) {
		elSystem::Message("�Q�[���I�[�o�[");
		elDraw::Exit();
	}
}
