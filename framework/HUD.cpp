#include "HUD.h"

HUD::HUD(SDL_Renderer *r) :
screenWidth(Gamedata::getInstance().getXmlInt("view/width")),
screenHeight(Gamedata::getInstance().getXmlInt("view/height")),
writeToScreen(false),
renderer(r)
{ }

void HUD::draw() const{
  if(writeToScreen || Clock::getInstance().getSeconds()<4){
    int r = Gamedata::getInstance().getXmlInt("HUD/red");
    int g = Gamedata::getInstance().getXmlInt("HUD/green");
    int b = Gamedata::getInstance().getXmlInt("HUD/blue");
    int a = Gamedata::getInstance().getXmlInt("HUD/alpha");
    int hudheight = Gamedata::getInstance().getXmlInt("HUD/height");
    int hudwidth = Gamedata::getInstance().getXmlInt("HUD/width");
    int linespace = Gamedata::getInstance().getXmlInt("HUD/linespace");
    SDL_Rect rect = {screenWidth-hudwidth, 0, hudwidth, hudheight};
    SDL_SetRenderDrawColor( renderer, r, g, b, a );
    SDL_RenderFillRect( renderer, &rect );
    IOmod::getInstance().writeText(Gamedata::getInstance().getXmlStr("HUD/title"),screenWidth-hudwidth,0);
    IOmod::getInstance().writeText(Gamedata::getInstance().getXmlStr("HUD/line1"),screenWidth-hudwidth,linespace);
    IOmod::getInstance().writeText(Gamedata::getInstance().getXmlStr("HUD/line2"),screenWidth-hudwidth,linespace*2);
    IOmod::getInstance().writeText(Gamedata::getInstance().getXmlStr("HUD/line3"),screenWidth-hudwidth,linespace*3);

 }
}

void HUD::toggle(){
  if(writeToScreen) writeToScreen = false;
  else writeToScreen = true;
}
