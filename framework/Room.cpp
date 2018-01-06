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
  enemies(),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  borderWidth(Gamedata::getInstance().getXmlInt(name+"/borderWidth")),
  borderHeight(Gamedata::getInstance().getXmlInt(name+"/borderHeight"))
{
  setScale(Gamedata::getInstance().getXmlInt(name+"/scale"));
  if(Gamedata::getInstance().checkTag(name+"/doorN/transparency")){
    doors[doorPlace::N]= RenderContext::getInstance()->getImage(name+"/doorN");
  }
  if(Gamedata::getInstance().checkTag(name+"/doorS/transparency")){
    doors[doorPlace::S]= RenderContext::getInstance()->getImage(name+"/doorS");
  }
  if(Gamedata::getInstance().checkTag(name+"/doorE/transparency")){
    doors[doorPlace::E]= RenderContext::getInstance()->getImage(name+"/doorE");
  }
  if(Gamedata::getInstance().checkTag(name+"/doorW/transparency")){
    doors[doorPlace::W]= RenderContext::getInstance()->getImage(name+"/doorW");
  }

  unsigned int enemyNum = 1;

  while(Gamedata::getInstance().checkTag(name+"/enemy"+std::to_string(enemyNum)+"/type")){
    std::string type = Gamedata::getInstance().getXmlStr(name+"/enemy"+std::to_string(enemyNum)+"/type");
    if(type == "spiderEnemy"){
      Gamedata::getInstance().getXmlInt(name+"/enemy"+std::to_string(enemyNum)+"/type");
      enemies.push_back(new spiderEnemy(name+"/enemy"+std::to_string(enemyNum)));
    } else {
      std::cout<<"Invalid Enemy Type in xml"<<std::endl;
      exit(1);
    }
    enemyNum++;
  }

}

Room::Room(const Room& s) :
  Drawable(s),
  image(s.image),
  border(s.border),
  doors(s.doors),
  enemies(s.enemies),
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
  //draw base image of room
  image->draw(getX(), getY(),getScale());
  //draw border to be used in collisions
  border->draw(getX(),getY(),getScale());

  //check for and draw each door present @TODO refactor to optimize
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

  //draw all enemies
  //@TODO sort enemies by y pos
  for(auto enemy:enemies){
    enemy->draw();
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
    loc[1] = getY()+getScaledHeight()/2-doors.at(doorPlace::W)->getHeight()*getScale()/2;
  }
  return loc;
}

std::vector<spiderEnemy*> Room::getEnemies(){
  return enemies;
}

void Room::checkforWeaponCollisions(){
//check for hits with other actors
  if(playerCharacter->getAnimIndex()!= playerAnim::ATTACKLEFT && playerCharacter->getAnimIndex()!=playerAnim::ATTACKRIGHT) return;
  for ( Drawable* d : enemies ) {
    if (PerPixelCollisionStrategy::instance().executeWeapon(*playerCharacter, *d) ) {
      d->collided(true);
      static_cast<spiderEnemy*>(d)->explode();
      static_cast<spiderEnemy*>(d)->reset();
    } else{
      d->collided(false);
    }
  }
}

void Room::checkForCollisions() {
  for ( Drawable* d : enemies ) {
    //don't check for collision on player if attacking
    if(playerCharacter->getAnimIndex()== playerAnim::ATTACKLEFT && playerCharacter->getAnimIndex()==playerAnim::ATTACKRIGHT) return;

    if (playerCharacter !=d && PerPixelCollisionStrategy::instance().execute(*playerCharacter, *d) ) {
      playerCharacter->collided(true);
      playerCharacter->explode();
      //@TODO reset room tracker
    } else{
    playerCharacter->collided(false);
    }
  }
}

void Room::checkBorderCollisions(){
  //check for shadow collisions
    for ( Drawable* d : enemies ) {
      if (PerPixelCollisionStrategy::instance().executeBorder(*this, *d) ) {
        d->shadowCollided(true);
      } else {
        d->shadowCollided(false);
      }
    }
  }

  void Room::checkDoors(){
      for ( auto door : doors ) {
        int scaledHeight= getDoorWidth(door.first);
        int scaledWidth= getDoorHeight(door.first);
        Vector2f loc = getDoorloc(door.first);
        if (MidPointCollisionStrategy::instance().executeImage(*playerCharacter, *door.second,scaledWidth,scaledHeight,loc) ) {
          //@TODO set logic to move player to another room
        } else {
        }
      }
    }

void Room::update(Uint32 ticks) {
  for(auto enemy : enemies){
    enemy->update(ticks);
  }
}

Room::~Room(){
  for(auto enemy:enemies){
    delete enemy;
  }
}
