#include "HUD.h"

HUD::HUD(SDL_Renderer *r) :
screenWidth(Gamedata::getInstance().getXmlInt("view/width")),
screenHeight(Gamedata::getInstance().getXmlInt("view/height")),
writeToScreen(false),
renderer(r),
red(Gamedata::getInstance().getXmlInt("HUD/red")),
green(Gamedata::getInstance().getXmlInt("HUD/green")),
blue(Gamedata::getInstance().getXmlInt("HUD/blue")),
alpha(Gamedata::getInstance().getXmlInt("HUD/alpha")),
hudheight(Gamedata::getInstance().getXmlInt("HUD/height")),
hudwidth(Gamedata::getInstance().getXmlInt("HUD/width")),
linespace(Gamedata::getInstance().getXmlInt("HUD/linespace")),
title(Gamedata::getInstance().getXmlStr("HUD/title")),
line1(Gamedata::getInstance().getXmlStr("HUD/line1")),
line2(Gamedata::getInstance().getXmlStr("HUD/line2")),
line3(Gamedata::getInstance().getXmlStr("HUD/line3")),
io(IOmod::getInstance())

{ }

void HUD::draw() const{
  if(writeToScreen || Clock::getInstance().getSeconds()<4){
    SDL_Rect rect = {screenWidth-hudwidth, 0, hudwidth, hudheight};
    SDL_SetRenderDrawColor( renderer, red, green, blue, alpha );
    SDL_RenderFillRect( renderer, &rect );
    io.writeText(title,screenWidth-hudwidth,0);
    io.writeText(line1,screenWidth-hudwidth,linespace);
    io.writeText(line2,screenWidth-hudwidth,linespace*2);
    io.writeText(line3,screenWidth-hudwidth,linespace*3);

 }
}

void HUD::toggle(){
  if(writeToScreen) writeToScreen = false;
  else writeToScreen = true;
}
