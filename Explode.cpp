#include "Explode.h"

Explode::Explode()//нужна пуста€ реализаци€ 
				  //конструктора
{}                //чтобы линковщик не запуталс€

Explode::Explode(int i)//конструктор с параметром
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
	10, 15, 10);//создаЄм испускатель частиц в виде
	//огн€, и в параметрах конструктора передаЄм 
	//ему желаемые значени€

	dbHideParticles(num);// скрываем испускатель 
	//вместе с частицами

	dbPositionParticles(num, Xpos, Ypos, Zpos);
	//позиционируем испускатель вместе с частицами
}

Explode::~Explode()//деструктор: 
//обнул€ем все данные-члены, уничтожаем объект
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

void Explode::Draw()//манипул€ции с взрывами, 
//подготовка к выводу на экран
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

void Explode::Die()//исчезновение взрыва
{
	alive = false;
}

void Explode::Fire(float x, float y, float z)
//по€вление взрыва
{
	if (!alive) {
		alive = true;
	start_time = GetTickCount();//отмечаем врем€ 
	Pos(x, y, z);
	}
}


void Explode::Pos(float x, float y, float z)
//позиционирование взрыва
{
	Xpos = x;
	Ypos = y;
	Zpos = z;
	dbPositionParticles(num, Xpos, Ypos, Zpos);
	//позиционирование в мировых координатах
}