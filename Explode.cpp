#include "Explode.h"

Explode::Explode()//����� ������ ���������� 
				  //������������
{}                //����� ��������� �� ���������

Explode::Explode(int i)//����������� � ����������
{
	num = i;
	alive = false;
	burn = false;
	Xpos = 0;
	Ypos = 0;
	Zpos = 0;
	Ydeg = 0;
	start_time = GetTickCount();

	dbMakeFireParticles(num, fire, 3000, 0, 0, 0, 
	10, 15, 10);//������ ����������� ������ � ����
	//����, � � ���������� ������������ ������� 
	//��� �������� ��������

	dbHideParticles(num);// �������� ����������� 
	//������ � ���������

	dbPositionParticles(num, Xpos, Ypos, Zpos);
	//������������� ����������� ������ � ���������
}

Explode::~Explode()//����������: 
//�������� ��� ������-�����, ���������� ������
{
	dbDeleteParticles(num);
	num = 0;
	alive = false;
	burn = false;
	Xpos = 0;
	Ypos = 0;
	Zpos = 0;
	Ydeg = 0;
	start_time = 0;
}

void Explode::Draw()//����������� � ��������, 
//���������� � ������ �� �����
{
	const int FireToLive = 999;
	if (alive)
	{
		dbShowParticles(num);
		Xpos = dbParticlesPositionX(num);
		Zpos = dbParticlesPositionZ(num);
		Ypos = dbParticlesPositionY(num);
		dbPositionParticles(num, Xpos, Ypos, Zpos);
if ((GetTickCount() - start_time) > FireToLive) 
Die();
	} else dbHideParticles(num);
}

void Explode::Die()//������������ ������
{
	alive = false;
}

void Explode::Fire(float x, float y, float z)
//��������� ������
{
	if (!alive) {
		alive = true;
	start_time = GetTickCount();//�������� ����� 
	Pos(x, y, z);
	}
}


void Explode::Pos(float x, float y, float z)
//���������������� ������
{
	Xpos = x;
	Ypos = y;
	Zpos = z;
	dbPositionParticles(num, Xpos, Ypos, Zpos);
	//���������������� � ������� �����������
}