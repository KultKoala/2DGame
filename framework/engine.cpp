#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "collisionStrategy.h"
#include "Room.h"
#include "smartSprite.h"
#include "multisprite.h"

Engine::~Engine() {
  for(auto room:rooms){
    delete room;
  }

  for(auto actor : actors) {
    delete actor;
  }

  for(auto background:worldBackgrounds){
    delete background;
  }
  delete strategy;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  viewport( Viewport::getInstance() ),
  worldBackgrounds(),
  actors(),
  rooms(),
  currentRoom(0),
  playerCharacter(new Player("playerCharacter")),
  hud(renderer),
  strategy(new PerPixelCollisionStrategy),
  coll(false),
  shadowcoll(false),
  makeVideo( false )
{
  worldBackgrounds.push_back(new World("sky", Gamedata::getInstance().getXmlInt("sky/factor") ));
  worldBackgrounds.push_back(new World("mountains", Gamedata::getInstance().getXmlInt("mountains/factor") ));
  worldBackgrounds.push_back(new World("ground", Gamedata::getInstance().getXmlInt("ground/factor") ));
  // actors.push_back(new MultiSprite("Giants"));
  actors.push_back(playerCharacter);
  // actors.push_back(new SmartSprite("face", playerCharacter->getPosition(),playerCharacter->getScaledWidth(),playerCharacter->getScaledHeight()));
  // playerCharacter->attach(dynamic_cast<SmartSprite*>(actors[1]));
  rooms.push_back(new Room("SnowyCorridor"));
  rooms.push_back(new Room("DarkLogCabin"));
  Viewport::getInstance().setObjectToTrack(rooms[0]);
  playerCharacter->setCurrentRoom(rooms[0]);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {

  for(auto layer: worldBackgrounds){
    layer->draw();
  }

  for(auto room:rooms){
    room->draw();
  }

  for(auto actor : actors) {
    actor->draw();
  }
  hud.draw();

  std::stringstream s;
  s<<playerCharacter->getVelocityX()<<" "<<playerCharacter->getVelocityY();
  io.writeText(s.str(), 5, Gamedata::getInstance().getXmlInt("view/height")-300);
  //username in lower lefthand corner
  io.writeText("Landon Byrd", 5, Gamedata::getInstance().getXmlInt("view/height")-25);
  if(coll){
    io.writeText("Player is colliding with enemy", 5,  Gamedata::getInstance().getXmlInt("view/height")-50);
  }
  else{
    io.writeText("Player is not colliding with enemy", 5,  Gamedata::getInstance().getXmlInt("view/height")-50);
  }

  if(shadowcoll){
    io.writeText("Collision with a wall by actor", 5,Gamedata::getInstance().getXmlInt("view/height")-100);
  }
  else{
    io.writeText("No wall collisions", 5,Gamedata::getInstance().getXmlInt("view/height")-100);
  }
  std::stringstream fps;
  fps << "FPS " << clock.getFps();
  std::string fpsStr = fps.str();
  io.writeText(fpsStr, 5, Gamedata::getInstance().getXmlInt("view/height")-150);
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  checkBorderCollisions();
  for(auto world:worldBackgrounds){
    world->update();
  }
  for(auto actor: actors){
    actor->update(ticks);
  }

  viewport.update(); // always update viewport last
}

void Engine::checkForCollisions() {
  coll = false;
//check for hits with other actors
  for ( const Drawable* d : actors ) {
    if (playerCharacter !=d && strategy->execute(*playerCharacter, *d) ) {
      coll = true;
    }
    playerCharacter->collided(coll);
  }
}

void Engine::checkBorderCollisions(){

  //check for shadow collisions
  shadowcoll = false;
    for ( Drawable* d : actors ) {
      if (strategy->executeBorder(*rooms[currentRoom], *d) ) {
        shadowcoll = true;
      }
      d->shadowCollided(shadowcoll);
    }
  }

void Engine::switchSprite(){
  Viewport::getInstance().setObjectToTrack(playerCharacter);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]){
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_F1] ) {
          hud.toggle();
        }

        // if ( keystate[SDL_SCANCODE_T] ) {
        //   switchSprite();
        // }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
        if(!playerCharacter->getShadowCollided()){
          if (keystate[SDL_SCANCODE_SLASH]) {
            playerCharacter->roll();
          } else if (keystate[SDL_SCANCODE_PERIOD]) {
            playerCharacter->attack();
          }
        }
      }
    }

      // In this section of the event loop we allow key bounce:

      ticks = clock.getElapsedTicks();
      if ( ticks > 0) {
        clock.incrFrame();
        if (keystate[SDL_SCANCODE_A]) {
          playerCharacter->left();
        }
        if (keystate[SDL_SCANCODE_D]) {
          playerCharacter->right();
        }
        if (keystate[SDL_SCANCODE_W]) {
          playerCharacter->up();
        }
        if (keystate[SDL_SCANCODE_S]) {
          playerCharacter->down();
        }
      }

      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
