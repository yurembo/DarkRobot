#include "Game_Obj.h"//��� ������������ ����, 
//���������� ���������� � ����������� �����

class Rocket : public GameObj// ������������
{
public:
	Rocket();// ����������� �� ���������
	Rocket(int i, char* filename);//�����������
	//� �����������
	void Draw(int terrain);
	void Die();
	void Fire(float x, float y, float z, float deg);
	//������� - ������ ����� ������
	void Pos(float x, float y, float z, float deg);
	//������ ���������������� �������-������
    void PlaySound(int sound, float x, float y,
	float z);//�������������� ����������� �������,
	         //������� ����������� ����
	~Rocket();//����������
};