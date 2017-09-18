#ifndef GAME_H_
#define GAME_H_

#include"Player.h"
#include"playerPlane.h"
#include"enemyPlane.h"
#include"weapon_gun.h"
#include"weapon_missle.h"
#include<time.h>
#include"costant.h"

/*���������Ϸ������*/
class Game
{
private:

	Player* m_player;
	PlayerPlane*m_fighter;		//player�ɻ�
	int m_maxEnemyPlane;		//���л�����
	Arr<Plane>m_enemy;			//�л�
	unsigned int m_timer;
public:
	Game()
	{
		m_timer = 0;
	}
	void addPlayer(Player*p)
	{
		m_player = p;
		m_fighter = new PlayerPlane(m_player->m_type,&Fighter,0,START_X, START_Y);
		m_maxEnemyPlane = (m_player->m_dificulty + 2) * 5;
	}
	int regEnemy(int tp, int x, int y)
	{
		EnemyPlane*t = new EnemyPlane(tp, &Ufo1,0,x, y);
		return m_enemy.insert(t);
	}
	bool isover()const
	{
		return m_player->m_left == 0;
	}
	
	/*��Ϸ���¿�ʼ*/
	void restart();

	/*��Ϸ��������*/
	void gameover();

	/*��Ϸ��ʼ*/
	void play();

	/*��ͼ����*/
	void Show();

	/*��������ɻ����˺�*/
	void Hit();
	
	/*��ȡ�û����� �����ɻ�״̬*/
	void Control();

	/*�ı���һʱ�̸���Ԫ�ص�״̬*/
	void React();

	~Game()
	{
		delete m_fighter;
	}
};




#endif
