#include "Game_Obj.h"//наш заголовочный файл, 
//содержащий объ€влени€ и абстрактный класс

class Rocket : public GameObj// наследование
{
public:
	Rocket();// конструктор по умолчанию
	Rocket(int i, char* filename);//конструктор
	//с параметрами
	void Draw(int terrain);
	void Die();
	void Fire(float x, float y, float z, float deg);
	//выстрел - начало жизни ракеты
	void Pos(float x, float y, float z, float deg);
	//фунци€ позиционировани€ объекта-ракеты
    void PlaySound(int sound, float x, float y,
	float z);//переопредел€ем виртуальную функцию,
	         //котора€ проигрывает звук
	~Rocket();//деструктор
};