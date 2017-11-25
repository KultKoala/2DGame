#include <cmath>
#include <random>
#include <functional>
#include "Room.h"
#include "gamedata.h"
#include "renderContext.h"

Room::Room(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")
                  ),
           Vector2f(0,0)
           ),
  image( RenderContext::getInstance()->getImage(name) ),
  border( RenderContext::getInstance()->getImage(name+"/border") ),

  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{setScale(Gamedata::getInstance().getXmlInt(name+"/scale")); }

Room::Room(const Room& s) :
  Drawable(s),
  image(s.image),
  border(s.border),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{}

Room& Room::operator=(const Room& rhs) {
  Drawable::operator=( rhs );
  image = rhs.image;
  border = rhs.border;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void Room::draw() const {
  if(getScale() < SCALE_EPSILON) return;
  image->draw(getX(), getY(),getScale());
  border->draw(getX(),getY(),getScale());
}

void Room::update(Uint32 ticks) {
}
