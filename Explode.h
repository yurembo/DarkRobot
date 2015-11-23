#include "Game_Obj.h"

class Explode : public GameObj//наследование
{
public:
	Explode();// конструктор по умолчанию
	Explode(int i);//конструктор с параметром
	void Draw();//манипуляции с взрывами, 
	//подготовка к выводу на экран
	void Die();//исчезновение взрыва
	void Fire(float x, float y, float z);
	//появление взрыва
	void Pos(float x, float y, float z);
	//позиционирование взрыва
	~Explode();//деструктор
};