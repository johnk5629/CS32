#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor
Actor::Actor(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* w)
	: GraphObject(imageID, startX, startY, dir, size, depth), m_dead(false), m_currentWorld(w)
{}

void Actor::sufferDamage(double amt, int cause)
{
	return;
}

StudentWorld* Actor::getWorld() const
{
	return m_currentWorld;
}

bool Actor::isDead() const
{
	return m_dead;
}

void Actor::setAsDead()
{
	m_dead = true;
}

//Star
Star::Star(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* w)
	: Actor(imageID, startX, startY, dir, size, depth, w)
{}

void Star::doSomething()
{
	moveTo(getX() - 1, getY());

	if (getX() < 0)
		setAsDead();
}

Explosion::Explosion(StudentWorld* w, double startX, double startY)
	: Actor(IID_EXPLOSION, startX, startY, 0, 1, 0, w), m_ticksRemaining(4)
{}

void Explosion::doSomething()
{
	if (isDead())
		return;

	setSize(getSize() * 1.5);

	m_ticksRemaining--;

	if (m_ticksRemaining <= 0)
		setAsDead();
}

//DamageableObject
DamageableObject::DamageableObject(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, double health, StudentWorld* w)
	: Actor(imageID, startX, startY, dir, size, depth, w), m_hp(health), m_maxHP(health)
{}

double DamageableObject::hitPoints() const
{
	return m_hp;
}

void DamageableObject::increaseHitPoints(double amt)
{
	if (amt > (m_maxHP - m_hp))
	{
		m_hp += (m_maxHP - m_hp);
	}
	else
	{
		m_hp += amt;
	}
}

void DamageableObject::sufferDamage(double amt, int cause)
{
	m_hp -= amt;

	if (cause == HIT_BY_SHIP)
		getWorld()->playSound(SOUND_DEATH);
	if (cause == HIT_BY_PROJECTILE)
		getWorld()->playSound(SOUND_BLAST);

	if (m_hp <= 0)
	{
		setAsDead();
		getWorld()->addActor(new Explosion(getWorld(), getX(), getY()));
		if (getType() == ALIEN)
			getWorld()->recordAlienDestroyed();
	}
}

//NachenBlaster
NachenBlaster::NachenBlaster(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* w)
	: DamageableObject(imageID, startX, startY, dir, size, depth, 50, w), m_cabbagePoints(30), m_numTorpedoes(0)
{}

bool NachenBlaster::canMoveHere(double x, double y)
{
	if (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT)
		return false;
	return true;
}

void NachenBlaster::doSomething()
{
	if (isDead())
		return;

	if (m_cabbagePoints < 30)
		m_cabbagePoints++;

	int ch;
	if (getWorld()->getKey(ch))
	{
		// user hit a key during this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			if (canMoveHere(getX() - 6, getY()))
				moveTo(getX() - 6, getY());
			break;
		case KEY_PRESS_RIGHT:
			if (canMoveHere(getX() + 6, getY()))
				moveTo(getX() + 6, getY());
			break;
		case KEY_PRESS_UP:
			if (canMoveHere(getX(), getY() + 6))
				moveTo(getX(), getY() + 6);
			break;
		case KEY_PRESS_DOWN:
			if (canMoveHere(getX(), getY() - 6))
				moveTo(getX(), getY() - 6);
			break;
		case KEY_PRESS_SPACE:
			if (m_cabbagePoints >= 5)
			{
				getWorld()->addActor(new Cabbage(getWorld(), getX() + 12, getY()));
				getWorld()->playSound(SOUND_PLAYER_SHOOT);
				m_cabbagePoints -= 5;
			}
			break;
		case KEY_PRESS_TAB:
			if (m_numTorpedoes > 0)
			{
				getWorld()->addActor(new PlayerLaunchedTorpedo(getWorld(), getX() + 12, getY()));
				getWorld()->playSound(SOUND_TORPEDO);
				m_numTorpedoes--;
			}
			break;
		default:
			break;
		}
	}
}

