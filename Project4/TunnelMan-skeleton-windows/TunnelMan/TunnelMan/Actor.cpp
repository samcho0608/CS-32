#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

void Actor::move(Direction dir, int x, int y, int dist)
{
	int destX = x;
	int destY = y;
	if (getDirection() == left)
		destX -= dist;
	else if (getDirection() == right)
		destX += dist;
	else if (getDirection() == up)
		destY += dist;
	else
		destY -= dist;

	if ((destX >= 0 && destX <= 60) && (destY >= 0 && destY <= 60) && !getWorld()->block(destX, destY))
		moveTo(destX, destY);
	else
		moveTo(x, y);
}

// TunnelMan implementation
void TunnelMan::doSomething()
{
	if (!getAlive())
		return;
	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		// user hit a key this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			if (getDirection() == left)
				move(left, getX(), getY(), 1);
			else
				setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() == right)
				move(right, getX(), getY(), 1);
			else
				setDirection(right);
			break;
		case KEY_PRESS_UP:
			if (getDirection() == up)
				move(up, getX(), getY(), 1);
			else
				setDirection(up);
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() == down)
				move(down, getX(), getY(), 1);
			else
				setDirection(down);
			break;
		case KEY_PRESS_ESCAPE:
			setAlive(false);
			break;
		case KEY_PRESS_SPACE:
			if (m_water > 0)
			{
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
				Squirt* water;
				Direction dir;
				int destX = getX();
				int destY = getY();
				if (getDirection() == up)
				{
					destY += 4;
					dir = up;
				}
				else if (getDirection() == down)
				{
					destY -= 4;
					dir = down;
				}
				else if (getDirection() == left)
				{
					destX -= 4;
					dir = left;
				}
				else
				{
					destX += 4;
					dir = right;
				}
				if ((destX >= 0 && destX <= 60) && (destY >= 0 && destY <= 60) && !getWorld()->block(destX, destY)&&!getWorld()->earthBlock(destX, destY))
				{
					water = new Squirt(destX, destY, dir, getWorld());
					getWorld()->addActors(water);
				}
				m_water--;
			}
			break;
		case KEY_PRESS_TAB:
			if (m_gold > 0)
			{
				GoldNugget* gold;
				gold = new GoldNugget(getX(), getY(), getWorld(), true, false, false);
				getWorld()->addActors(gold);
				m_gold--;
			}
			break;
		case 'Z':
		case 'z':
			if (m_sonar > 0)
			{
				m_sonar--;
				getWorld()->useSonar(getX(), getY());
			}
			break;
		/*case 'V':
		case 'v': // DELETE LATER
			getWorld()->allVisible();
			break;*/
		}
		if(getWorld()->deleteEarth(getX(), getY()))
			getWorld()->playSound(SOUND_DIG);
	}
}

void TunnelMan::getAnnoyed()
{
	m_hp -= 2;
	if (m_hp <= 0)
		setAlive(false);
}

// SQUIRT FUNCTIONS
void Squirt::doSomething()
{

	getWorld()->annoyProtesters(this);

	if (m_travel == 0)
		setAlive(false);
	//else
	Direction dir = getDirection();
	int destX = getX();
	int destY = getY();
	if (dir == up)
		destY += 1;
	else if (dir == down)
		destY -= 1;
	else if (dir == right)
		destX += 1;
	else
		destX -= 1;
	if(!getWorld()->earthBlock(destX, destY))
		move(dir, getX(), getY(), 1);
	m_travel--;
	//setAlive(false);
}

/*BARREL FUNCTIONS*/

void Barrel::doSomething()
{
	if (!getAlive())
		return;
	TunnelMan* player = getWorld()->getPlayer();
	if (!isVisible())
	{
		if (pow(getX() - player->getX(), 2) <= 16 && pow(getY() - player->getY(), 2) <= 16)
		{
			setVisible(true);
			return;
		}
	}
	if (pow(getX() - player->getX(), 2) <= 9 && pow(getY() - player->getY(), 2) <= 9)
	{
		setAlive(false);
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->incOil();
	}
}

