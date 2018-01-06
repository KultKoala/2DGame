#ifndef COLLISION__H
#define COLLISION__H

#include <cmath>
#include "drawable.h"
#include "player.h"

class CollisionStrategy {
public:
  virtual bool execute(const Drawable&, const Drawable&) const = 0;
  virtual bool executeBorder(const Drawable&, const Drawable&) const {return false;};
  virtual bool executeImage(const Player&, const Image&, int ,int, Vector2f) const {return false;};
  virtual bool executeWeapon(const Player&, const Drawable&) const {return false;};
  virtual void draw() const = 0;
  virtual ~CollisionStrategy() {}
};

class RectangularCollisionStrategy : public CollisionStrategy {
public:
  static RectangularCollisionStrategy& instance(){
  static RectangularCollisionStrategy *instance = new RectangularCollisionStrategy();
  return *instance;
}
  virtual bool execute(const Drawable&, const Drawable&) const;
  virtual void draw() const;
private:
    RectangularCollisionStrategy() {}
};

class MidPointCollisionStrategy : public CollisionStrategy {
public:
  static MidPointCollisionStrategy& instance(){
  static MidPointCollisionStrategy *instance = new MidPointCollisionStrategy();
  return *instance;
}
  virtual bool execute(const Drawable&, const Drawable&) const;
  virtual bool executeImage(const Player&, const Image& image, int,int, Vector2f) const;
  virtual void draw() const;
  float distance(float, float, float, float) const;
private:
    MidPointCollisionStrategy() {}
};

class PerPixelCollisionStrategy : public CollisionStrategy {
public:
  static PerPixelCollisionStrategy& instance(){
  static PerPixelCollisionStrategy *instance = new PerPixelCollisionStrategy();
  return *instance;
}
  virtual bool execute(const Drawable&, const Drawable&) const;
  virtual bool executeWeapon(const Player& obj1, const Drawable& obj2) const;
  virtual bool executeBorder(const Drawable&, const Drawable&) const;
  virtual void draw() const;
private:
  PerPixelCollisionStrategy() {}
  bool isVisible(Uint32, SDL_Surface*) const;
};

#endif