void NachenBlaster::increaseTorpedoes(int amt)
{
	m_numTorpedoes += amt;
}

double NachenBlaster::healthPct() const
{
	return (hitPoints() / 50.0) * 100.0;
}

double NachenBlaster::cabbagePct() const
{
	return (m_cabbagePoints / 30.0) * 100.0;
}

int NachenBlaster::numTorpedoes() const
{
	return m_numTorpedoes;
}

//Aliens

Alien::Alien(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, double health, double travelSpeed, StudentWorld* w, double damageAmt, double deltaX,
	double deltaY, unsigned int scoreValue)
	: DamageableObject(imageID, startX, startY, dir, size, depth, health, w), m_tSpeed(travelSpeed), m_damageAmt(damageAmt), m_deltaX(deltaX), m_deltaY(deltaY), m_scoreValue(scoreValue), m_flightPlanLength(0)
{}

void Alien::sufferDamage(double amt, int cause)
{
	DamageableObject::sufferDamage(amt, cause);
	if (hitPoints() <= 0)
	{
		getWorld()->increaseScore(m_scoreValue);
	}
}

void Alien::move()
{
	moveTo(getX() + (m_tSpeed * m_deltaX), getY() + (m_tSpeed * m_deltaY));
	m_flightPlanLength--;
}

void Alien::collisionReaction()
{
	if (getWorld()->getCollidingPlayer(this) != nullptr)
	{
		getWorld()->getCollidingPlayer(this)->sufferDamage(m_damageAmt, HIT_BY_SHIP);
		setAsDead();
		getWorld()->increaseScore(m_scoreValue);
		getWorld()->recordAlienDestroyed();
		getWorld()->addActor(new Explosion(getWorld(), getX(), getY()));
		possiblyDropGoodie();
	}
}

double Alien::getDeltaY() const
{
	return m_deltaY;
}

void Alien::setDeltaY(double dy)
{
	m_deltaY = dy;
}

void Alien::setSpeed(double speed)
{
	m_tSpeed = speed;
}

void Alien::setFlightPlanLength(int fl)
{
	m_flightPlanLength = fl;
}

void Alien::flightPlanChecks()
{
	if (m_flightPlanLength <= 0)
	{
		setDeltaY(randInt(-1, 1));
		m_flightPlanLength = randInt(1, 32);
	}
	else if (getY() <= 0)
	{
		setDeltaY(1);
		m_flightPlanLength = randInt(1, 32);
	}
	else if (getY() >= VIEW_HEIGHT - 1)
	{
		setDeltaY(-1);
		m_flightPlanLength = randInt(1, 32);
	}
}

void Alien::possiblyDropGoodie()
{
	return;
}

void Alien::doSomething()
{
	if (isDead())
		return;

	if (getX() < 0)
	{
		setAsDead();
		return;
	}

	//first check for collision
	collisionReaction();

	//if the alien died just return
	if (isDead())
		return;

	flightPlanChecks();

	if (getWorld()->playerInLineOfFire(this))
	{
		if (withinBoundsBehavior())
			return;		//if the withinBoundBehavior() went through, the doSomething() will return and not do anything for the rest of the tick
	}

	move();

	//second check for collision if it wasn't dead before movement
	collisionReaction();
}

//Smallgon

Smallgon::Smallgon(StudentWorld* w, double startX, double startY)
	: Alien(IID_SMALLGON, startX, startY, 0, 1.5, 1, 5 * (1 + (w->getLevel() - 1) * .1), 2.0, w, 5, -1.0, 0, 250)
{}

bool Smallgon::withinBoundsBehavior()
{
	int chance = randInt(1, (20 / getWorld()->getLevel()) + 5);

	if (chance == 1)
	{
		getWorld()->addActor(new Turnip(getWorld(), getX() - 14, getY()));
		getWorld()->playSound(SOUND_ALIEN_SHOOT);
		return true;
	}

	return false;
}

