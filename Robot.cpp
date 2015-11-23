#include "Robot.h"
#include <String.h>//������������ ����, ����������
//������� ��� ������ �� ������������ �-��������

//���-�� �����, ���������� ����� ��� ���� �������� 
//������
int wincount = 0;//� ��� ����������� ��������� 
//������

Robot::Robot()//����� ������ ���������� ������������
{}              //����� ��������� �� ���������

Robot::Robot(int i, int life, float x, float y, 
			 float z, float degree, char* filename,
			 int multigame)
{
	const float animSpeed = 480;//�������� ��������

	num = i;
	lifes = life;
	cnumber = 0;
   multiplayer = multigame;//������������� ��� ����
	if (multiplayer == 1) alive = true; 
	else {
		if (multiplayer == 2) {
	if (num == myrobot) alive = true;//����.���� 
	//������ ����� (� ��������), 
			else alive = false;//��������� ���� ���
		}
	}
	collide = false;
	burn = false;
	evil = false;
	start_time = GetTickCount();
	Xpos = x;
	Ypos = y;
	Zpos = z;
	Ydeg = degree;
	dbLoadObject(filename, num); // �������� ������
	dbSetObjectSpeed(num, animSpeed); 
	// ������������� �������� ��������
	dbPositionObject(num, Xpos, Ypos, Zpos);
	dbYRotateObject(num, degree);
	dbLoopObject(num);
}

Robot::~Robot()//����������: 
//�������� ��� ������-�����, ���������� ������
{
if((multiplayer > 1) && (num == myrobot))mnFinish();
//��� ������ ������ ������, ����������� �� �������
	dbDeleteObject(num);
	num = 0;
	lifes = 0;
	alive = false;
	collide = false;
	burn = false;
	evil = false;
	start_time = 0;
	Xpos = 0;
	Ypos = 0;
	Zpos = 0;
	Ydeg = 0;
}

void Robot::Draw(bool user, bool hit, bool colgam, 
				 int terrain, float degree)
//������, ������ ������� �� �������������� �����
{ //��� ������ ������������� ������, ��� �������� 
	//���� �������

	const float enemySpeed = 0.3;
	//�������� ����������� ������-�����
	const float factor = 1.5;
	//������ �� �������� � ��������� ������������

	if (alive) {// ���� ����� ���
		dbShowObject(num);

	if ((!user) && (multiplayer == 1))
		AI_Moving(enemySpeed); 
	// !user - ��������� �����

	if (!hit) {
		if (user) // user - ���������������� �����
			collide = false;
	} else {
		if (user) 
			collide = true;
		if (!user)
		{
			Ydeg += degree;
			dbYRotateObject(num, Ydeg);	
		}
		if ((colgam) && (!user)) {  
//���� colgam, ������ � ������������ 
//��������� "���" �����
dbMoveObject(num, enemySpeed * factor); 
}else
if ((!colgam) && (!user)) {	
dbMoveObject(num, enemySpeed * (factor*3));
}
if (user) dbMoveObject(num, -enemySpeed * factor);
}

Xpos = dbObjectPositionX(num);
Zpos = dbObjectPositionZ(num);

float ground=dbGetTerrainGroundHeight(terrain,Xpos,
				// ����������, �������� ������  
		Zpos);// ����������� ���������

		Ypos = ground + 1;

		Check_Position();

	//������������� ���������
	dbPositionListener(Xpos, Ypos, Zpos);
	dbRotateListener(0, Ydeg, 0);

	dbPositionObject(num, Xpos,// ���������� ������
		Ypos, Zpos);

	} else dbHideObject(num);

	if (user)
	{   //�������� C-������
		char life[] = "Life: ";
		char l[2];
	itoa(lifes, l, 10);//����������� ����� � ������
		strcat(life, l);//������������� ������
dbText(ScreenWidth - 100 - 20*num, 
ScreenHeight - 50 - 20*num, life);
//������� �������������� ������
	}
}

void Robot::AI_Moving(const float speed)
{
	const float pi = 3.14159265359;
	//����� ��: �������� ����� �� MSDN
	const int mult = 5;//���������

	if (!evil) {//���� ����� �� "����", ��
	//������� �������� �������� - ��������������
if ((num % 2) == 0)//������� � ������� �������� 
//���������� � ���� �������, � ��������� � ������
	Ydeg += 360 / (dbObjectSizeZ(terrain) * mult);
//������� ������������ ������ ��� ��������
else 
Ydeg += 360 / (-dbObjectSizeZ(terrain) * mult);
dbYRotateObject(num, Ydeg);
//�������������� ������� ������
dbMoveObject(num, speed);
//�������������� �������� ������
} else {
//�����, ���� ����� "����", �� 
//������� ������� �������� ���������
float x, z;
x = dbObjectPositionX(myrobot) - 
dbObjectPositionX(num);
//��������� ���������� �� ����� ��
z = dbObjectPositionZ(myrobot) - 
dbObjectPositionZ(num);
//�������� ��������� �� ���� x � z
if (z < 0)
Ydeg = dbATANFULL(x, z) + pi / 2;
//��������� ���� ����������� ��������, �������� �
else                                 
Ydeg = dbATANFULL(x, z) - pi / 2;
//�������, ���� ��������� ���� - �������� ���������
dbYRotateObject(num, Ydeg);//�������� ������, 
//������������ ����� � ����������������� ������
if ((dbABS(x) > 0.02) && (dbABS(z) > 0.02)) {
//���� ������� ���������� �� ����� �� ����. ������.
Xpos = Xpos + x / 100;
// �� ���������� ����� � �������� ���������
Zpos = Zpos + z / 100;// �� ���� x � z
}//������, ���������� ���������� � �������������� 
//������� ���������� Dark GDK, ��������� ��� �����
//��������������
dbPositionObject(num, Xpos, Ypos, Zpos);
//���������������� ���������� ������ �� ����� 
//����������� ����������� (��. ����)
}
}

