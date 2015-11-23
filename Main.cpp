#include "Rocket.h"
#include "Explode.h"
#include "Robot.h"

// ����
char* robotPath = "robot\\robot.x";
//� �� ������� �� �����������
char* enrobotPath = "enemyrobot\\enemyrobot.x";
//������ ���
char* rocketPath = "Rocket\\rocket.x";
//� ������� ������ ������������ �� ����������� �������
char* GameSoundFile = "sound\\Engines.wav";
char* LevelCompleteFile = "sound\\LevelComplete.wav";
char* GameOverFile = "sound\\GAMEOVER.wav";
char* RobotShortFile = "sound\\Short.wav";
char* RocketExplodeFile = "sound\\RocketExplode.wav";
char* RobotExplodeFile = "sound\\RobotExplode.wav";

bool first = true;//��������� ��������
bool play = true;//����� ����
int gamemod = 0;//����� ����:
                //0 - ����� �� ����������
                //1 - ����������
                //2 - �����������

// ����������� ���� ��������� ������ ��� 
//������������� ���� ���������
// Tcp
const int NewClient = 0;
const int DeadClient = 1;
// Udp
const int PositionClient = 0;
const int Win = 1;// ������!!!
//���������� �������, ������� ���� ������� ��� 
//������ � ������������
const int MaxDeath = 10;

void LoadAllSounds()// ��������� ��� �����
{
dbLoadSound(GameSoundFile, GameSound);
//��������� ���� ������� �������
dbSetSoundVolume(GameSound, 95);
//����������� ���������
dbLoadSound(LevelCompleteFile, LevelComplete);
//��������� ���� ������� �������
dbLoadSound(GameOverFile, GameOver);
//��������� ���� ������� �������
dbLoad3DSound(RobotShortFile, RobotShort);
//��������� ����, ����� ������������ ������������
//��� � �������� ���������
dbSetSoundVolume(RobotShort, 85);
//����������� ���������
dbLoad3DSound(RocketExplodeFile, RocketExplode);
//��������� ����, ����� ������������ ������������
//��� � �������� ���������
dbSetSoundVolume(RocketExplode, 85);
//����������� ���������
dbLoad3DSound(RobotExplodeFile, RobotExplode);
//��������� ����, ����� ������������ ������������
//��� � �������� ���������
}

// � �������� �������� ���������� ����� 
// ��������� ������, ���������� �� �������
int MaximumClients;
int MaximumOperations;
//��� ������������
char* userName = NULL;
// ������, � ������� ����� �������� ��������
//�������� ip ������� 
char* ConnectIP = NULL;

