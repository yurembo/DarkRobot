#include "Rocket.h"
#include "Explode.h"
#include "Robot.h"

// пути
char* robotPath = "robot\\robot.x";
//я не объявил их константами
char* enrobotPath = "enemyrobot\\enemyrobot.x";
//потому что
char* rocketPath = "Rocket\\rocket.x";
//в функцию должны передаваться не константные объекты
char* GameSoundFile = "sound\\Engines.wav";
char* LevelCompleteFile = "sound\\LevelComplete.wav";
char* GameOverFile = "sound\\GAMEOVER.wav";
char* RobotShortFile = "sound\\Short.wav";
char* RocketExplodeFile = "sound\\RocketExplode.wav";
char* RobotExplodeFile = "sound\\RobotExplode.wav";

bool first = true;//начальная загрузка
bool play = true;//режим игры
int gamemod = 0;//режим игры:
                //0 - режим не установлен
                //1 - синглплеер
                //2 - мультиплеер

// объявленные ниже константы служат для 
//идентификации типа сообщения
// Tcp
const int NewClient = 0;
const int DeadClient = 1;
// Udp
const int PositionClient = 0;
const int Win = 1;// ПОБЕДА!!!
//количество убийств, которое надо набрать для 
//победы в мультиплеере
const int MaxDeath = 10;

void LoadAllSounds()// загружаем все звуки
{
dbLoadSound(GameSoundFile, GameSound);
//загружаем звук обычным образом
dbSetSoundVolume(GameSound, 95);
//настраиваем громкость
dbLoadSound(LevelCompleteFile, LevelComplete);
//загружаем звук обычным образом
dbLoadSound(GameOverFile, GameOver);
//загружаем звук обычным образом
dbLoad3DSound(RobotShortFile, RobotShort);
//загружаем звук, чтобы впоследствии использовать
//его в качестве объёмного
dbSetSoundVolume(RobotShort, 85);
//настраиваем громкость
dbLoad3DSound(RocketExplodeFile, RocketExplode);
//загружаем звук, чтобы впоследствии использовать
//его в качестве объёмного
dbSetSoundVolume(RocketExplode, 85);
//настраиваем громкость
dbLoad3DSound(RobotExplodeFile, RobotExplode);
//загружаем звук, чтобы впоследствии использовать
//его в качестве объёмного
}

// в качестве значений переменных будут 
// выступать данные, полученные от сервера
int MaximumClients;
int MaximumOperations;
//имя пользователя
char* userName = NULL;
// строка, в которой будет хранится введённый
//геймером ip сервера 
char* ConnectIP = NULL;

bool makeConnect() // создаём сетевое соединение
{
// номер порта, к которому надо подключиться
unsigned short ConnectPort = 7707;
// время ожидания (в секундах), в течении которого
// программа будет ждать ответа от сервера
int Timeout = 10;

while (ConnectIP == NULL)
{
dbCLS(dbRGB(0, 0, 0));
dbText(10, 10, "Enter the IP that you would like to connect to: ");
dbSetCursor(10, 50);
dbSync();
ConnectIP = dbInput();
int size = dbLen(ConnectIP);
if ((size > 16) || (size < 1)) {// проверка на длину 
dbText(10, 30, "Mistake, try again");
dbText(10, 50, "if you wanna exit to main menu press Escape");
dbSync();
ConnectIP = NULL;
dbWaitKey();
}
if (dbEscapeKey()) {
ConnectIP = NULL;
return false;
}
} 

// попытаться соединиться
int Connect = mnConnect(ConnectIP,ConnectPort,
			ConnectIP,ConnectPort,Timeout, 0);

	// соединение успешно
	if(Connect == 1)
	{
		dbPrint("Connection was successful!");
	// получить кол-во клиентов и udp-операций
		MaximumClients = mnGetMaxClients();
		MaximumOperations = mnGetMaxOperations();
// вывести полученные ip, номера портов (tcp и udp)
		//и udp-режим
		dbPrintC("The local TCP IP is: ");
		dbPrint(mnGetLocalIPTCP());
		dbPrintC("The local TCP port is: ");
		dbPrint(dbStr(mnGetLocalPortTCP()));
		dbPrintC("The local UDP IP is: ");
		dbPrint(mnGetLocalIPUDP());
		dbPrintC("The local UDP port is: ");
		dbPrint(dbStr(mnGetLocalPortUDP()));
		dbPrintC("Press any key to continue");

		dbSync();
		dbWaitKey();
	}
	// соединение завершилось неудачей...
if(Connect == 0)
{
dbPrint("Connection timed out! Check that the server is switched on, press any key");
dbSync();
dbWaitKey();
gamemod = 1;
return false;
}
	// ошибка произошла при подключении
if(Connect == -1)
{
dbPrint("An error occurred whilst trying to connect, press any key");
dbSync();
dbWaitKey();
gamemod = 1;
return false;
}
return true;
}

