#ifndef ROOM__H
#define ROOM__H
#include <string>
#include <list>
#include "drawable.h"
#include <map>

enum struct doorPlace {NONE,N,S,W,E};

class Room : public Drawable {
public:
  Room(const std::string&);
  Room(const Room&);
  virtual ~Room() { }
  Room& operator=(const Room&);

  virtual void draw() const;
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

private:
  const Image * image;
  const Image * border;
  std::map<doorPlace, Image*> doors;


protected:
  int worldWidth;
  int worldHeight;

  int getDistance(const Room*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif
