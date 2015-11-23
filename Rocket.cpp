#include "Rocket.h"

Rocket::Rocket()
//����� ������ ���������� ������������
{}                //����� ��������� �� ���������

Rocket::Rocket(int i, char *filename)
{
	num = i;
	alive = false;//������ ��������������
	burn = false;
	Xpos = 0;
	Ypos = 0;
	Zpos = 0;
	Ydeg = 0;
	start_time = GetTickCount();
	if (num == myrocket)//������ ������
	{
dbLoadObject(filename,num);//��������� � �� �����,
	}
	else //��� ��� ��� ������ ���������, ��...
	{
		dbCloneObject(num, myrocket);
		//...��������� �������� � ������
	}
		dbHideObject(num);//��������
	dbPositionObject(num, Xpos, Ypos, Zpos);
	dbYRotateObject(num, Ydeg);
}

Rocket::~Rocket()//����������: 
//�������� ��� ������-�����, ���������� ������
{
	dbDeleteObject(num);
	num = 0;
	alive = false;//������ ��������������
	burn = false;
	Xpos = 0;
	Ypos = 0;
	Zpos = 0;
	Ydeg = 0;
	start_time = 0;
}

void Rocket::Draw(int terrain)//����������� ���
//�������: ���������� � ��������� ��������������
//� �.�.
{
const float rocketSpeed = 1.1;//������� �����������
//������
const int RockToLive = 3000;

if (alive)  { //���� ������ ��������������, 
//�� ���������� � � 
dbShowObject(num);// ���������� ����������� ��� ���
dbMoveObject(num, rocketSpeed);

Xpos = dbObjectPositionX(num);
Ypos = dbObjectPositionY(num);
Zpos = dbObjectPositionZ(num);

float ground = dbGetTerrainGroundHeight(terrain, 
		Xpos,// ����������, �������� ������  
		Zpos);// ����������� ���������
if (ground >= Ypos) {
	// ��������� ������������ � ������������
		Die();
	}

	dbPositionObject(num, Xpos,// ���������� ������
		Ypos, Zpos);

if ((GetTickCount() - start_time) > RockToLive) 
Die();
} else dbHideObject(num);//����� �������� �
}

void Rocket::Fire(float x, float y, float z, 
	float deg)//������� - ������ ����� ������
{
if (!alive) {
alive = true;
PlaySound(RobotShort, x, y, z);
//�������� "��������" �������
start_time = GetTickCount();//�������� ����� ������
// ��������� ������� ������� �� �������� �� �������
// ����� ����� ����������� ������� �� ����� �������� 
// ���-�� ��� ����� ����������������
Pos(x, y, z, deg);
}
}

void Rocket::Pos(float x, float y, float z, 
float deg)//������ ���������������� �������-������
{
	Xpos = x;
	Ypos = y;
	Zpos = z;
	Ydeg = deg;

dbPositionObject(num, Xpos, Ypos, Zpos);
//���������������� � ������� �����������

dbMoveObject(num, 2);//������������� �������� -
dbMoveObjectRight(num, 2);//����������������
dbMoveObjectUp(num, 2.5);//� ��������� �����������

dbYRotateObject(num, Ydeg);
}

void Rocket::Die()//����� ������� ����� ������
{
	burn = true;//�������
	PlaySound(RocketExplode, Xpos, Ypos, Zpos);
	//�������� "��������" �������
	alive = false;
}

void Rocket::PlaySound(int sound, float x, float y,
float z)//����������� ����, ���������� � �������� 
//������� ���������, � ����������
{// ��� � ������� ��� �� �������� �����������
dbPositionSound(sound, x, y, z);//�����������
dbPlaySound(sound);//����������, ��������� ������
}