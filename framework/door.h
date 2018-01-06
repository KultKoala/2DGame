#ifndef DOOR__H
#define DOOR__H

#include "image.h"
#include "room.h"

class door: public Drawable{
public:
  door(doorPlace,Room*);
  void open();
  void close();
  Room* getleadsTo();
  doorPlace getdoorPlace();
private:
  std::vector <Image*> close;
  std::vector <Image*> open;
  Room* leadsTo;
  doorPlace place;
};

#endif
