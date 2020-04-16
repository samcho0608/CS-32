#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <vector>
#include <algorithm>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir) {}

	virtual int init();

	virtual int move();

	virtual void cleanUp()
	{
		for (int i = 0; i < VIEW_WIDTH; i++)
			for (int j = 0; j < VIEW_HEIGHT; j++)
				delete m_earth[i][j];

		std::vector<Actor*>::iterator i = m_actors.begin();
		while( i != m_actors.end())
		{
			Actor* temp = *i;
			i = m_actors.erase(i);
			delete temp;
		}
		delete m_player;
	}

	~StudentWorld()
	{
		cleanUp();
	}
	/*
	void allVisible()
	{
		for (std::vector<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++)
		{
			if (!(*i)->isVisible())
				(*i)->setVisible(true);
			else
				(*i)->setVisible(false);
		}
	}
	*/
	Earth* getEarth(int x, int y) { return m_earth[x][y]; }
	bool protesterGold(int x, int y);
	bool tooClose(int x, int y);
	bool deleteEarth(int x, int y);
	void useSonar(int x, int y);
	void addActors(Actor* a)
	{
		m_actors.push_back(a);
	}
	void annoyProtesters(Actor* s);
	bool block(int x, int y);
	bool earthBlock(int x, int y);
	TunnelMan* getPlayer() { return m_player; }
	bool distCheck(int x, int y, int xf, int yf, int limit, bool inclusive);
	void deleteDead();
	void incOil() { oilCount += 1; }
	bool fallProtester(int x, int y);
	bool boulderBlock(int x, int y, Boulder* b)
	{
		for (std::vector<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++)
		{
			if ((*i)->getType() == "Boulder" && (*i)->getX() != b->getX() && (*i)->getY() != b->getY())
				if (distCheck(x, y, (*i)->getX(), (*i)->getY(), 3, true))
					return true;
		}
		return false;
	}
	void decP() { m_p -= 1; }
	bool straight(int x, int y, int xf, int yf);
	void setDisplayText();
	std::string displayText(int level, int lives, int health, int squirts, int gold, int barrels, int sonar, int score);

private:
	Earth* m_earth[VIEW_WIDTH][VIEW_HEIGHT] = {};
	TunnelMan* m_player = nullptr;
	std::vector<Actor*> m_actors;
	int B=0; //boulders
	int G=0; //gold
	int L=0; //oil
	int oilCount=0;
	int m_tick = 0;//tick for protesters
	int m_p = 0; // protester count
	bool dir[VIEW_WIDTH][VIEW_HEIGHT] = {};
};

#endif // STUDENTWORLD_H_
