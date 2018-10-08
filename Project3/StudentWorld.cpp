#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

int StudentWorld::init()
{
	//initialize stars
	for (int i = 0; i < 30; i++)
	{
		addActor(new Star(IID_STAR, randInt(0, VIEW_WIDTH - 1), randInt(0, VIEW_HEIGHT - 1), 0, (randInt(5, 50) / 100.0), 3, this));
	}

	player = new NachenBlaster(IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, this);

	currentAliens = 0;
	destroyedAliens = 0;
	remainingAliens = 6 + (4 * getLevel());
	maximumAliens = 4 + (0.5 * getLevel());

	S1 = 60;
	S2 = 20 + (getLevel() * 5);
	S3 = 5 + (getLevel() * 10);
	S = S1 + S2 + S3;

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	ostringstream oss;
	oss.setf(ios::fixed);
	oss.precision(0);
	oss << "Lives: " << getLives();
	oss << setw(10) << "Health: " << player->healthPct() << "%";
	oss << setw(9) << "Score: " << getScore();
	oss << setw(9) << "Level: " << getLevel(); 
	oss << setw(12) << "Cabbages: " << player->cabbagePct() << "%";
	oss << setw(13) << "Torpedoes: " << player->numTorpedoes();
	string s = oss.str();
	setGameStatText(s);

	//loop through the vector of actors so each actor can doSomething() for each tick
	for (int i = 0; i < vactors.size(); i++)
	{
		if (!vactors.at(i)->isDead())
		{
			vactors.at(i)->doSomething();
		}

		if (vactors[i]->isDead())
		{
			if (vactors[i]->getType() == ALIEN)
				currentAliens--;
			delete vactors[i];
			vector<Actor*>::iterator p = vactors.begin() + i;
			vactors.erase(p);
		}
	}

	//if player is alive doSomething()
	if (!player->isDead())
	{
		player->doSomething();
	}

	//if player died go to death screen
	if (player->isDead())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	//summoning aliens
	if (currentAliens < maximumAliens && currentAliens < remainingAliens)
	{
		int shipSummon = randInt(1, S);

		if (shipSummon <= S1 && currentAliens < maximumAliens)
		{
			addActor(new Smallgon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1)));
			currentAliens++;
		}

		else if (shipSummon > S1 && shipSummon <= (S1 + S2) && currentAliens < maximumAliens)
		{
			addActor(new Smoregon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1)));
			currentAliens++;
		}

		else if (shipSummon >(S1 + S2) && currentAliens < maximumAliens)
		{
			addActor(new Snagglegon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1)));
			currentAliens++;
		}
	}

	if (remainingAliens <= 0)
		return GWSTATUS_FINISHED_LEVEL;
	
	//summoning stars
	if (randInt(1, 100) <= 15)
	{
		addActor(new Star(IID_STAR, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), 0, (randInt(5, 50) / 100.0), 3, this));
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (int i = vactors.size() - 1; i >= 0 ; i--)
	{
		delete vactors.at(i);
		vactors.pop_back();
	}

	delete player;
	player = nullptr;
}

Actor* StudentWorld::getOneCollidingAlien(const Actor* a) const
{
	for (int i = 0; i < vactors.size(); i++)
	{
		if (vactors[i]->getType() == ALIEN)
		{
			if (euclidianDist(vactors[i]->getX(), vactors[i]->getY(), a->getX(), a->getY()) < (0.75 * (vactors[i]->getRadius() + a->getRadius())))
				return vactors[i];
		}
	}

	return nullptr;
}

NachenBlaster* StudentWorld::getCollidingPlayer(const Actor* a) const
{
	if (euclidianDist(player->getX(), player->getY(), a->getX(), a->getY()) < (0.75 * (player->getRadius() + a->getRadius())))
		return player;

	return nullptr;
}

bool StudentWorld::playerInLineOfFire(const Actor* a) const
{
	return (player->getX() < a->getX() && (player->getY() >= a->getY() - 4 && player->getY() <= a->getY() + 4));
}

void StudentWorld::addActor(Actor* a)
{
	vactors.push_back(a);
}

void StudentWorld::recordAlienDestroyed()
{
	destroyedAliens++;
	remainingAliens--;
}