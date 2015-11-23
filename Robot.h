#include "Game_Obj.h"

class Robot : public GameObj// наследование
{
private:
	int lifes, // жизни
		cnumber;// номер клиента
		bool collide,//в этой переменной хранитс€ 
			//состо€ние объекта на счЄт столкновени€
		 evil;//режим, в котором находитс€ робот: 
		//активный или пассивный

// идентификатор мултиплеерной игры
int multiplayer;

public:
	Robot();// конструктор по умолчанию
	Robot(int i, int life, float x, float y, 
	float z, float degree, char* filename, 
	int multigame);//конструктор с параметрами
	void Draw(bool user, bool hit, bool colgam,
	int terrain, float degree);//переопредел€ем 
	//виртуальную функцию класса-предка
	void Control_Position(int terrain);	//функци€, 
//управл€юща€ движением пользовательского робота                                           
	void Life_Decrease(bool strikenbyMR);//в 
//параметре передаЄтс€ идентификатор автора выстрела:
//если это глав.пер., то тогда - истина и + 1 победа
	DWORD GetTime() { return start_time; } 
	// получаем врем€ начала отсчЄта
	void Die(bool strikenbyMR);//гибель робота
	void SetAlive(bool live);//оживл€ем робота
	void Check_Position();//функци€, провер€юща€
	//текущую позицию робота
	void AI_Moving(const float speed);//функци€, 
	//управл€юща€ движением вражеских роботов
	void PlaySound();//прототип такой же как в 
	//классе предке, функци€ проигрывает звук
	void SetNumber(int i);//установить номер клиента
	int GetNumber();//возвратить номер клиента
	void SetLifeCount(int life);
	//установить кол-во жизней
	int GetLifeCount();
	//узнать сколько жизней осталось
	int GetWinCount();//вернуть кол-во побед
	void ZeroWinCount();//обнул€ем счЄтчик побед
	void SetMultiplayerMode(int mode);
	//устанавлаем мульлиплеерный режим игры
	int GetMultiplayerMode();
	//узнать текущий режим игры
	~Robot();//деструктор
};