bool makeConnect() // ������ ������� ����������
{
// ����� �����, � �������� ���� ������������
unsigned short ConnectPort = 7707;
// ����� �������� (� ��������), � ������� ��������
// ��������� ����� ����� ������ �� �������
int Timeout = 10;

while (ConnectIP == NULL)
{
dbCLS(dbRGB(0, 0, 0));
dbText(10, 10, "Enter the IP that you would like to connect to: ");
dbSetCursor(10, 50);
dbSync();
ConnectIP = dbInput();
int size = dbLen(ConnectIP);
if ((size > 16) || (size < 1)) {// �������� �� ����� 
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

// ���������� �����������
int Connect = mnConnect(ConnectIP,ConnectPort,
			ConnectIP,ConnectPort,Timeout, 0);

	// ���������� �������
	if(Connect == 1)
	{
		dbPrint("Connection was successful!");
	// �������� ���-�� �������� � udp-��������
		MaximumClients = mnGetMaxClients();
		MaximumOperations = mnGetMaxOperations();
// ������� ���������� ip, ������ ������ (tcp � udp)
		//� udp-�����
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
	// ���������� ����������� ��������...
if(Connect == 0)
{
dbPrint("Connection timed out! Check that the server is switched on, press any key");
dbSync();
dbWaitKey();
gamemod = 1;
return false;
}
	// ������ ��������� ��� �����������
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

void GetUserName()//������� ����������� ��� 
//������������
{
	while (userName == NULL)
	{
		dbCLS(dbRGB(0, 0, 0));
		dbText(10, 10, "Enter your name: ");
		dbSetCursor(10, 30);
		dbSync();
		userName = dbInput();
		int size = dbLen(userName);
		if ((size > 30) || (size < 1)) {// �������� ����� �� ����� 
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


bool SelectGameType()//�������� ��� ����: 
//�������������������� ��� ���������������������
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
dbSyncOn();//���� ����������� ������ ��� ���� 
//��������

dbSyncRate(30); // ������������� ������� 
//����������� ������

dbHideMouse();//�������� ����

dbSetDisplayMode(ScreenWidth, ScreenHeight, 
ScreenBitDepth);// ������������� ��������� ������
// ��� ������������ ���������� �������� �� ����� 
//��������� �� ������ ���������������� 
//���������� �������
//dbSetDisplayMode(640, 480, 32);// � ��� 
//�����������������
dbMaximiseWindow();// ������������� ���� �� ���� 
					//�����
	                  // � ��� ���� �������������

	if (!SelectGameType()) return false;

	dbLoadImage("DarkRobot_Loading.jpg",
		DarkRobot_Loading); //��������� 
	//�������� ��� ������ �������� ����
	dbPasteImage(DarkRobot_Loading,
		0, 0);// ������� � �� �����
dbSync();// ������� ����������� ���������� ������,
dbSync();// ����� �������� �� �����������

	// ������ �������� ��� ������� ������ � ������
	dbLoadImage("win_screen.jpg", win_screen);
	dbLoadImage("death_screen.jpg", death_screen);

dbSetCameraRange(1.0f, 30000.0f);// ������������� 
								 //������� 
	// � ������� ��������� ���������
	dbLoadImage("texture.jpg", basetex); 
	// ��������� ������� �1

	dbLoadImage("detail.jpg", dettex); 
	// ��������� ������� �2

dbSetupTerrain(); // �������, ���������������� 
//���������� ����������, 
//������ �������� ����� ��������� ������ ���������
dbMakeObjectTerrain(terrain);// ��������������� 
							//�������� �������
	// ���������� �����
	dbSetTerrainHeightMap(terrain, "map.bmp");
	// �������� ����� �����
	dbSetTerrainScale(terrain, 3.0f, 0.6f, 3.0f);
	// ���������������
	dbSetTerrainLight(terrain,  1.0f, -0.25f, 0.0f,
		1.0f, 1.0f, 0.78f, 0.5f );// ���������
	dbSetTerrainTexture(terrain, basetex, dettex);
	// ��������������� 
	dbBuildTerrain(terrain);
	// ������������� ��������� ���������

	dbLoadObject("skybox.x", skybox);
	// �������� �������� ��������
	dbSetObjectLight(skybox, 0);
	// ��������� ���������
	dbScaleObject(skybox, 10000, 10000, 10000);
	// ���������������
	dbSetObjectTexture(skybox, 3, 1);
	// ���������������

	dbLoadImage("fire.jpg", fire);
	// ������� ���� - ��������� �� �����

	// ���������� ��������� ������:
	//dbSetTextFont("Verdana");//�����
	//dbSetTextSize(24);//������

	// ������������� �������� ����������
	LoadAllSounds();

	first = false;

	return true;
}

void BSOD(const bool win)//Black Screen Of Death :)
{
	dbStopSound(GameSound);//������������� ������
	bool b = true;
	while (!play) {
		if (!win) {
	dbPasteImage(death_screen, 0, 0);
	//������� ������ ����������� �� �����
	if (b) {
		dbPlaySound(GameOver);//����������� ������
		//����
		b = false;
	}
		} else {
			dbPasteImage(win_screen, 0, 0);
			//������� ������ ����������� �� �����
			if (b) {
				dbPlaySound(LevelComplete);
				//����������� ������ ����
				b = false;
			}
		}
	dbText(50, ScreenHeight - 100,
	"if you wanna exit to main menu press Enter");
	dbSync();
			if (dbReturnKey()) {
				play = true;
				DarkGDK();//�������� ���� ������
				return;
			}
		}
}

// �����
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

//��������� ��� �������� ����� ������� � ���-�� 
//��������� ������
struct User 
{
string name;
int frag;
};

User user[count+1];

bool NetGameOver()//������� �����
{
	while (true) {
	dbCLS(dbRGB(0, 0, 0));
dbPrint("DarkROBOT - Multiplayer Game - Totals:");
	for (int i = 0; i < count + 1; i++)
	{
	string s = user[i].name;
	if (s == "") continue;
	char* ch = &s[0];//���� ����� 1-�� �������,
	//� ������ � ��� ��� ������ :)
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

void DarkGDK(void) // ����� ����� - ������� �������
{
	dbRandomize(dbTimer());// ������������� 
	//���������� ��������� ��������

	dbDisableEscapeKey();//���������� ����������� 
//���������� ������ ���������� ��� ������� �� Escape

	if (first) {//��� ������ ��������
	if (!Init()) return; // ��������� �������������
	} else 
	{
		if (!SelectGameType()) return;
		dbPasteImage(DarkRobot_Loading, 0, 0);
		// ������� �������� �� �����
		dbSync();//�������������� �����
		dbShowObject(skybox);
		//������ �������� ������� �������,
		dbShowObject(terrain);
		//������, ���� �� �� �� ����������
	}     //�� ��������� ����������� ������   

// ��� ������� �������� �� ������ �������� ��������
	// ������, ��� �������� ����������
	float xx = dbRnd(dbObjectSizeX(terrain)-1);
	float zz = dbRnd(dbObjectSizeZ(terrain)-1);
float yy = dbGetTerrainGroundHeight(terrain, 
									xx, zz);
	Robot MyRobot(myrobot, 10, xx, yy,
		zz, 0, robotPath, gamemod);
	//������ ����������������� ������

	int lifecount;//���������� ������
	(gamemod > 1) ? lifecount = 10 : lifecount = 5;
	// ��� ��������������������� ���� ���������� 
	//������ �� �������� ����� �������� �� ������,
	// ��� ��� �� �������� �� �� ����� ������� ��
	//���������������� �������
	Robot enemyRobot[count] = // ������ �������: 
		//4-� �������-������
	{
		Robot(rob_num[0], lifecount, 100, 23, 700,
		0, enrobotPath, gamemod), //� ����������� �
		//�������� ���������� ��������� 		
		Robot(rob_num[1], lifecount, 300, 23, 500,
		180, enrobotPath, gamemod),//��������� ��� 
		//������������ �����-�������-������
		Robot(rob_num[2], lifecount, 500, 23, 300, 
		180, enrobotPath, gamemod),//� ����� ������
		//��� ����� ���������
		Robot(rob_num[3], lifecount, 700, 23, 100,
		0, enrobotPath, gamemod)//�� ������ �� 
		//������ ����������� ������ �������
	};    //� ������������ ������ ������

	Rocket MyRocket(myrocket, rocketPath);
	//������ ���������������� ������

	Rocket enemyRocket[count] = //������ ��� 4 
	                    	//��������� ������ 
	{                     //�.�. ��� ������ ���������
Rocket(rock_num[0], ""),//������ ��������� ����� 
Rocket(rock_num[1], ""),//����� ����������� � ������
Rocket(rock_num[2], ""),//���������������� ������
Rocket(rock_num[3], "")//� ������������ ������
	};

	Explode explode[count+1] = //������ ��� ������
	{
		Explode(exp_num[0]),
		Explode(exp_num[1]),
		Explode(exp_num[2]),
		Explode(exp_num[3]),
		Explode(exp_num[4])
	};

	// �������� ��������������� �����
dbLoopSound(GameSound);//������������� � �����������

//dbPositionCamera(424, 27, 89);// ������������� ������
dbPositionCamera(200, 100, 100);

DWORD globalTime = 0;// ���������� �������������������

// �������
int fire = 0;

	while (LoopGDK() && (play))// �������� ����
	{    
		// ���� �������������� �����...
		if (MyRobot.GetMultiplayerMode() > 1)
		{
		// ������� ��������� �� �������
		// ��������� tcp-���������
		int iTcpPackets = mnRecvTCP(NULL);
		
		// ���� ����...
		if(iTcpPackets > 0)
		{
// �������� �����-��� �������� �� ���������� 
//���������
// � ��� �� ����� �������, � ������� ���
//���������
			int Operation = mnGetInt();
			int Client = mnGetInt();

// � ��������� ���������, ��� ����������� �����
//������
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

// ������ ����������
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
	
// ��������� �� ������� ������-����������
		//udp-���������
// � ������� ���������� ������������ �������
		// ��������� ������� �������
for(int Client = 1;Client<=MaximumClients;Client++)
{
// ��������� ��� �������� �������� �������
for(int Operation = 0;Operation<MaximumOperations;
	                                   Operation++)
{
// ����������, �������� �� ������� udp-���������
int UdpPackets = mnRecvUDP(Client,Operation);
// ���� ��� ����� �����.....
if ((Operation == PositionClient) && (UdpPackets == 1))
{
for (int i = 0; i <= count; i++)
if ((enemyRobot[i].GetAlive()) &&
(Client == enemyRobot[i].GetNumber()))
{
// ������� �� ���� (���������) ������������ � ���� 
	//�������� �������
		// � �������� � ��� ������
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
// ���-�� ������ ������ ���-�� ������ - 
//����� ������� ����!
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
MyRobot.~Robot();//���� �������� ������������ -
//���������� �������
for (i = 0; i < count; i++) enemyRobot[i].~Robot();

				MyRocket.~Rocket();

for (i = 0; i < count; i++) 
enemyRocket[i].~Rocket();

for (i = 0; i < count + 1; i++)
explode[i].~Explode();

dbHideObject(terrain);
dbHideObject(skybox);

if (!NetGameOver()) return;//������� �����
}
}
}
		
// �������� ���������� � ���� �������� ������ 
//������ �� ������ ����� udp
		// ��� ����� ������������ �����
		mnAddInt(PositionClient);
		mnAddFloat(MyRobot.GetXpos());
		mnAddFloat(MyRobot.GetYpos());
		mnAddFloat(MyRobot.GetZpos());
		mnAddFloat(MyRobot.GetDegree());	
		mnAddInt(fire);
		mnAddInt(MyRobot.GetLifeCount());
//��������� ������ ������������� ������� ��� 
//���������� ����������, � �������, �������� 
//��� �� ����������
mnAddString(userName);
int p = MyRobot.GetWinCount();
mnAddInt(p);
if (p >= MaxDeath) MyRobot.ZeroWinCount()
;// ����� ��� ������ ���� "������" � ������
		fire = 0;
		// ����������, �������� ������
		mnSendUDP(NULL,false,true);
} // - ������ ����� ��� ���� ������� ���

int win = 0;//������� �������� ������
for (int i = 0; i < count; i++)//���������:
//���� ����� �����,
if (!enemyRobot[i].GetAlive()) {
	//� ��� ������ ��������,
win++;
if (enemyRocket[i].GetAlive()) 
enemyRocket[i].Die();//�� ���������� �
}
if ((MyRobot.GetMultiplayerMode() == 1) && 
	(MyRobot.GetAlive()) &&
	(play) && (win == count) && ((GetTickCount() - 
	globalTime) > 3000))//�������� �� ������
	{
			play = false;
			int i;
			MyRobot.~Robot();
			//���� �������� ����������� - ���������� �������
			for (i = 0; i < count; i++) 
				enemyRobot[i].~Robot();

			MyRocket.~Rocket();

			for (i = 0; i < count; i++) 
				enemyRocket[i].~Rocket();

			for (i = 0; i < count + 1; i++) 
				explode[i].~Explode();

			dbHideObject(terrain);
			dbHideObject(skybox);
			BSOD(true);// ������� ���������� ����
		}

if (MyRobot.GetAlive()) 
MyRobot.Control_Position(terrain);
//�������, ������� ��������� ������� ������� 
//� ���������� ����������� ��������, ����������� 
//(� �����, ����������� �����������)
//����������������� ������

if ((dbSpaceKey()) && (MyRobot.GetAlive()))
// ��� �������
{
MyRocket.Fire(MyRobot.GetXpos(), MyRobot.GetYpos(), 
MyRobot.GetZpos(), MyRobot.GetDegree());
fire = 1;// ������������� ��������
}

dbSetCameraToObjectOrientation(MyRobot.GetNum());
//���������� ������, �������������� ���������� ������

dbXRotateCamera(25);// ������������ ������ �� ��� �

bool hit = false, hit2 = false;//������ �������� 
//� ���, ���� �� ������������
float deg = 0;

for (int i = 0; i < count; i++)
{	
int j = 0;
hit = false;
hit2 = false;
deg = 0;
if ((MyRobot.GetAlive()) && (enemyRobot[i].GetAlive()))
//���� ��� ����, �� ��������� ���� �� ������������
if (dbObjectCollision(MyRobot.GetNum(), 
	enemyRobot[i].GetNum()) == 1)
	//1-��������� ������������ ����� 
	{     //����� � ��������� ��������
		hit = true;//������������ ���������
	}

	deg = 0;
if ((hit) && (MyRobot.GetAlive())) 
//���� �� ������������ � ��� �� ��� �����
{
deg = -MyRobot.GetDegree();
MyRobot.Draw(true, hit, true, terrain, 0);
//���� ������ ������� ��� �������, 
MyRobot.Draw(true, hit, true, terrain, 0);
//����� ��������� ���������� ������������
}
enemyRobot[i].Draw(false, hit, true, terrain, deg);
//��������� ��������� ������
// ������� ���������� ������
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
//���� ��� ����, �� ��������� ���� �� ������������
if (dbObjectCollision(enemyRobot[i].GetNum(), 
	enemyRobot[j].GetNum()) == 1)
	//1-��������� ������������ ����� 
{  //���������� ��������
	hit2 = true;//������������ ���������
}
}
deg = 0;
if ((hit2) && (enemyRobot[i].GetAlive()))
//���� �� ������������ � ��� �� ��������� ����� 
{
deg = 90;
enemyRobot[i].Draw(false, hit2, false, terrain, 
				deg);//��������� ��������� ������
enemyRobot[j].Draw(false, hit2, false, terrain,
			deg);//� ������ ������������ ����������
}
}
}

MyRobot.Draw(true, false, true, terrain, 0);
//��������� ��������� ������

if ((!MyRobot.GetAlive()) && (play) && 
	((GetTickCount() - MyRobot.GetTime()) > 3000))
	//�������� �� ������
{
play = false;
int i;
MyRobot.~Robot();//���� �������� ������������ -
//���������� �������
for (i = 0; i < count; i++) enemyRobot[i].~Robot();

MyRocket.~Rocket();

for (i = 0; i < count; i++) 
enemyRocket[i].~Rocket();

for (i = 0; i < count + 1; i++) 
explode[i].~Explode();

dbHideObject(terrain);
dbHideObject(skybox);
BSOD(false);// ������� ���������� ����
}

for (int i = 0; i < count; i++) 
if ((MyRocket.GetAlive()) && 
	(enemyRobot[i].GetAlive()))
//���� ��� ����, �� ��������� ���� �� ������������
if (dbObjectCollision(MyRocket.GetNum(), 
	enemyRobot[i].GetNum()) == 1)
	//��������� ������������ �����
{  //��������� ������� � ����� �������
enemyRobot[i].Life_Decrease(myrobot);
//��������� ���-�� ������
globalTime = GetTickCount();
//���������� ��� ��������� :)
MyRocket.Die();//����������� ������
}

if (MyRobot.GetMultiplayerMode() > 1)
//������ � ������������ ������������ �����
//��������� ������� 
{//� (������) ��������� ������� ����� ��������
for (int i = 0; i < count; i++) 
for (int j = 0; j < count; j++)
if (i != j)
if ((enemyRocket[i].GetAlive()) &&
	(enemyRobot[j].GetAlive()))
//���� ��� ����, �� ��������� ���� �� ������������
if (dbObjectCollision(enemyRocket[i].GetNum(), 
	enemyRobot[j].GetNum()) == 1)
	//��������� ������������ �����
{  //��������� ������� � ��������� �������
enemyRobot[j].Life_Decrease(false);
//��������� ���-�� ������
globalTime = GetTickCount();
//���������� ��� ��������� :)
enemyRocket[i].Die();//����������� ������
}
}
			                                                           
MyRocket.Draw(terrain);
//������� "����" ������ �� �����
if (MyRocket.GetBurn())//���� ��������� �������,...
{
explode[0].Fire(MyRocket.GetXpos(),
MyRocket.GetYpos(), MyRocket.GetZpos());
//...�� ������ �����
MyRocket.SetBurn(false);
//��������� ��������� �������
}

for (int i = 0; i < count; i++)
{
enemyRocket[i].Draw(terrain);
//���������� ��������� ������
if (enemyRocket[i].GetBurn())
//���� ��������� �������, ...
{
explode[i+1].Fire(enemyRocket[i].GetXpos(),
enemyRocket[i].GetYpos(),enemyRocket[i].GetZpos());
//...�� ������ �����
enemyRocket[i].SetBurn(false);
//��������� ��������� �������
}
if ((MyRobot.GetAlive()) && 
	(enemyRocket[i].GetAlive()))
//���� ��� ����, �� ��������� ���� �� ������������
if (dbObjectCollision(MyRobot.GetNum(),
	enemyRocket[i].GetNum()) == 1)
	//��������� ������������ �����
{   //����� ����� ������� � ��������� �������
MyRobot.Life_Decrease(false);//��������� ���-�� ������
enemyRocket[i].Die();//����������� ������
}
}

for (int i = 0; i < count+1; i++)
explode[i].Draw();//��������� ������

if (MyRobot.GetMultiplayerMode() > 1) { 
	// �� ����� ������� ����...
		char yourName[50] = "Your name is ";
		strcat(yourName, userName);
dbText(10, 10, yourName);//������� ��� ������������
		char killnum[15] = "You killed: ";
	strcat(killnum, dbStr(MyRobot.GetWinCount()));
dbText(/*ScreenWidth - 130*/640-130, 10, killnum);
//� �����, ������� ���-�� ��������� ������
}

if ((dbEscapeKey()) && (gamemod > 0))
//������ �� �����
{
if (GamePause()) {
	//���� ������, �� ������������� ����
dbStopSound(GameSound);//������������� ������
int i;
MyRobot.~Robot();//���� �������� ����������� - 
//���������� �������
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

dbUpdateTerrain();// ��������� ��������
dbSync();// �������������� �����
}
	
//��� ������� ������������ � ������� �����
//������������, ������� ���������� ������������� 
// ����� ������ ������� �� ������� ���������

return;//���������� ���������� ������������ �������
}