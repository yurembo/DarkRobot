#include "Rocket.h"

Rocket::Rocket()
//нужна пуста€ реализаци€ конструктора
{}                //чтобы линковщик не запуталс€

Rocket::Rocket(int i, char *filename)
{
	num = i;
	alive = false;//ракета деактивирована
	burn = false;
	Xpos = 0;
	Ypos = 0;
	Zpos = 0;
	Ydeg = 0;
	start_time = GetTickCount();
	if (num == myrocket)//перва€ ракета
	{
dbLoadObject(filename,num);//загружаем еЄ из файла,
	}
	else //так как все ракеты одинаковы, то...
	{
		dbCloneObject(num, myrocket);
		//...остальные копируем с первой
	}
		dbHideObject(num);//скрываем
	dbPositionObject(num, Xpos, Ypos, Zpos);
	dbYRotateObject(num, Ydeg);
}

Rocket::~Rocket()//деструктор: 
//обнул€ем все данные-члены, уничтожаем объект
{
	dbDeleteObject(num);
	num = 0;
	alive = false;//ракета деактивирована
	burn = false;
	Xpos = 0;
	Ypos = 0;
	Zpos = 0;
	Ydeg = 0;
	start_time = 0;
}

void Rocket::Draw(int terrain)//манипул€ции над
//ракетой: вычисление и установка местаположени€
//и т.д.
{
const float rocketSpeed = 1.1;//скрость перемещени€
//ракеты
const int RockToLive = 3000;

if (alive)  { //если ракета активизирована, 
//то показываем еЄ и 
dbShowObject(num);// производим манипул€ции над ней
dbMoveObject(num, rocketSpeed);

Xpos = dbObjectPositionX(num);
Ypos = dbObjectPositionY(num);
Zpos = dbObjectPositionZ(num);

float ground = dbGetTerrainGroundHeight(terrain, 
		Xpos,// переменна€, хран€ща€ высоту  
		Zpos);// поверхности ландшафта
if (ground >= Ypos) {
	// произошло столкновение с поверхностью
		Die();
	}

	dbPositionObject(num, Xpos,// перемещаем ракету
		Ypos, Zpos);

if ((GetTickCount() - start_time) > RockToLive) 
Die();
} else dbHideObject(num);//иначе скрываем еЄ
}

void Rocket::Fire(float x, float y, float z, 
	float deg)//выстрел - начало жизни ракеты
{
if (!alive) {
alive = true;
PlaySound(RobotShort, x, y, z);
//вызываем "звуковую" функцию
start_time = GetTickCount();//отмечаем врем€ вылета
// отдельна€ функци€ создана со взгл€дом на будущее
// вдруг потом понадобитс€ сделать во врем€ выстрела 
// что-то ещЄ кроме позиционаровани€
Pos(x, y, z, deg);
}
}

void Rocket::Pos(float x, float y, float z, 
float deg)//фунци€ позиционировани€ объекта-ракеты
{
	Xpos = x;
	Ypos = y;
	Zpos = z;
	Ydeg = deg;

dbPositionObject(num, Xpos, Ypos, Zpos);
//позиционирование в мировых координатах

dbMoveObject(num, 2);//относительное смещение -
dbMoveObjectRight(num, 2);//позиционирование
dbMoveObjectUp(num, 2.5);//в локальных координатах

dbYRotateObject(num, Ydeg);
}

void Rocket::Die()//конец текущей жизни ракеты
{
	burn = true;//горение
	PlaySound(RocketExplode, Xpos, Ypos, Zpos);
	//вызываем "звуковую" функцию
	alive = false;
}

void Rocket::PlaySound(int sound, float x, float y,
float z)//проигрывает звук, переданный в качестве 
//первого параметра, и перемещает
{// его в позицию так же заданную параметрами
dbPositionSound(sound, x, y, z);//перемещение
dbPlaySound(sound);//собственно, запускает музыку
}