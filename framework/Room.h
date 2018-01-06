#ifndef ROOM__H
#define ROOM__H
#include <string>
#include <list>
#include "drawable.h"
#include <map>
#include <vector>
#include "spiderEnemy.h"
#include "player.h"

class Player;

#include "collisionStrategy.h"

class spiderEnemy;

class Room : public Drawable {
public:
  Room(const std::string&);
  Room(const Room&);
  virtual ~Room();
  Room& operator=(const Room&);

  virtual void draw() const;
  std::vector<spiderEnemy*> getEnemies();
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const {
    return image->getSurface();
  }

  virtual const Image* getBorder() const { return border;}
  const SDL_Surface* getBorderSurface () const {
    return border->getSurface();
  }


  int getScaledWidth()  const { return getScale()*image->getWidth();  }
  int getScaledHeight() const { return getScale()*image->getHeight(); }

  const std::map<doorPlace,Image*> getDoors();
  int getDoorWidth(doorPlace)const;
  int getDoorHeight (doorPlace)const;
  Vector2f getDoorloc(doorPlace)const;
  void checkForCollisions();
  void checkforWeaponCollisions();
  void checkBorderCollisions();
  void checkDoors();

private:
  const Image * image;
  const Image * border;
  std::map<doorPlace, Image*> doors;
  std::vector<spiderEnemy*> enemies;
  Player *playerCharacter;
protected:
  int worldWidth;
  int worldHeight;
  int borderWidth;
  int borderHeight;

  int getDistance(const Room*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif
