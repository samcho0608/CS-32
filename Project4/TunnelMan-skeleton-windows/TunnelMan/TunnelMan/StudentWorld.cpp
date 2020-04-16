#include "StudentWorld.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
/*INIT FUNCTIONS*/
int StudentWorld::init()
{
	B = std::min((int)getLevel() / 2 + 2, 9); //boulders
	G = std::max(5 - (int)getLevel() / 2, 2); //gold
	L = std::min(2 + (int)getLevel(), 21); //oil
	oilCount = 0;
	//Locate Earth
	for (int i = 0; i < VIEW_HEIGHT; i++) // row
	{
		for (int j = 0; j < VIEW_WIDTH; j++) // col
		{
			if (i < 60 && (i < 4 || (j < 30 || j > 33)))
				m_earth[j][i] = new Earth(j, i, this);
			else
				m_earth[j][i] = nullptr;
		}
	}

	int x, y;
	int count = 0;
	bool empty = false;
	while (count != B) // locates boulders
	{
		x = rand() % 60 + 1;
		y = rand() % 60 + 1;
		for (int i = x; i < x+4; i++)
			for (int j = y; j < y+4; j++)
				if (m_earth[i][j]==nullptr)
					empty = true;
		if (!tooClose(x, y) && !empty)
		{
			Boulder* boul = new Boulder(x, y, this);
			deleteEarth(x, y);
			addActors(boul);
			count++;
		}
		empty = false;
	}
	count = 0;

	while (count != L) // locates barrels
	{
		x = rand() % 60 + 1;
		y = rand() % 60 + 1;
		for (int i = x; i < x+4; i++)
			for (int j = y; j < y+4; j++)
				if (m_earth[i][j] == nullptr)
					empty = true;
		if (!tooClose(x, y) && !empty)
		{
			Barrel* bar = new Barrel(x, y, this);
			addActors(bar);
			count++;
		}
		empty = false;
	}
	count = 0;

	while (count != G) // locates gold nuggets
	{
		x = rand() % 60 + 1;
		y = rand() % 60 + 1;
		for (int i = x; i < x+4; i++)
			for (int j = y; j < y+4; j++)
				if (m_earth[i][j] == nullptr)
					empty = true;
		if (!tooClose(x, y) && !empty)
		{
			GoldNugget* gold = new GoldNugget(x, y, this, false, true, true);
			addActors(gold);
			count++;
		}
		empty = false;
	}

	int random = rand() % 10 + 1;
	if (random == min(90, (int)(getLevel() * 10 + 30)))
		addActors(new Hardcore(60, 60, this));
	else
		addActors(new Regular(60, 60, this));
	m_p++;

	m_player = new TunnelMan(this);
	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::tooClose(int x, int y)
{
	for (vector<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++)
	{
		for(int a = (*i)->getX(); a!=(*i)->getX()+4; a++)
			for(int b = (*i)->getY(); b!=(*i)->getY()+4;b++)
				if (distCheck(x, y, a, b, 6, true))
					return true;
	}
	return false;
}


/*MOVE FUNCTIONS*/
int StudentWorld::move()
{
	setDisplayText();
	if(m_player->getAlive())
		m_player->doSomething();
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	for (vector<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++)
	{
		if ((*i)->getAlive())
		{
			(*i)->doSomething();
			if (!m_player->getAlive())
			{
				playSound(SOUND_PLAYER_GIVE_UP);
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
			/*if level completed, return finished level constant*/
			if (L == oilCount)
				return GWSTATUS_FINISHED_LEVEL;
		}
		
	}
	
	m_tick++;
	int T = max(25, (int)(200 - getLevel()));
	int P = min(15, (int)(2 + getLevel()*1.5));
	if (m_tick >= T && m_p < P)
	{
		int random = rand() % 10 + 1;
		if (random == min(90, (int)(getLevel() * 10 + 30)))
			addActors(new Hardcore(60, 60, this));
		else
			addActors(new Regular(60, 60, this));
		m_p++;
	}

	//goodie placement
	int goodie = getLevel() * 25 + 300;
	if ((rand() % goodie + 1) == 1)
	{
		int probG = rand() % 5 + 1;
		if (probG == 1)
			m_actors.push_back(new SonarKit(0, 60, this));
		else
		{
			bool empty = false;
			int x, y;
			while (!empty)
			{
				empty = true;
				x = rand() % 56;
				y = rand() % 56;
				for (int i = x; i < x + 4; i++)
				{
					for (int j = y; j < y + 4; j++)
						if (m_earth[i][j] != nullptr)
						{
							empty = false;
							break;
						}
					if (!empty)
						break;
				}
			}
			m_actors.push_back(new Water(x, y, this));
		}
	}


	deleteDead();
	
	if (!m_player->getAlive())
	{
		playSound(SOUND_PLAYER_GIVE_UP);
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	/*if level completed, return finished level constant*/
	if (L == oilCount)
	{
		oilCount = 0;
		advanceToNextLevel();
		return GWSTATUS_FINISHED_LEVEL;
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setDisplayText()
{
	int level = getLevel();
	int lives = getLives();
	int health = m_player->getHP() * 10; // change to percentage
	int squirts = m_player->getWater();
	int gold = m_player->getGold();
	int barrelsLeft = L-oilCount;
	int sonar = m_player->getSonar();
	int score = getScore();
	// Next, create a string from your statistics, of the form:
	// Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
	string s = displayText(level, lives, health, squirts, gold, barrelsLeft, sonar, score);
	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

string StudentWorld::displayText(int level, int lives, int health,
	int squirts, int gold, int barrels, int sonar, int score) //fix
{
	ostringstream oss;
	oss << "Lvl:";
	oss << setw(2) << level;
	oss << " Lives:";
	oss << setw(1) << lives;
	oss << " Hlth:";
	oss << setw(3) << health << "%";
	oss << " Wtr:";
	oss << setw(2) << squirts;
	oss << " Gld:";
	oss << setw(2) << gold;
	oss << " Oil Left:";
	oss << setw(2) << barrels;
	oss << " Sonar:";
	oss << setw(2) << sonar;
	oss << " Scr:";
	oss.fill('0');
	oss << setw(6) << score;


	return oss.str();
}

bool StudentWorld::deleteEarth(int x, int y)
{
	bool deleted = false;
	for (int i = x; i <= x+3; i++)
	{
		for (int j = y; j <= y+3; j++)
		{
			if (m_earth[i][j] != nullptr)
			{
				m_earth[i][j]->setVisible(false);
				delete m_earth[i][j];
				m_earth[i][j] = nullptr;
				deleted = true;
			}
		}
	}
	return deleted;
}

void StudentWorld::deleteDead()
{
	std::vector<Actor*>::iterator i = m_actors.begin();
	while (i != m_actors.end())
	{
		if (!(*i)->getAlive())
		{
			(*i)->setVisible(false);
			Actor* temp = *i;
			i = m_actors.erase(i);
			delete temp;
		}
		else
			i++;
	}
}


/*ACTOR AUXILILARY FUNCTIONS*/
bool StudentWorld::straight(int x, int y, int xf, int yf)
{
	bool straight = true;
	int gx, lx, gy, ly;
	if (x > xf)
	{
		gx = x;
		lx = xf;
	}
	else
	{
		gx = xf;
		lx = x;
	}
	if (y > yf)
	{
		gy = y;
		ly = yf;
	}
	else
	{
		gy = yf;
		ly = y;
	}
	for (int i = lx; i != gx; i++)
		if ((m_earth[i][y] != nullptr) || block(i, y))
			straight = false;
	for (int i = ly; i != gy; i++)
		if ((m_earth[x][i] != nullptr) || block(x, i))
			straight = false;
	return straight;
}

void StudentWorld::useSonar(int x, int y)
{
	playSound(SOUND_SONAR);
	for (vector<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++)
		if((*i)->getType()== "Barrel" || (*i)->getType() == "GoldNugget")
			if (distCheck(x,y,(*i)->getX(), (*i)->getY(),12, false))
				(*i)->setVisible(true);
}

void StudentWorld::annoyProtesters(Actor* s)
{
	for (vector<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++)
	{
		if ((*i)->getType() == "Protester")
		{
			if (distCheck(s->getX(), s->getY(), (*i)->getX(), (*i)->getY(), 3, true))
			{
				(*i)->getAnnoyed();
				s->setAlive(false);
			}
		}
	}
}

bool StudentWorld::fallProtester(int x, int y)
{
	bool died = false;
	for (vector<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++)
	{
		if ((*i)->getType() == "Protester" && (*i)->getX() == x && (*i)->getY()==y)
		{
			(*i)->setAlive(false);
			died = true;
		}
	}
	return died;
}

bool StudentWorld::protesterGold(int x, int y)
{
	for (vector<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++)
	{
		if ((*i)->getType() == "Protester" && distCheck(x, y, (*i)->getX(), (*i)->getY(), 3, true))
		{
			playSound(SOUND_PROTESTER_FOUND_GOLD);
			(*i)->getBribed();
			return true;
		}
	}
	return false;
}

bool StudentWorld::distCheck(int x, int y, int xf, int yf, int limit, bool inclusive)
{
	double distX = pow(xf - x,2);
	double distY = pow(yf - y,2);
	if (inclusive)
		return (distX <= pow(limit,2)&& distY<= pow(limit,2));
	else
		return (distX < pow(limit, 2) && distY < pow(limit, 2));
}

bool StudentWorld::block(int x, int y)
{
	bool block = false;
	int centerX, centerY;
	for (vector<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++)
	{
		if ((*i)->getType() == "Boulder")
		{
			centerX = (*i)->getX();
			centerY = (*i)->getY();
			if(distCheck(x,y,centerX,centerY,3,true))
				block = true;
		}
	}
	return block;
}

bool StudentWorld::earthBlock(int x, int y)
{
	bool block = false;
	for(int i  = x; i < x+4;i++)
		for(int j = y; j < y+4;j++)
			if (m_earth[i][j] != nullptr)
				block = true;
	return block;
}