void Robot::Die(bool strikenbyMR)//������ ������
{
	alive = false;
	PlaySound();//�������� "��������" �������
	start_time = GetTickCount();
	if ((multiplayer > 1) && (num != myrobot)) 
	{
		if (strikenbyMR)
		wincount++;//���� ����� �� ����.���., 
	//� ������� �������� �������� ��, �� +1 ������
	}
}

bool stop = false;// ��������� ����������,
//� ������� ����������

void Robot::Control_Position(int terrain)
{
	const float robotSpeed = 0.5;
	//�������� ����������� ������

	Xpos = dbObjectPositionX(myrobot);
	Zpos = dbObjectPositionZ(myrobot);

float ground = dbGetTerrainGroundHeight(terrain,
		// ����������, �������� ������  
		Xpos, Zpos);// ����������� ���������

	Ypos = ground + 1; 

	bool rot = false; // ��� �� �������

if (dbLeftKey() || dbKeyState(30)) {
	// �� ������ �� ������� ����� ��� 'A'
Ydeg -= 2;
dbYRotateObject(myrobot, Ydeg);//������� ������
dbMoveCamera(-4);// ����������� ������
rot = true;// ������� ���
dbSetCameraToFollow(Xpos, // ��� ������� ����������
//������ �����, ����� ������ ����� �� ������
//���������� ��������
Ypos, Zpos,  // ����������� ������ � ������
5, 5, 5, 4, 1);
	}

if (dbRightKey() || dbKeyState(32)) {
// �� ������ �� ������� ������ ��� 'D'
Ydeg += 2;
dbYRotateObject(myrobot, Ydeg);//������� ������												
dbMoveCamera(-4);// ����������� ������
rot = true; // ������� ���
dbSetCameraToFollow(Xpos,
Ypos, Zpos,  // ����������� ������ � ������
5, 5, 5, 4, 1);
}

if (dbUpKey() || dbKeyState(17))
// �� ������ �� ������� ����� ��� 'W'
{
if (!stop) 
{
dbLoopObject(myrobot);// �������� �������� ������
stop = true;
}

if (!collide) dbMoveObject(myrobot, robotSpeed);
// ���� �� ���� ������������, �� ���������� 
//���������� ������
if (!rot) {// ���� ��� �������, ��...
dbMoveCamera(-4);// ����������� ������
dbSetCameraToFollow(Xpos,
Ypos, Zpos,  // ����������� ������ � ������
5, 5, 5, 4, 1);
}
} else
if (dbDownKey() || dbKeyState(31))
// �� ������ �� ������� ���� ��� 'S'
{
if (!stop) {
dbLoopObject(myrobot);// �������� �������� ������
stop = true;
}
if (!collide) dbMoveObject(myrobot, -robotSpeed);
//���� �� ���� ������������, �� ���������� 
//���������� ������
if (!rot) {// ���� ��� �������, ��...
dbMoveCamera(-4);// ����������� ������
dbSetCameraToFollow(Xpos,
Ypos, Zpos,  // ����������� ������ � ������
5, 5, 5, 4, 1);
}
} else {
dbStopObject(myrobot);// ��������� �������� ������
stop = false;
}
}

void Robot::Life_Decrease(bool strikenbyMR)
//���������� ���-�� ������ ������
{
	lifes--;
	if (lifes == 0) Die(strikenbyMR);
	else evil = !evil;//������ ����� ������
	if (num != myrobot)
		Draw(false, true, false, terrain, 180);
}

void Robot::Check_Position()
//�������, ����������� ������� ������� ������
{
bool b = false;

if (Zpos > dbObjectSizeZ(terrain)-1)
//������� ���������� ������ �������, 
{//� ������ ������ ���������, �� ��� Z 
Zpos = dbObjectSizeZ(terrain)-1;
b = true;                     
}

	if (Zpos < 1)
	{
		Zpos = 1;
		b = true;
	}

	if (Xpos > dbObjectSizeX(terrain)-1)
	//������� ���������� ������ ������� �� ��� X
	{
		Xpos = dbObjectSizeX(terrain)-1;
		b = true;
	}

	if (Xpos < 1)
	{
		Xpos = 1;
		b = true;
	}

	if ((b) && (num != myrobot))
		Draw(false, true, false, terrain, 180);
}

void Robot::PlaySound()//����������� ����
{
dbPositionSound(RobotExplode, Xpos, Ypos, Zpos);
//���������� � �������� ����������
dbPlaySound(RobotExplode);
//����������, ��������� ������
}

void Robot::SetAlive(bool live)
//�������� ������ :)
{
	alive = live;
}

void Robot::SetNumber(int i)
//���������� ����� �������
{ 
	cnumber = i; 
}

int Robot::GetNumber()
//���������� ����� �������
{ 
	return cnumber;
}

void Robot::SetLifeCount(int life)
//���������� ���-�� ������
{
	lifes = life;
}

int Robot::GetLifeCount()
//������ ������� ������ ��������
{
	return lifes;
}

int Robot::GetWinCount()//������� ���-�� �����
{
	return wincount;
}

void Robot::ZeroWinCount()//�������� ������� �����
{
	wincount = 0;
}

void Robot::SetMultiplayerMode(int mode)
{
	multiplayer = mode;
}

int Robot::GetMultiplayerMode()
{
	return multiplayer;
}