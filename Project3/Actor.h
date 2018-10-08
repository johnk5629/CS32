#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

const int HIT_BY_SHIP = 0;
const int HIT_BY_PROJECTILE = 1;

const int OTHER = 0;
const int PLAYER = 1;
const int ALIEN = 2;
const int PROJECTILE = 3;
const int GOODIE = 4;


class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
  public:
	  Actor(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* w);
	  virtual void doSomething() = 0;
	  virtual void sufferDamage(double amt, int cause);
	  bool isDead() const;
	  StudentWorld* getWorld() const;
	  void setAsDead();
	  virtual int getType() const { return OTHER; };

  private:
	  bool m_dead;
	  StudentWorld* m_currentWorld;
};

//Actors that cannot be classified into more subgroups

class Star : public Actor
{
  public:
	  Star(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* w);
	  virtual void doSomething();

  private:

};

class Explosion : public Actor
{
  public:
	  Explosion(StudentWorld* w, double startX, double startY);
	  virtual void doSomething();

  private:
	  int m_ticksRemaining;
};

//start of DamagableObjects

class DamageableObject : public Actor
{
  public:
	  DamageableObject(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, double health, StudentWorld* w);
	  virtual void doSomething() = 0;

	  // How many hit points does this actor have left?
	  double hitPoints() const;

	  // Increase this actor's hit points by amt.
	  void increaseHitPoints(double amt);

	  // This actor suffers an amount of damage caused by being hit by either
	  // a ship or a projectile (see constants above).
	  virtual void sufferDamage(double amt, int cause);

  private:
	  double m_hp;
	  double m_maxHP;
};

class NachenBlaster : public DamageableObject
{
  public:
	  NachenBlaster(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* w);
	  virtual void doSomething();
	  virtual int getType() const { return PLAYER; };

	  // Incease the number of torpedoes the player has by amt.
	  void increaseTorpedoes(int amt);

	  // Get the player's health percentage.
	  double healthPct() const;

	  // Get the player's cabbage energy percentage.
	  double cabbagePct() const;

	  // Get the number of torpedoes the player has.
	  int numTorpedoes() const;

  private:
	  bool canMoveHere(double x, double y);
	  int m_cabbagePoints;
	  int m_numTorpedoes;
	  
};

//start of class of Aliens

class Alien : public DamageableObject
{
  public:
	  Alien(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, double health, double travelSpeed, StudentWorld* w, double damageAmt, double deltaX,
		  double deltaY, unsigned int scoreValue);
	  void doSomething();
	  void sufferDamage(double amt, int cause);
	  virtual void flightPlanChecks();
	  virtual bool withinBoundsBehavior() = 0;

	  //reaction of alien when collided with player
	  void collisionReaction();


	  // Move the player by the current speed in the direction indicated
	  // by the x and y deltas.
	  virtual void move();

	  double getDeltaY() const;

	  // Set the player's y direction.
	  void setDeltaY(double dy);

	  // Set the player's speed.
	  void setSpeed(double speed);

	  // Set the flightsplan length
	  void setFlightPlanLength(int fl);

	  // If this alien drops goodies, drop one with the appropriate probability.
	  virtual void possiblyDropGoodie();

  private:
	  double m_tSpeed;
	  int m_flightPlanLength;
	  double m_damageAmt;
	  double m_deltaX;
	  double m_deltaY;
	  int m_scoreValue;
};

class Smallgon : public Alien
{
  public:
	  Smallgon(StudentWorld* w, double startX, double startY);
	  virtual bool withinBoundsBehavior();
	  virtual int getType() const { return ALIEN; };

  private:

};

class Smoregon : public Alien
{
  public:
	  Smoregon(StudentWorld* w, double startX, double startY);
	  virtual bool withinBoundsBehavior();
	  virtual void possiblyDropGoodie();
	  virtual int getType() const { return ALIEN; };

  private:

};

class Snagglegon : public Alien
{
  public:
	  Snagglegon(StudentWorld* w, double startX, double startY);
	  virtual bool withinBoundsBehavior();
	  virtual void possiblyDropGoodie();
	  virtual void move();
	  virtual void flightPlanChecks();
	  virtual int getType() const { return ALIEN; };

  private:

};

//end of Aliens
//end of DamageableObjects

//start of Projectiles

class Projectile : public Actor
{
  public:
	  Projectile(StudentWorld* w, double startX, double startY, int imageID, double damageAmt, double deltaX, bool rotates, int imageDir);
	  void doSomething();
	  virtual void collisionBehavior() = 0;
	  double getDeltaX() const;
	  bool doesRotate() const;

  private:
	  double m_projectileDamage;
	  double m_deltaX;
	  bool m_rotates;

};

class Cabbage : public Projectile
{
  public:
	  Cabbage(StudentWorld* w, double startX, double startY);
	  virtual void collisionBehavior();
	  virtual int getType() const { return PROJECTILE; };

  private:

};

class Turnip : public Projectile
{
  public:
	  Turnip(StudentWorld* w, double startX, double startY);
	  virtual void collisionBehavior();
	  virtual int getType() const { return PROJECTILE; };

  private:

};

//start of Torpedo classes
class Torpedo : public Projectile
{
  public:
	  Torpedo(StudentWorld* w, double startX, double startY, double deltaX, int imageDir);
	  virtual int getType() const { return PROJECTILE; };

  private:

};

class PlayerLaunchedTorpedo : public Torpedo
{
  public:
	  PlayerLaunchedTorpedo(StudentWorld* w, double startX, double startY);
	  virtual void collisionBehavior();

  private:

};

class AlienLaunchedTorpedo : public Torpedo
{
  public:
	  AlienLaunchedTorpedo(StudentWorld* w, double startX, double startY);
	  virtual void collisionBehavior();

  private:

};

//end of Torpedos
//end of Projectiles

//start of Goodies

class Goodie : public Actor
{
  public:
	  Goodie(StudentWorld* w, double startX, double startY, int imageID);
	  virtual void doSomething();
	  virtual void grantSpecificReward(NachenBlaster* n) = 0;

  private:

};

class ExtraLifeGoodie : public Goodie
{
  public:
	  ExtraLifeGoodie(StudentWorld* w, double startX, double startY);
	  virtual void grantSpecificReward(NachenBlaster* n);
	  virtual int getType() const { return GOODIE; };

  private:

};

class RepairGoodie : public Goodie
{
  public:
	  RepairGoodie(StudentWorld* w, double startX, double startY);
	  virtual void grantSpecificReward(NachenBlaster* n);
	  virtual int getType() const { return GOODIE; };

  private:

};

class TorpedoGoodie : public Goodie
{
  public:
	  TorpedoGoodie(StudentWorld* w, double startX, double startY);
	  virtual void grantSpecificReward(NachenBlaster* n);
	  virtual int getType() const { return GOODIE; };

  private:

};

//end of Goodies


#endif // ACTOR_H_