//Smoregon

Smoregon::Smoregon(StudentWorld* w, double startX, double startY)
	: Alien(IID_SMOREGON, startX, startY, 0, 1.5, 1, 5 * (1 + (w->getLevel() - 1) * .1), 2.0, w, 5, -1, 0, 250)
{}

bool Smoregon::withinBoundsBehavior()
{
	int chance = randInt(1, (20 / getWorld()->getLevel()) + 5);

	if (chance == 1)
	{
		getWorld()->addActor(new Turnip(getWorld(), getX() - 14, getY()));
		getWorld()->playSound(SOUND_ALIEN_SHOOT);
		return true;
	}

	if (chance == 2)
	{
		setDeltaY(0);
		setFlightPlanLength(VIEW_WIDTH);
		setSpeed(5);
		return false;
	}

	return false;
}

void Smoregon::possiblyDropGoodie()
{
	if (randInt(1, 3) == 1)
	{
		int chance = randInt(1, 2);
		if (chance == 1)
			getWorld()->addActor(new RepairGoodie(getWorld(), getX(), getY()));
		else
			getWorld()->addActor(new TorpedoGoodie(getWorld(), getX(), getY()));
	}
}

//Snagglegon

Snagglegon::Snagglegon(StudentWorld* w, double startX, double startY)
	: Alien(IID_SNAGGLEGON, startX, startY, 0, 1.5, 1, 10 * (1 + (w->getLevel() - 1) * .1), 1.75, w, 15, -1, -1, 1000)
{}

bool Snagglegon::withinBoundsBehavior()
{
	int chance = randInt(1, (15 / getWorld()->getLevel()) + 10);

	if (chance == 1)
	{
		getWorld()->addActor(new AlienLaunchedTorpedo(getWorld(), getX() - 14, getY()));
		getWorld()->playSound(SOUND_TORPEDO);
		return true;
	}

	return false;
}

void Snagglegon::possiblyDropGoodie()
{
	if (randInt(1, 6) == 1)
	{
		getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY()));
	}
}

void Snagglegon::flightPlanChecks()
{
	if (getY() <= 0)
	{
		setDeltaY(1);
	}
	else if (getY() >= VIEW_HEIGHT - 1)
	{
		setDeltaY(-1);
	}
}

void Snagglegon::move()
{
	moveTo(getX() + (1.75 * -1), getY() + (1.75 * getDeltaY()));
}

//Projectiles

Projectile::Projectile(StudentWorld* w, double startX, double startY, int imageID, double damageAmt, double deltaX, bool rotates, int imageDir)
	: Actor(imageID, startX, startY, imageDir, 0.5, 1, w), m_projectileDamage(damageAmt), m_rotates(rotates), m_deltaX(deltaX)
{}

double Projectile::getDeltaX() const
{
	return m_deltaX;
}

bool Projectile::doesRotate() const
{
	return m_rotates;
}

void Projectile::doSomething()
{
	if (isDead())
		return;

	//projectiles from player
	if (getDeltaX() > 0)
	{
		if (getX() >= VIEW_WIDTH)
		{
			setAsDead();
			return;
		}
	}

	//projectiles from aliens
	if (getDeltaX() < 0)
	{
		if (getX() < 0)
		{
			setAsDead();
			return;
		}
	}

	collisionBehavior();

	//check death after potential collision
	if (isDead())
		return;

	moveTo(getX() + getDeltaX(), getY());

	if (doesRotate())
	{
		setDirection(getDirection() + 20);
	}

	collisionBehavior();		//check for collision again after movement
}

//Cabbage

Cabbage::Cabbage(StudentWorld* w, double startX, double startY)
	: Projectile(w, startX, startY, IID_CABBAGE, 2, 8, true, 0)
{}

