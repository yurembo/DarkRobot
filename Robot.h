#include "Game_Obj.h"

class Robot : public GameObj// ������������
{
private:
	int lifes, // �����
		cnumber;// ����� �������
		bool collide,//� ���� ���������� �������� 
			//��������� ������� �� ���� ������������
		 evil;//�����, � ������� ��������� �����: 
		//�������� ��� ���������

// ������������� ������������� ����
int multiplayer;

public:
	Robot();// ����������� �� ���������
	Robot(int i, int life, float x, float y, 
	float z, float degree, char* filename, 
	int multigame);//����������� � �����������
	void Draw(bool user, bool hit, bool colgam,
	int terrain, float degree);//�������������� 
	//����������� ������� ������-������
	void Control_Position(int terrain);	//�������, 
//����������� ��������� ����������������� ������                                           
	void Life_Decrease(bool strikenbyMR);//� 
//��������� ��������� ������������� ������ ��������:
//���� ��� ����.���., �� ����� - ������ � + 1 ������
	DWORD GetTime() { return start_time; } 
	// �������� ����� ������ �������
	void Die(bool strikenbyMR);//������ ������
	void SetAlive(bool live);//�������� ������
	void Check_Position();//�������, �����������
	//������� ������� ������
	void AI_Moving(const float speed);//�������, 
	//����������� ��������� ��������� �������
	void PlaySound();//�������� ����� �� ��� � 
	//������ ������, ������� ����������� ����
	void SetNumber(int i);//���������� ����� �������
	int GetNumber();//���������� ����� �������
	void SetLifeCount(int life);
	//���������� ���-�� ������
	int GetLifeCount();
	//������ ������� ������ ��������
	int GetWinCount();//������� ���-�� �����
	void ZeroWinCount();//�������� ������� �����
	void SetMultiplayerMode(int mode);
	//����������� �������������� ����� ����
	int GetMultiplayerMode();
	//������ ������� ����� ����
	~Robot();//����������
};