#ifndef LOCATION_H_
#define LOCATION_H_


/*�ں�����չ�� LocationӦ�������� �Խ��и���׼ȷ�Ķ�λ����ײ���*/
/*��ǰ��ÿ�������ΪԲ ���ֻ��Ҫһ��Բ�����꼴��*/

struct Location
{
	int m_px;
	int m_py;

	Location(int x = 0, int y = 0) :
		m_px(x),
		m_py(y) {};
	
	void setLocation(int x, int y)
	{
		m_px = x;
		m_py = y;
	}

	int getLocX()const{return m_px;}
	int getLocY()const{return m_py;}
	int cal_distance(const Location& loc)const
	{
		int a = m_px - loc.m_px;
		int b = m_py - loc.m_py;
		return sqrt(a*a + b*b);
	}
};


#endif