void GetUserName()//функция запрашивает имя 
//пользователя
{
	while (userName == NULL)
	{
		dbCLS(dbRGB(0, 0, 0));
		dbText(10, 10, "Enter your name: ");
		dbSetCursor(10, 30);
		dbSync();
		userName = dbInput();
		int size = dbLen(userName);
		if ((size > 30) || (size < 1)) {// проверка имени на длину 
dbText(10, 50, "Your name is so long or no symbol, press any key and try again");
dbText(10, 70, "if you wanna exit to main menu press Escape");
			dbSync();
			userName = NULL;
			dbWaitKey();
		}
		if (dbEscapeKey()) {
			userName = NULL;
			DarkGDK();
			return;
		}
	} 
	if (!makeConnect()) DarkGDK();
}


bool SelectGameType()//выбираем тип игры: 
//однопользовательский или многопользовательский
{
	gamemod = 0;
while (gamemod == 0) 
{
dbCLS(dbRGB(0, 0, 0));
dbText(10, 10, "Press '1' to start Singleplayer game");
dbText(10, 30, "Press '2' to start Multiplayer game");
dbText(10, 50, "Press Escape to exit");
dbSync();

	if (dbScanCode() == 2) gamemod = 1; else
		if (dbScanCode() == 3) gamemod = 2;	else
			if (dbEscapeKey())
			{
				play = false;
				return false;
			}

	if (gamemod == 2) GetUserName();
}
return true;
}

bool Init()
{
dbSyncOn();//берём перерисовку экрана под свой 
//контроль

dbSyncRate(30); // устанавливаем частоту 
//перерисовки экрана

dbHideMouse();//скрываем мышь

dbSetDisplayMode(ScreenWidth, ScreenHeight, 
ScreenBitDepth);// устанавливаем параметры экрана
// при тестировании нескольких клиентов на одном 
//комьютере не забудь закомментировать 
//предыдущую строчку
//dbSetDisplayMode(640, 480, 32);// а эту 
//раскомментировать
dbMaximiseWindow();// разворачиваем окно на весь 
					//экран
	                  // и эту тоже закомментируй

	if (!SelectGameType()) return false;

	dbLoadImage("DarkRobot_Loading.jpg",
		DarkRobot_Loading); //загружаем 
	//текстуру для экрана загрузки игры
	dbPasteImage(DarkRobot_Loading,
		0, 0);// выводим её на экран
dbSync();// функция перерисовки вызывается дважды,
dbSync();// иначе текстура не отобразится

	// грузим текстуры для экранов победы и смерти
	dbLoadImage("win_screen.jpg", win_screen);
	dbLoadImage("death_screen.jpg", death_screen);

dbSetCameraRange(1.0f, 30000.0f);// устанавливаем 
								 //ближнюю 
	// и дальнюю плоскости отсечения
	dbLoadImage("texture.jpg", basetex); 
	// загружаем тексуру №1

	dbLoadImage("detail.jpg", dettex); 
	// загружаем тексуру №2

dbSetupTerrain(); // функция, инициализирующая 
//внутренние переменные, 
//должна вызывать перед созданием любого ландшафта
dbMakeObjectTerrain(terrain);// предварительное 
							//создание объекта
	// присвоение имени
	dbSetTerrainHeightMap(terrain, "map.bmp");
	// загрузка карты высот
	dbSetTerrainScale(terrain, 3.0f, 0.6f, 3.0f);
	// масштабирование
	dbSetTerrainLight(terrain,  1.0f, -0.25f, 0.0f,
		1.0f, 1.0f, 0.78f, 0.5f );// освещение
	dbSetTerrainTexture(terrain, basetex, dettex);
	// текстурирование 
	dbBuildTerrain(terrain);
	// окончательная генерация ландшафта

	dbLoadObject("skybox.x", skybox);
	// загрузка небесной оболочки
	dbSetObjectLight(skybox, 0);
	// отключаем освещение
	dbScaleObject(skybox, 10000, 10000, 10000);
	// масштабирование
	dbSetObjectTexture(skybox, 3, 1);
	// текстурирование

	dbLoadImage("fire.jpg", fire);
	// частица огня - загружаем из файла

	// настриваем параметры текста:
	//dbSetTextFont("Verdana");//шрифт
	//dbSetTextSize(24);//размер

	// инициализация звуковой подсистемы
	LoadAllSounds();

	first = false;

	return true;
}

