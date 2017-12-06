#include <cmath>
#include "drawable.h"
#include "player.h"

class CollisionStrategy {
public:
  virtual bool execute(const Drawable&, const Drawable&) const = 0;
  virtual bool executeBorder(const Drawable&, const Drawable&) const {return false;};
  virtual bool executeWeapon(const Player&, const Drawable&) const {return false;};
  virtual void draw() const = 0;
  virtual ~CollisionStrategy() {}
};

class RectangularCollisionStrategy : public CollisionStrategy {
public:
  RectangularCollisionStrategy() {}
  virtual bool execute(const Drawable&, const Drawable&) const;
  virtual void draw() const;
};

class MidPointCollisionStrategy : public CollisionStrategy {
public:
  MidPointCollisionStrategy() {}
  virtual bool execute(const Drawable&, const Drawable&) const;
  virtual void draw() const;
  float distance(float, float, float, float) const;
};

class PerPixelCollisionStrategy : public CollisionStrategy {
public:
  PerPixelCollisionStrategy() {}
  virtual bool execute(const Drawable&, const Drawable&) const;
  virtual bool executeWeapon(const Player& obj1, const Drawable& obj2) const;
  virtual bool executeBorder(const Drawable&, const Drawable&) const;
  virtual void draw() const;
private:
  bool isVisible(Uint32, SDL_Surface*) const;
};