void Cabbage::collisionBehavior()
{
	if (getWorld()->getOneCollidingAlien(this) != nullptr)
	{
		getWorld()->getOneCollidingAlien(this)->sufferDamage(2, HIT_BY_PROJECTILE);
		setAsDead();
	}
}

//Turnip

Turnip::Turnip(StudentWorld* w, double startX, double startY)
	: Projectile(w, startX, startY, IID_TURNIP, 2, -6, true, 0)
{}

void Turnip::collisionBehavior()
{
	if (getWorld()->getCollidingPlayer(this) != nullptr)
	{
		getWorld()->getCollidingPlayer(this)->sufferDamage(2, HIT_BY_PROJECTILE);
		setAsDead();
	}
}

//Torpedo

Torpedo::Torpedo(StudentWorld* w, double startX, double startY, double deltaX, int imageDir)
	: Projectile(w, startX, startY, IID_TORPEDO, 8, deltaX, false, imageDir)
{}

// PlayerLaunchedTorpedo

PlayerLaunchedTorpedo::PlayerLaunchedTorpedo(StudentWorld* w, double startX, double startY)
	: Torpedo(w, startX, startY, 8, 0)
{}

void PlayerLaunchedTorpedo::collisionBehavior()
{
	if (getWorld()->getOneCollidingAlien(this) != nullptr)
	{
		getWorld()->getOneCollidingAlien(this)->sufferDamage(8, HIT_BY_PROJECTILE);
		setAsDead();
	}
}

//AlienLaunchedTorpedo

AlienLaunchedTorpedo::AlienLaunchedTorpedo(StudentWorld* w, double startX, double startY)
	: Torpedo(w, startX, startY, -8, 180)
{}

void AlienLaunchedTorpedo::collisionBehavior()
{
	if (getWorld()->getCollidingPlayer(this) != nullptr)
	{
		getWorld()->getCollidingPlayer(this)->sufferDamage(8, HIT_BY_PROJECTILE);
		setAsDead();
	}
}


//Goodies

Goodie::Goodie(StudentWorld* w, double startX, double startY, int imageID)
	: Actor(imageID, startX, startY, 0, 0.5, 1, w)
{}

void Goodie::doSomething()
{
	if (isDead())
		return;

	if (getX() < 0 || getY() < 0)
	{
		setAsDead();
		return;
	}

	if (getWorld()->getCollidingPlayer(this) != nullptr)
	{
		grantSpecificReward(getWorld()->getCollidingPlayer(this));
		setAsDead();
		getWorld()->playSound(SOUND_GOODIE);
		return;
	}
	
	moveTo(getX() - 0.75, getY() - 0.75);

	if (getWorld()->getCollidingPlayer(this) != nullptr)
	{
		grantSpecificReward(getWorld()->getCollidingPlayer(this));
		setAsDead();
		getWorld()->playSound(SOUND_GOODIE);
		return;
	}
}

//ExtraLifeGoodie

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* w, double startX, double startY)
	: Goodie(w, startX, startY, IID_LIFE_GOODIE)
{}

void ExtraLifeGoodie::grantSpecificReward(NachenBlaster* n)
{
	n->getWorld()->increaseScore(100);
	n->getWorld()->incLives();
}

//RepairGoodie

RepairGoodie::RepairGoodie(StudentWorld* w, double startX, double startY)
	: Goodie(w, startX, startY, IID_REPAIR_GOODIE)
{}

void RepairGoodie::grantSpecificReward(NachenBlaster* n)
{
	n->getWorld()->increaseScore(100);
	n->increaseHitPoints(10);
}

//TorpedoGoodie

TorpedoGoodie::TorpedoGoodie(StudentWorld* w, double startX, double startY)
	: Goodie(w, startX, startY, IID_TORPEDO_GOODIE)
{}

void TorpedoGoodie::grantSpecificReward(NachenBlaster* n)
{
	n->getWorld()->increaseScore(100);
	n->increaseTorpedoes(5);
}