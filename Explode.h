#include "Game_Obj.h"

class Explode : public GameObj//������������
{
public:
	Explode();// ����������� �� ���������
	Explode(int i);//����������� � ����������
	void Draw();//����������� � ��������, 
	//���������� � ������ �� �����
	void Die();//������������ ������
	void Fire(float x, float y, float z);
	//��������� ������
	void Pos(float x, float y, float z);
	//���������������� ������
	~Explode();//����������
};