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
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  borderWidth(Gamedata::getInstance().getXmlInt(name+"/borderWidth")),
  borderHeight(Gamedata::getInstance().getXmlInt(name+"/borderHeight"))
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
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  borderWidth(s.borderWidth),
  borderHeight(s.borderHeight)
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
    int x = getX() -doors.at(doorPlace::N)->getWidth()*getScale()/2+getScaledWidth()/2;
    int y = getY() +borderHeight*getScale() -  doors.at(doorPlace::N)->getHeight()*getScale()+2*getScale();
    doors.at(doorPlace::N)->draw(x,y,getScale());
  }

  if(doors.find(doorPlace::E)!=doors.end()){
    int x = getX() - doors.at(doorPlace::E)->getWidth()*getScale()+getScaledWidth();
    int y= getY()+getScaledHeight()/2-doors.at(doorPlace::E)->getHeight()*getScale()/2;
    doors.at(doorPlace::E)->draw(x,y,getScale());
  }

  if(doors.find(doorPlace::W)!=doors.end()){
    Vector2f loc =getDoorloc(doorPlace::W);

    doors.at(doorPlace::W)->draw(loc[0],loc[1],getScale());
  }

  if(doors.find(doorPlace::S)!=doors.end()){
    int x = getX() -doors.at(doorPlace::S)->getWidth()*getScale()/2+getScaledWidth()/2;
    int y = getY() + getScaledHeight()- borderHeight*getScale()-4*getScale();
    doors.at(doorPlace::S)->draw(x,y,getScale());
  }

}

const std::map<doorPlace,Image*> Room::getDoors(){
  return doors;
}

int Room::getDoorWidth(doorPlace p) const{
  return doors.at(p)->getWidth()*getScale();
}

int Room::getDoorHeight(doorPlace p)const{
  return doors.at(p)->getHeight()*getScale();
}

Vector2f Room::getDoorloc(doorPlace p) const{
  Vector2f loc;
  if(p ==doorPlace::S){
    loc = Vector2f(getX() -doors.at(doorPlace::S)->getWidth()*getScale()/2+getScaledWidth()/2 , getY() + getScaledHeight()- borderHeight*getScale()-4*getScale());
  }

  if(p ==doorPlace::N){
    loc[0] = getX() -doors.at(doorPlace::N)->getWidth()*getScale()/2+getScaledWidth()/2;
    loc[1] = getY() +borderHeight*getScale() -  doors.at(doorPlace::N)->getHeight()*getScale()+2*getScale();
  }

  if(p==doorPlace::E){
    loc= Vector2f(getX() - doors.at(doorPlace::E)->getWidth()*getScale()+getScaledWidth(),getY()+getScaledHeight()/2-doors.at(doorPlace::E)->getHeight()*getScale()/2);
  }

  if(p==doorPlace::W){
    loc[0] = getX();
    loc[1] = getY()+getScaledHeight()/2-doors.at(doorPlace::E)->getHeight()*getScale()/2;;
  }
  return loc;
}

void Room::update(Uint32 ticks) {
}