/*GOLDNUGGET FUNCTION*/
void GoldNugget::doSomething()
{
	if (!getAlive())
		return;
	TunnelMan* player = getWorld()->getPlayer();
	if (!isVisible())
	{
		if (pow(getX() - player->getX(), 2) <= 16 && pow(getY() - player->getY(), 2) <= 16)
		{
			setVisible(true);
			return;
		}
	}

	//player
	if (pow(getX() - player->getX(), 2) <= 9 && pow(getY() - player->getY(), 2) <= 9 && m_pickup)
	{
		setAlive(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		player->addGold(1);
		getWorld()->increaseScore(10);
		return;
	}

	//protesters
	if (pow(getX() - player->getX(), 2) <= 9 && pow(getY() - player->getY(), 2) <= 9 && m_pickup)
		if(getWorld()->protesterGold(getX(),getY()))
			setAlive(false);
	return;
}

/*BOULDER FUNCTION*/
void Boulder::doSomething() // fix
{
	if (!getAlive() || getY() == 0)
		return;
	if (m_stable == "stable")
	{
		for (int i = getX(); i < getX() + 4; i++)
			if (getWorld()->getEarth(i, getY() - 1) != nullptr)
				return;
		m_stable = "waiting";
	}
	else if (m_stable == "waiting")
	{
		m_tick++;
		if (m_tick > 30)
		{
			getWorld()->playSound(SOUND_FALLING_ROCK);
			m_stable = "falling";
		}
	}
	else // falling
	{
		int destY = getY() - 1;
		TunnelMan* player = getWorld()->getPlayer();
		if (getWorld()->distCheck(getX(), destY, player->getX(), player->getY(), 3, true))
			player->setAlive(false);
		if (getWorld()->fallProtester(getX(), destY))
			setAlive(false);

		if (destY == 0 || getWorld()->earthBlock(getX(), destY) ||getWorld()->boulderBlock(getX(),destY,this))
			setAlive(false);
		else
			moveTo(getX(), destY);
	}
}

/*SONAR KIT FUNCTION*/
SonarKit::SonarKit(int x, int y, StudentWorld* sWorld) :
	Actor(TID_SONAR, x, y, right, 1.0, 2, sWorld)
{
	setType("SonarKit");
	m_ticks = std::max(100, (int)(300 - 10 * (sWorld->getLevel())));
}

void SonarKit::doSomething()
{
	m_ticks--;
	if (!getAlive())
		return;
	TunnelMan* player = getWorld()->getPlayer();
	if (pow(getX() - player->getX(), 2) <= 9 && pow(getY() - player->getY(), 2) <= 9 && m_pickup)
	{
		setAlive(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		player->addSonar(1);
		getWorld()->increaseScore(75);
		return;
	}
	if (m_ticks <= 0)
		setAlive(false);
}

/*WATER FUNCTIONS*/
Water::Water(int x, int y, StudentWorld* sWorld)
	:Actor(TID_WATER_POOL, x, y, right, 1.0, 2, sWorld)
{
	m_ticks = max(100, (int)(300 - 10 * getWorld()->getLevel()));
}

void Water::doSomething()
{
	m_ticks--;
	if (!getAlive())
		return;
	TunnelMan* player = getWorld()->getPlayer();
	if (pow(getX() - player->getX(), 2) <= 9 && pow(getY() - player->getY(), 2) <= 9 && m_pickup)
	{
		setAlive(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		player->addWater(5);
		getWorld()->increaseScore(100);
		return;
	}
	if (m_ticks <= 0)
		setAlive(false);
}

/*PROTESTER FUNCTIONS*/

void Regular::doSomething()
{
	if (m_stun > 0)
	{
		m_stun--;
		return;
	}
	if (getHP() <= 0)
		setLeave(true);
	if (!getAlive())
		return;
	if (!getState())
		countRest();
	if (getLeave())
	{
		if (getX() == 60 && getY() == 60)
			setAlive(false);
		//recursion
		
	}
	TunnelMan* a = getWorld()->getPlayer();
	if (getWorld()->distCheck(getX(), getY(), a->getX(), a->getY(), 4, true))
	{
		getWorld()->playSound(SOUND_PROTESTER_YELL);
		a->getAnnoyed();
		m_stop = true; // check this again
		return;
	}


	else if ((a->getX() == getX() || a->getY() == getY())
		&& !getWorld()->distCheck(getX(), getY(), a->getX(), a->getY(), 4, true)
		&& getWorld()->straight(a->getX(),a->getY(),getX(),getY()))
	{
		if (a->getX() > getX())
			setDirection(right);
		else if (a->getX() < getX())
			setDirection(left);
		else if (a->getY() > getY())
			setDirection(up);
		else
			setDirection(down);
		move(getDirection(), getX(), getY(), 1);
		resetCur();
	}

	/*else
	{
		addCur(-1);
		if (getCur() <= 0)
		{
			while (true)
			{
				int random = rand() % 4 + 1;
				if (random == 1)
					setDirection(up);
				else if (random == 2)
					setDirection(down);
				else if (random == 3)
					setDirection(right);
				else
					setDirection(left);

				Direction dir = getDirection();
				int destX = getX();
				int destY = getY();
				if (dir == up)
					destY += 1;
				else if (dir == down)
					destY -= 1;
				else if (dir == right)
					destX += 1;
				else
					destX -= 1;
				if (!getWorld()->earthBlock(destX, destY) && !getWorld()->block(destX, destY))
					break;
			}
			setCur();
		}
		else
		{
			bool perp = false;
			if (getDirection() == left || getDirection() == right)
			{
				if(getWorld()->earthBlock(getX(), getY()-1) || getWorld()->earthBlock(getX(),getY()+1)
					||getWorld()->block(getX(), getY()-1) || getWorld()->block(getX(),getY()+1))
					perp = true;
			}
			else
			{
				perp = true;
			}

		}

	}*/


}

void Regular::getAnnoyed()
{
	if (getLeave())
		return;
	addHP(-2);
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	m_stun = std::max(50, 100 - (int)(getWorld()->getLevel()) * 10);
	if (getHP() < 0)
	{
		setLeave(true);
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		resetRest();
	}
}

void Regular::getBribed()
{
	getWorld()->increaseScore(25);
	setLeave(true);
}

void Hardcore::getBribed()
{
	getWorld()->increaseScore(50);
	//stare
}

void Hardcore::doSomething()
{
	if (getHP() <= 0)
		setLeave(true);
	if (!getAlive())
		return;
	if (!getState())
		countRest();
	if (getLeave())
	{
		if (getX() == 60 && getY() == 60)
			setAlive(false);
		//recursion

	}
	TunnelMan* a = getWorld()->getPlayer();
	if (getWorld()->distCheck(getX(), getY(), a->getX(), a->getY(), 4, true))
	{
		getWorld()->playSound(SOUND_PROTESTER_YELL);
		a->getAnnoyed();
		m_stop = true; // check this again
		return;
	}


	else if ((a->getX() == getX() || a->getY() == getY())
		&& !getWorld()->distCheck(getX(), getY(), a->getX(), a->getY(), 4, true)
		&& getWorld()->straight(a->getX(), a->getY(), getX(), getY()))
	{
		if (a->getX() > getX())
			setDirection(right);
		else if (a->getX() < getX())
			setDirection(left);
		else if (a->getY() > getY())
			setDirection(up);
		else
			setDirection(down);
		move(getDirection(), getX(), getY(), 1);
		resetCur();
	}
}