void BSOD(const bool win)//Black Screen Of Death :)
{
	dbStopSound(GameSound);//останавливаем музыку
	bool b = true;
	while (!play) {
		if (!win) {
	dbPasteImage(death_screen, 0, 0);
	//выводим данное изображение на экран
	if (b) {
		dbPlaySound(GameOver);//проигрываем данный
		//звук
		b = false;
	}
		} else {
			dbPasteImage(win_screen, 0, 0);
			//выводим данное изображение на экран
			if (b) {
				dbPlaySound(LevelComplete);
				//проигрываем данный звук
				b = false;
			}
		}
	dbText(50, ScreenHeight - 100,
	"if you wanna exit to main menu press Enter");
	dbSync();
			if (dbReturnKey()) {
				play = true;
				DarkGDK();//начинаем игру заново
				return;
			}
		}
}

// ПАУЗА
bool GamePause()
{
	bool b = false;
	while (true)
	{
		dbCLS(dbRgb(0,0,0));
		dbText(10, 10, 
			"if you wanna continue press Enter");
		dbText(10, 30, 
	"if you wanna exit to main menu press Space");
		if (dbReturnKey()) {
			b = false;
			break;
		}
		if (dbSpaceKey()) {
			b = true;
			break;
		}
		dbSync();
	}
	return b;
}

//структура для хранения имени клиента и кол-во 
//набранных фрагов
struct User 
{
string name;
int frag;
};

User user[count+1];

bool NetGameOver()//выводим итоги
{
	while (true) {
	dbCLS(dbRGB(0, 0, 0));
dbPrint("DarkROBOT - Multiplayer Game - Totals:");
	for (int i = 0; i < count + 1; i++)
	{
	string s = user[i].name;
	if (s == "") continue;
	char* ch = &s[0];//берём адрес 1-го символа,
	//а вместе с ним всю строку :)
    dbPrintC(ch);
	dbPrintC(" : ");
 	dbPrint(dbStr(user[i].frag));
	}
	dbPrint("");
	dbPrint("to exit to main menu press Enter");
	dbSync();

	if (dbReturnKey()) 
	{
		play = true;
		DarkGDK();
		return false;
	}
	}
	return true;
}

