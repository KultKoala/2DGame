#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "smartSprite.h"
#include <list>
#include "Room.h"

enum playerAnim{
  LEFTUP,RIGHTUP,RIGHTDOWN,LEFTDOWN,ROLLLEFT,ROLLRIGHT,
  ATTACKLEFT,ATTACKRIGHT
};

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);


  void setCurrentRoom(Room *);

  virtual const Image* getImage() const {
    if(rolling) return rollLeftAnim[currentFrame];

    if(movingLeft && movingDown) return leftDownAnim[currentFrame];
    else if(!movingLeft && movingDown) return rightDownAnim[currentFrame];
    else if(!movingLeft && !movingDown) return rightUpAnim[currentFrame];
    else return leftUpAnim[currentFrame];
  }
  int getScaledWidth()  const {
    if(rolling) return rollLeftAnim[currentFrame]->getWidth()*getScale();

    if(movingLeft && movingDown) return leftDownAnim[currentFrame]->getWidth()*getScale();
    else if(!movingLeft && movingDown) return rightDownAnim[currentFrame]->getWidth()*getScale();
    else if(!movingLeft && !movingDown) return rightUpAnim[currentFrame]->getWidth()*getScale();
    else return leftUpAnim[currentFrame]->getWidth()*getScale();
  }

  int getScaledHeight()  const {
    if(rolling) return rollLeftAnim[currentFrame]->getHeight()*getScale();

    if(movingLeft && movingDown) return leftDownAnim[currentFrame]->getHeight()*getScale();
    else if(!movingLeft && movingDown) return rightDownAnim[currentFrame]->getHeight()*getScale();
    else if(!movingLeft && !movingDown) return rightUpAnim[currentFrame]->getHeight()*getScale();
    else return leftUpAnim[currentFrame]->getHeight()*getScale();
  }

  virtual const SDL_Surface* getSurface() const {
    if(rolling) return rollLeftAnim[currentFrame]->getSurface();

    if(movingLeft && movingDown) return leftDownAnim[currentFrame]->getSurface();
    else if(!movingLeft && movingDown) return rightDownAnim[currentFrame]->getSurface();
    else if(!movingLeft && !movingDown) return rightUpAnim[currentFrame]->getSurface();
    else return leftUpAnim[currentFrame]->getSurface();
  }

  virtual const SDL_Surface* getBorderSurface() const{
    return shadow->getSurface();
  }


  //setting animations
  void right();
  void left();
  void up();
  void down();
  void attack();
  void roll();
  void stop();

  void attach( SmartSprite* o ) { observers.push_back(o); }
  void detach( SmartSprite* o );



protected:
  std::list<SmartSprite*> observers;
  Room *currentRoom;

  //TODO: push all into one vector to be iterated though
  std::vector<Image *> leftUpAnim;
  std::vector<Image *> rightUpAnim;
  std::vector<Image *> rightDownAnim;
  std::vector<Image *> leftDownAnim;
  std::vector<Image *> rollLeftAnim;
  std::vector<Image *> rollRightAnim;
  std::vector<Image *>attackLeftAnim;
  std::vector<Image *>attackRightAnim;
  std::vector<Image *>shovelLeftAnim;
  std::vector<Image *>shovelRightAnim;

  Image * shadow;
  bool movingLeft;
  bool movingDown;
  bool stopped;
  bool rolling;
  bool attacking;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  Player& operator=(const Player&);
private:
  Vector2f initialVelocity;
  Vector2f previousVel;
};
#endif
