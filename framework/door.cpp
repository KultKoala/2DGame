#include "door.h"

door::door(const std::string& name, doorPlace place) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")
                  ),
           Vector2f(0,0)
           ),
  