void DarkGDK(void) // точка входа - главная функция
{
	dbRandomize(dbTimer());// инициализация 
	//генератора случайных значений

	dbDisableEscapeKey();//отключение возможности 
//завершения работы приложения при нажатии на Escape

	if (first) {//при первой загрузке
	if (!Init()) return; // выполняем инициализацию
	} else 
	{
		if (!SelectGameType()) return;
		dbPasteImage(DarkRobot_Loading, 0, 0);
		// выводим текстуру на экран
		dbSync();//перерисовываем экран
		dbShowObject(skybox);
		//делаем видимыми скрытые объекты,
		dbShowObject(terrain);
		//заметь, пока мы их не отображаем
	}     //до следующей перерисовки экрана   

// все объекты создадим до начала игрового процесса
	// заметь, как задаются координаты
	float xx = dbRnd(dbObjectSizeX(terrain)-1);
	float zz = dbRnd(dbObjectSizeZ(terrain)-1);
float yy = dbGetTerrainGroundHeight(terrain, 
									xx, zz);
	Robot MyRobot(myrobot, 10, xx, yy,
		zz, 0, robotPath, gamemod);
	//создаём пользовательского робота

	int lifecount;//количество жизней
	(gamemod > 1) ? lifecount = 10 : lifecount = 5;
	// при многопользовательской игре координаты 
	//врагов по большому счёту значения не играют,
	// так как их значения мы всё равно получим от
	//соответствующего клиента
	Robot enemyRobot[count] = // создаём объекты: 
		//4-х роботов-врагов
	{
		Robot(rob_num[0], lifecount, 100, 23, 700,
		0, enrobotPath, gamemod), //в конструктор в
		//качестве последнего параметра 		
		Robot(rob_num[1], lifecount, 300, 23, 500,
		180, enrobotPath, gamemod),//передаётся имя 
		//зугружаемого файла-объекта-робота
		Robot(rob_num[2], lifecount, 500, 23, 300, 
		180, enrobotPath, gamemod),//в нашем случае
		//все враги одинаковы
		Robot(rob_num[3], lifecount, 700, 23, 100,
		0, enrobotPath, gamemod)//но ничего не 
		//мешает насоздавать разных роботов
	};    //и использовать разные модели

	Rocket MyRocket(myrocket, rocketPath);
	//создаём пользовательскую ракету

	Rocket enemyRocket[count] = //создаём ещё 4 
	                    	//вражеские ракеты 
	{                     //т.к. все ракеты одинаковы
Rocket(rock_num[0], ""),//образы вражеских ракет 
Rocket(rock_num[1], ""),//будут скопированы с образа
Rocket(rock_num[2], ""),//пользовательской ракеты
Rocket(rock_num[3], "")//в конструкторе класса
	};

	Explode explode[count+1] = //создаём все взрывы
	{
		Explode(exp_num[0]),
		Explode(exp_num[1]),
		Explode(exp_num[2]),
		Explode(exp_num[3]),
		Explode(exp_num[4])
	};

	// начинаем воспроизведение звука
dbLoopSound(GameSound);//воспроизводим и зацикливаем

//dbPositionCamera(424, 27, 89);// позиционируем камеру
dbPositionCamera(200, 100, 100);

DWORD globalTime = 0;// глобальный секундозапоминатель

// выстрел
int fire = 0;

	while (LoopGDK() && (play))// основной цикл
	{    
		// если мультиплеерный режим...
		if (MyRobot.GetMultiplayerMode() > 1)
		{
		// ожидаем сообщения от сервера
		// проверяем tcp-сообщения
		int iTcpPackets = mnRecvTCP(NULL);
		
		// если есть...
		if(iTcpPackets > 0)
		{
// получить номер-тип операции из пришедшего 
//сообщения
// а так же номер клиента, с которым она
//произошла
			int Operation = mnGetInt();
			int Client = mnGetInt();

// в сообщении говорится, что подключился новый
//клиент
if(Operation == NewClient)
{				
for (int i = 0; i < count; i++)
if (!enemyRobot[i].GetAlive()) 
{
enemyRobot[i].SetAlive(true);
enemyRobot[i].SetNumber(Client);
break;
}
}

// клиент отключился
if (Operation == DeadClient)
{
for (int i = 0; i <= count; i++)
if ((enemyRobot[i].GetAlive()) &&
(Client == enemyRobot[i].GetNumber()))
{
enemyRobot[i].Die(false);
break;
}
}
}
	
// проверяем на наличие нового-пришедшего
		//udp-сообщения
// в котором содержится расположение клиента
		// проверяем каждого клиента
for(int Client = 1;Client<=MaximumClients;Client++)
{
// проверяем все операции текущего клиента
for(int Operation = 0;Operation<MaximumOperations;
	                                   Operation++)
{
// собственно, проверка на наличие udp-сообщения
int UdpPackets = mnRecvUDP(Client,Operation);
// если оно имеет место.....
if ((Operation == PositionClient) && (UdpPackets == 1))
{
for (int i = 0; i <= count; i++)
if ((enemyRobot[i].GetAlive()) &&
(Client == enemyRobot[i].GetNumber()))
{
// получим из него (сообщения) расположение и угол 
	//поворота клиента
		// и применим к его образу
	float PosX = mnGetFloat();
	float PosY = mnGetFloat();
	float PosZ = mnGetFloat();
	float RotY = mnGetFloat();
	int f = mnGetInt();
	int lifecount = mnGetInt();
	
dbPositionObject(enemyRobot[i].GetNum(),
				 PosX,PosY,PosZ);
dbRotateObject(enemyRobot[i].GetNum(),0,RotY,0);
enemyRobot[i].SetLifeCount(lifecount);

if ((f == 1) && (!enemyRocket[i].GetAlive()) &&
	(enemyRobot[i].GetAlive()))
	enemyRocket[i].Fire(PosX, PosY, PosZ, RotY);

break;
}
} 
// кто-то набрал нужное кол-во фрагов - 
//конец сетевой игры!
if ((Operation == Win) && (UdpPackets == 1))
{
for (int i = 0; i < count + 1; i++)
{
string s = mnGetString();
if (s == "") continue;
user[i].name = s;
user[i].frag = mnGetInt();
}

play = false;
int i;
MyRobot.~Robot();//явно вызываем декструкторы -
//уничтожаем объекты
for (i = 0; i < count; i++) enemyRobot[i].~Robot();

				MyRocket.~Rocket();

for (i = 0; i < count; i++) 
enemyRocket[i].~Rocket();

for (i = 0; i < count + 1; i++)
explode[i].~Explode();

dbHideObject(terrain);
dbHideObject(skybox);

if (!NetGameOver()) return;//выводим итоги
}
}
}
		
// отправим координаты и угол поворота нашего 
//робота на сервер через udp
		// тем самым сформулируем пакет
		mnAddInt(PositionClient);
		mnAddFloat(MyRobot.GetXpos());
		mnAddFloat(MyRobot.GetYpos());
		mnAddFloat(MyRobot.GetZpos());
		mnAddFloat(MyRobot.GetDegree());	
		mnAddInt(fire);
		mnAddInt(MyRobot.GetLifeCount());
//следующие данные предназначены серверу для 
//проведения статистики, и обратно, клиентам 
//они не высылаются
mnAddString(userName);
int p = MyRobot.GetWinCount();
mnAddInt(p);
if (p >= MaxDeath) MyRobot.ZeroWinCount()
;// чтобы был только один "сигнал" о победе
		fire = 0;
		// собственно, отправка пакета
		mnSendUDP(NULL,false,true);
} // - дальше общий для всех режимов код

int win = 0;//счётчик погибших врагов
for (int i = 0; i < count; i++)//проверяем:
//если робот погиб,
if (!enemyRobot[i].GetAlive()) {
	//а его ракета запущена,
win++;
if (enemyRocket[i].GetAlive()) 
enemyRocket[i].Die();//то уничтожаем её
}
if ((MyRobot.GetMultiplayerMode() == 1) && 
	(MyRobot.GetAlive()) &&
	(play) && (win == count) && ((GetTickCount() - 
	globalTime) > 3000))//проверка на победу
	{
			play = false;
			int i;
			MyRobot.~Robot();
			//явно вызываем деструкторы - уничтожаем объекты
			for (i = 0; i < count; i++) 
				enemyRobot[i].~Robot();

			MyRocket.~Rocket();

			for (i = 0; i < count; i++) 
				enemyRocket[i].~Rocket();

			for (i = 0; i < count + 1; i++) 
				explode[i].~Explode();

			dbHideObject(terrain);
			dbHideObject(skybox);
			BSOD(true);// функция завершения игры
		}

if (MyRobot.GetAlive()) 
MyRobot.Control_Position(terrain);
//функция, которая считывает нажатые клавиши 
//и производит необходимые повороты, перемещения 
//(в общем, необходимые модификации)
//пользовательского робота

if ((dbSpaceKey()) && (MyRobot.GetAlive()))
// наш выстрел
{
MyRocket.Fire(MyRobot.GetXpos(), MyRobot.GetYpos(), 
MyRobot.GetZpos(), MyRobot.GetDegree());
fire = 1;// идентификация выстрела
}

dbSetCameraToObjectOrientation(MyRobot.GetNum());
//направляем камеру, соответственно ориентации робота

dbXRotateCamera(25);// поворачиваем камеру по оси Х

bool hit = false, hit2 = false;//хранят значение 
//о том, было ли столкновение
float deg = 0;

for (int i = 0; i < count; i++)
{	
int j = 0;
hit = false;
hit2 = false;
deg = 0;
if ((MyRobot.GetAlive()) && (enemyRobot[i].GetAlive()))
//если все живы, то проверяем было ли столкновение
if (dbObjectCollision(MyRobot.GetNum(), 
	enemyRobot[i].GetNum()) == 1)
	//1-произошло столкновение между 
	{     //нашим и вражеским роботами
		hit = true;//столкновение произошло
	}

	deg = 0;
if ((hit) && (MyRobot.GetAlive())) 
//было ли столкновение и жив ли наш робот
{
deg = -MyRobot.GetDegree();
MyRobot.Draw(true, hit, true, terrain, 0);
//надо дважды вызвать эту функцию, 
MyRobot.Draw(true, hit, true, terrain, 0);
//чтобы корректно обработать столкновение
}
enemyRobot[i].Draw(false, hit, true, terrain, deg);
//обновляем состояние робота
// выстрел вражеского робота
if (MyRobot.GetMultiplayerMode() == 1) {
if ((!enemyRocket[i].GetAlive()) && 
	(enemyRobot[i].GetAlive()))
enemyRocket[i].Fire(enemyRobot[i].GetXpos(), 
					enemyRobot[i].GetYpos(),
enemyRobot[i].GetZpos(),enemyRobot[i].GetDegree());
}
}

for (int i = 0; i < count; i++) 
{
for (int j = 0; j < count; j++) 
{
deg = 0;
hit2 = false;
if (i != j) 
{
if ((enemyRobot[i].GetAlive()) &&
	(enemyRobot[j].GetAlive()))
//если все живы, то проверяем было ли столкновение
if (dbObjectCollision(enemyRobot[i].GetNum(), 
	enemyRobot[j].GetNum()) == 1)
	//1-произошло столкновение между 
{  //вражескими роботами
	hit2 = true;//столкновение произошло
}
}
deg = 0;
if ((hit2) && (enemyRobot[i].GetAlive()))
//было ли столкновение и жив ли вражеский робот 
{
deg = 90;
enemyRobot[i].Draw(false, hit2, false, terrain, 
				deg);//обновляем состояние робота
enemyRobot[j].Draw(false, hit2, false, terrain,
			deg);//с учётом изменившихся параметров
}
}
}

MyRobot.Draw(true, false, true, terrain, 0);
//обновляем состояние робота

if ((!MyRobot.GetAlive()) && (play) && 
	((GetTickCount() - MyRobot.GetTime()) > 3000))
	//проверка на гибель
{
play = false;
int i;
MyRobot.~Robot();//явно вызываем декструкторы -
//уничтожаем объекты
for (i = 0; i < count; i++) enemyRobot[i].~Robot();

MyRocket.~Rocket();

for (i = 0; i < count; i++) 
enemyRocket[i].~Rocket();

for (i = 0; i < count + 1; i++) 
explode[i].~Explode();

dbHideObject(terrain);
dbHideObject(skybox);
BSOD(false);// функция завершения игры
}

for (int i = 0; i < count; i++) 
if ((MyRocket.GetAlive()) && 
	(enemyRobot[i].GetAlive()))
//если все живы, то проверяем было ли столкновение
if (dbObjectCollision(MyRocket.GetNum(), 
	enemyRobot[i].GetNum()) == 1)
	//произошло столкновение между
{  //вражеским роботом и нашей ракетой
enemyRobot[i].Life_Decrease(myrobot);
//уменьшаем кол-во жизней
globalTime = GetTickCount();
//запоминаем это мгновение :)
MyRocket.Die();//уничтожение ракеты
}

if (MyRobot.GetMultiplayerMode() > 1)
//только в мультиплеере столкновение между
//вражеским роботом 
{//и (другой) вражеской ракетой имеет значение
for (int i = 0; i < count; i++) 
for (int j = 0; j < count; j++)
if (i != j)
if ((enemyRocket[i].GetAlive()) &&
	(enemyRobot[j].GetAlive()))
//если все живы, то проверяем было ли столкновение
if (dbObjectCollision(enemyRocket[i].GetNum(), 
	enemyRobot[j].GetNum()) == 1)
	//произошло столкновение между
{  //вражеским роботом и вражеской ракетой
enemyRobot[j].Life_Decrease(false);
//уменьшаем кол-во жизней
globalTime = GetTickCount();
//запоминаем это мгновение :)
enemyRocket[i].Die();//уничтожение ракеты
}
}
			                                                           
MyRocket.Draw(terrain);
//выводим "нашу" ракету на экран
if (MyRocket.GetBurn())//если состояние горения,...
{
explode[0].Fire(MyRocket.GetXpos(),
MyRocket.GetYpos(), MyRocket.GetZpos());
//...то создаём взрыв
MyRocket.SetBurn(false);
//отключаем состояние горения
}

for (int i = 0; i < count; i++)
{
enemyRocket[i].Draw(terrain);
//отображаем вражескую ракету
if (enemyRocket[i].GetBurn())
//если состояние горения, ...
{
explode[i+1].Fire(enemyRocket[i].GetXpos(),
enemyRocket[i].GetYpos(),enemyRocket[i].GetZpos());
//...то создаём взрыв
enemyRocket[i].SetBurn(false);
//отключаем состояние горения
}
if ((MyRobot.GetAlive()) && 
	(enemyRocket[i].GetAlive()))
//если все живы, то проверяем было ли столкновение
if (dbObjectCollision(MyRobot.GetNum(),
	enemyRocket[i].GetNum()) == 1)
	//произошло столкновение между
{   //между нашим роботом и вражеской ракетой
MyRobot.Life_Decrease(false);//уменьшаем кол-во жизней
enemyRocket[i].Die();//уничтожение ракеты
}
}

for (int i = 0; i < count+1; i++)
explode[i].Draw();//обновляем взрывы

if (MyRobot.GetMultiplayerMode() > 1) { 
	// во время сетевой игры...
		char yourName[50] = "Your name is ";
		strcat(yourName, userName);
dbText(10, 10, yourName);//выводим имя пользователя
		char killnum[15] = "You killed: ";
	strcat(killnum, dbStr(MyRobot.GetWinCount()));
dbText(/*ScreenWidth - 130*/640-130, 10, killnum);
//а также, выводим кол-во набранных фрагов
}

if ((dbEscapeKey()) && (gamemod > 0))
//уходим на паузу
{
if (GamePause()) {
	//если истина, то перезапускаем игру
dbStopSound(GameSound);//останавливаем музыку
int i;
MyRobot.~Robot();//явно вызываем деструкторы - 
//уничтожаем объекты
for (i = 0; i < count; i++) enemyRobot[i].~Robot();

MyRocket.~Rocket();

for (i = 0; i < count; i++) 
enemyRocket[i].~Rocket();

for (i = 0; i < count + 1; i++) 
explode[i].~Explode();

dbHideObject(terrain);
dbHideObject(skybox);
DarkGDK();
}
}

dbUpdateTerrain();// обновляем ландшафт
dbSync();// перерисовываем экран
}
	
//все объекты уничтожаются с помощью своих
//деструкторов, которые вызываются автоматически 
// после выхода объекта из области видимости

return;//возвращаем управление операционной системе
}