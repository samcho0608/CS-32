#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <algorithm>

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth,
		StudentWorld* sWorld) : GraphObject(imageID, startX, startY, dir, size, depth),
		m_world(sWorld), m_alive(true) {setVisible(true); }
	~Actor() {}
	virtual void doSomething()=0;
	virtual void getBribed() {}
	StudentWorld* getWorld() { return m_world; }
	virtual void getAnnoyed() {}
	bool getAlive() { return m_alive; }
	void setAlive(bool alive) { m_alive = alive; }
	std::string getType() { return m_type; }
	void setType(std::string type) { m_type = type; }
	void move(Direction dir, int x, int y, int dist);
private:
	StudentWorld* m_world;
	bool m_alive;
	std::string m_type;
};

class Earth : public Actor
{
public:
	Earth(int x, int y, StudentWorld* sWorld) : Actor(TID_EARTH, x, y, right, 0.25, 3, sWorld) { setType("Earth"); }
	~Earth() {}
	virtual void doSomething() {}
private:
};

class TunnelMan : public Actor
{
public:
	TunnelMan(StudentWorld* sWorld) : Actor(TID_PLAYER, 30, 60, right, 1.0, 0, sWorld), 
		m_hp(10), m_water(5), m_sonar(1), m_gold(0) {}
	~TunnelMan() {}
	virtual void doSomething();
	virtual void getAnnoyed();
	int getHP() { return m_hp; }
	void addHP(int x) { m_hp += x; }
	int getWater() { return m_water; }
	void addWater(int x) { m_water += x; }
	int getSonar() { return m_sonar; }
	void addSonar(int x) { m_sonar += x; }
	int getGold() { return m_gold; }
	void addGold(int x) { m_gold += x; }
private:
	int m_hp;
	int m_water;
	int m_sonar;
	int m_gold;
};

class Boulder : public Actor
{
public:
	Boulder(int x, int y, StudentWorld* sWorld) : Actor(TID_BOULDER, x, y, down, 1.0, 1, sWorld) { setType("Boulder"); }
	void doSomething();
private:
	std::string m_stable = "stable";
	int m_tick=0;
};

class Squirt : public Actor
{
public:
	Squirt(int x, int y, Direction dir, StudentWorld* sWorld) : 
		Actor(TID_WATER_SPURT, x, y, dir, 1.0, 1, sWorld), m_travel(4) {
		setType("Squirt");
	}
	virtual void doSomething();
private:
	int m_travel;
};

class Barrel :public Actor
{
public:
	Barrel(int x, int y, StudentWorld* sWorld) :
		Actor(TID_BARREL, x, y, right, 1.0, 2, sWorld) 
	{
		setType("Barrel");
		setVisible(false);
	}
	virtual void doSomething();
private:
};

class GoldNugget : public Actor
{
public:
	GoldNugget(int x, int y, StudentWorld* sWorld, bool visibility, bool pickup, bool state) :
		Actor(TID_GOLD, x, y, right, 1.0, 2, sWorld), m_pickup(pickup), m_state(state)
	{
		setType("GoldNugget");
		setVisible(visibility);
	}
	void doSomething();
private:
	bool m_pickup; // true = tunnelman, false = protestors
	bool m_state; // true = permanent, false = temporary
};

class SonarKit : public Actor
{
public:
	SonarKit(int x, int y, StudentWorld* sWorld);
	virtual void doSomething();
private:
	const bool m_pickup = true; //can only be picked up by tunnelman
	const bool m_state = false; //always temporary
	int m_ticks;
};

class Water : public Actor
{
public:
	Water(int x, int y, StudentWorld* sWorld);
	virtual void doSomething();
private:
	int m_ticks;
	const bool m_pickup = true;
	const bool m_state = false;
};

class Protester : public Actor
{
public:
	Protester(int imageID, int x, int y,int hp, bool l, StudentWorld* sWorld) :
		Actor(imageID, x, y, left, 1.0, 0, sWorld),m_hp(hp),m_leave(l)
	{
		setType("Protester");
	}
	//bool exit(int sx, int sy);
	
	//getter and setter
	bool getState() { return m_state; }
	void setState(bool s) { m_state = s; }
	int getRest() { return restingTicks; }
	void countRest() { restingTicks++; }
	void resetRest() { restingTicks = 0; }
	int getHP() { return m_hp; }
	void addHP(int i) { m_hp += i; }
	bool getLeave() { return m_leave; }
	void setLeave(bool l) { m_leave = l; }
	void countTick() { m_ticks++; }
	int getTicks() { return m_ticks; }
	void resetTicks() { m_ticks = 0; }
	int getCur() { return numCur; }
	void resetCur() { numCur = 0; }
	void setCur() { numCur = rand() % 53 + 8; }
	void addCur(int i) { numCur += i; }


private:
	int m_ticks=0;
	int restingTicks=0;
	int m_hp;
	bool m_state=false; //false == rest
	bool m_leave; //true == leave
	int numCur = rand() % 53 + 8; // 8~60
};

class Regular : public Protester
{
public:
	Regular(int x, int y, StudentWorld* sWorld) :
		Protester(TID_PROTESTER, x, y, 5, false, sWorld) {}
	virtual void doSomething();
	virtual void getAnnoyed();
	virtual void getBribed();

private:
	bool m_stop; // true == stop shouting
	int m_stun=0;
};

class Hardcore : public Protester
{
public:
	Hardcore(int x, int y, StudentWorld* sWorld): Protester(TID_HARD_CORE_PROTESTER, x, y,20,false,sWorld){}
	virtual void getBribed();
	virtual void doSomething();
private:
	bool m_stop;
};

#endif // ACTOR_H_
