#include "Robot.h"
#include <String.h>//заголовочный файл, содержащий
//функции для работы со стандартными С-строками

//кол-во побед, переменная общая для всех объектов 
//класса
int wincount = 0;//в ней суммируются поражения 
//врагов

Robot::Robot()//нужна пустая реализация конструктора
{}              //чтобы линковщик не запутался

Robot::Robot(int i, int life, float x, float y, 
			 float z, float degree, char* filename,
			 int multigame)
{
	const float animSpeed = 480;//скорость анимации

	num = i;
	lifes = life;
	cnumber = 0;
   multiplayer = multigame;//устанавливаем тип игры
	if (multiplayer == 1) alive = true; 
	else {
		if (multiplayer == 2) {
	if (num == myrobot) alive = true;//глав.пера 
	//делаем живым (и активным), 
			else alive = false;//остальных пока нет
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
	dbLoadObject(filename, num); // загрузка робота
	dbSetObjectSpeed(num, animSpeed); 
	// устанавливаем скорость анимации
	dbPositionObject(num, Xpos, Ypos, Zpos);
	dbYRotateObject(num, degree);
	dbLoopObject(num);
}

Robot::~Robot()//деструктор: 
//обнуляем все данные-члены, уничтожаем объект
{
if((multiplayer > 1) && (num == myrobot))mnFinish();
//при гибели нашего робота, отключаемся от сервера
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
//замечу, данная функция не перерисовывает экран
{ //она только позиционирует робота, для которого 
	//была вызвана

	const float enemySpeed = 0.3;
	//скорость перемещения робота-врага
	const float factor = 1.5;
	//влияет на быстроту и дальность отталкивания

	if (alive) {// если робот жив
		dbShowObject(num);

	if ((!user) && (multiplayer == 1))
		AI_Moving(enemySpeed); 
	// !user - вражеский робот

	if (!hit) {
		if (user) // user - пользовательский робот
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
//если colgam, значит в столкновении 
//участвует "наш" робот
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
				// переменная, хранящая высоту  
		Zpos);// поверхности ландшафта

		Ypos = ground + 1;

		Check_Position();

	//позиционируем слушателя
	dbPositionListener(Xpos, Ypos, Zpos);
	dbRotateListener(0, Ydeg, 0);

	dbPositionObject(num, Xpos,// перемещаем робота
		Ypos, Zpos);

	} else dbHideObject(num);

	if (user)
	{   //типичные C-строки
		char life[] = "Life: ";
		char l[2];
	itoa(lifes, l, 10);//преобразуем число в строку
		strcat(life, l);//конкатенируем строки
dbText(ScreenWidth - 100 - 20*num, 
ScreenHeight - 50 - 20*num, life);
//выводим результирующую строку
	}
}

void Robot::AI_Moving(const float speed)
{
	const float pi = 3.14159265359;
	//число пи: значение взято из MSDN
	const int mult = 5;//множитель

	if (!evil) {//если робот не "злой", то
	//обычный сценарий движения - патрулирование
if ((num % 2) == 0)//роботов с чётными номерами 
//отправляем в одну сторону, с нечётными в другую
	Ydeg += 360 / (dbObjectSizeZ(terrain) * mult);
//немного поворачиваем робота при движении
else 
Ydeg += 360 / (-dbObjectSizeZ(terrain) * mult);
dbYRotateObject(num, Ydeg);
//действительный поворот робота
dbMoveObject(num, speed);
//действительное смещение робота
} else {
//иначе, если робот "злой", то 
//активно атакует главного персонажа
float x, z;
x = dbObjectPositionX(myrobot) - 
dbObjectPositionX(num);
//вычисляем расстояние от врага до
z = dbObjectPositionZ(myrobot) - 
dbObjectPositionZ(num);
//главного персонажа по осям x и z
if (z < 0)
Ydeg = dbATANFULL(x, z) + pi / 2;
//вычисляем угол направления движения, двигаясь в
else                                 
Ydeg = dbATANFULL(x, z) - pi / 2;
//котором, враг достигнет цели - главного персонажа
dbYRotateObject(num, Ydeg);//попросту говоря, 
//поворачиваем врага к пользовательскому роботу
if ((dbABS(x) > 0.02) && (dbABS(z) > 0.02)) {
//если имеется расстояние от врага до глав. персон.
Xpos = Xpos + x / 100;
// то перемещаем врага к главному персонажу
Zpos = Zpos + z / 100;// по осям x и z
}//заметь, вычисления проводятся с использованием 
//функций библиотеки Dark GDK, поскольку они лучше
//оптимизированы
dbPositionObject(num, Xpos, Ypos, Zpos);
//позиционирование вражеского робота по ранее 
//вычисленным координатам (см. выше)
}
}

void Robot::Die(bool strikenbyMR)//гибель робота
{
	alive = false;
	PlaySound();//вызываем "звуковую" функцию
	start_time = GetTickCount();
	if ((multiplayer > 1) && (num != myrobot)) 
	{
		if (strikenbyMR)
		wincount++;//если погиб не глав.пер., 
	//а автором выстрела является он, то +1 победа
	}
}

bool stop = false;// объявляем переменную,
//в будущем пригодится

void Robot::Control_Position(int terrain)
{
	const float robotSpeed = 0.5;
	//скорость перемещения робота

	Xpos = dbObjectPositionX(myrobot);
	Zpos = dbObjectPositionZ(myrobot);

float ground = dbGetTerrainGroundHeight(terrain,
		// переменная, хранящая высоту  
		Xpos, Zpos);// поверхности ландшафта

	Ypos = ground + 1; 

	bool rot = false; // был ли поворот

if (dbLeftKey() || dbKeyState(30)) {
	// не нажата ли клавиша ВЛЕВО или 'A'
Ydeg -= 2;
dbYRotateObject(myrobot, Ydeg);//поворот робота
dbMoveCamera(-4);// перемещение камеры
rot = true;// поворот был
dbSetCameraToFollow(Xpos, // эта функция вызывается
//только тогда, когда нажата любая из клавиш
//управления курсором
Ypos, Zpos,  // привязываем камеру к роботу
5, 5, 5, 4, 1);
	}

if (dbRightKey() || dbKeyState(32)) {
// не нажата ли клавиша ВПРАВО или 'D'
Ydeg += 2;
dbYRotateObject(myrobot, Ydeg);//поворот робота												
dbMoveCamera(-4);// перемещение камеры
rot = true; // поворот был
dbSetCameraToFollow(Xpos,
Ypos, Zpos,  // привязываем камеру к роботу
5, 5, 5, 4, 1);
}

if (dbUpKey() || dbKeyState(17))
// не нажата ли клавиша ВВЕРХ или 'W'
{
if (!stop) 
{
dbLoopObject(myrobot);// включаем анимацию робота
stop = true;
}

if (!collide) dbMoveObject(myrobot, robotSpeed);
// если не было столкновения, то перемещаем 
//координаты робота
if (!rot) {// если был поворот, то...
dbMoveCamera(-4);// перемещение камеры
dbSetCameraToFollow(Xpos,
Ypos, Zpos,  // привязываем камеру к роботу
5, 5, 5, 4, 1);
}
} else
if (dbDownKey() || dbKeyState(31))
// не нажата ли клавиша ВНИЗ или 'S'
{
if (!stop) {
dbLoopObject(myrobot);// включаем анимацию робота
stop = true;
}
if (!collide) dbMoveObject(myrobot, -robotSpeed);
//если не было столкновения, то перемещаем 
//координаты робота
if (!rot) {// если был поворот, то...
dbMoveCamera(-4);// перемещение камеры
dbSetCameraToFollow(Xpos,
Ypos, Zpos,  // привязываем камеру к роботу
5, 5, 5, 4, 1);
}
} else {
dbStopObject(myrobot);// выключаем анимацию робота
stop = false;
}
}

void Robot::Life_Decrease(bool strikenbyMR)
//уменьшение кол-ва жизней робота
{
	lifes--;
	if (lifes == 0) Die(strikenbyMR);
	else evil = !evil;//меняем режим робота
	if (num != myrobot)
		Draw(false, true, false, terrain, 180);
}

void Robot::Check_Position()
//функция, проверяющая текущую позицию робота
{
bool b = false;

if (Zpos > dbObjectSizeZ(terrain)-1)
//функция возвращает размер объекта, 
{//в данном случае ландшафта, по оси Z 
Zpos = dbObjectSizeZ(terrain)-1;
b = true;                     
}

	if (Zpos < 1)
	{
		Zpos = 1;
		b = true;
	}

	if (Xpos > dbObjectSizeX(terrain)-1)
	//функция возвращает размер объекта по оси X
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

void Robot::PlaySound()//проигрывает звук
{
dbPositionSound(RobotExplode, Xpos, Ypos, Zpos);
//перемещает в заданную координату
dbPlaySound(RobotExplode);
//собственно, запускает музыку
}

void Robot::SetAlive(bool live)
//оживляем робота :)
{
	alive = live;
}

void Robot::SetNumber(int i)
//установить номер клиента
{ 
	cnumber = i; 
}

int Robot::GetNumber()
//возвратить номер клиента
{ 
	return cnumber;
}

void Robot::SetLifeCount(int life)
//установить кол-во жизней
{
	lifes = life;
}

int Robot::GetLifeCount()
//узнать сколько жизней осталось
{
	return lifes;
}

int Robot::GetWinCount()//вернуть кол-во побед
{
	return wincount;
}

void Robot::ZeroWinCount()//обнуляем счётчик побед
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