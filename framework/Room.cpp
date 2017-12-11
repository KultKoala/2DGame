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
  doors(),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{
  setScale(Gamedata::getInstance().getXmlInt(name+"/scale"));
  if(Gamedata::getInstance().getXmlInt(name+"/doorN/presence")){
    doors[doorPlace::N]= RenderContext::getInstance()->getImage(name+"/doorN");
  }
  if(Gamedata::getInstance().getXmlInt(name+"/doorS/presence")){
    doors[doorPlace::S]= RenderContext::getInstance()->getImage(name+"/doorS");
  }
  if(Gamedata::getInstance().getXmlInt(name+"/doorE/presence")){
    doors[doorPlace::E]= RenderContext::getInstance()->getImage(name+"/doorE");
  }
  if(Gamedata::getInstance().getXmlInt(name+"/doorW/presence")){
    doors[doorPlace::W]= RenderContext::getInstance()->getImage(name+"/doorW");
  }

}

Room::Room(const Room& s) :
  Drawable(s),
  image(s.image),
  border(s.border),
  doors(s.doors),
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
  if(doors.find(doorPlace::N)!=doors.end()){
    doors.at(doorPlace::N)->draw(getX()+getScaledWidth()/2,getY(),getScale());
  }

  if(doors.find(doorPlace::E)!=doors.end()){
    doors.at(doorPlace::E)->draw(getX(),getY()+getScaledHeight()/2,getScale());
  }

  if(doors.find(doorPlace::W)!=doors.end()){
    int doorWidth = doors.at(doorPlace::W)->getWidth()*getScale();
    doors.at(doorPlace::W)->draw(getX(),getY()+getScaledHeight()/2,getScale());
  }

  if(doors.find(doorPlace::S)!=doors.end()){
    int doorHeight = doors.at(doorPlace::S)->getHeight()*getScale();
    doors.at(doorPlace::S)->draw(getX()+getScaledWidth()/2-doorHeight,getY()-getScaledHeight(),getScale());
  }

}

void Room::update(Uint32 ticks) {
}
