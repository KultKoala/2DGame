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
  actors.push_back(playerCharacter);
  actors.push_back(new spiderEnemy("spiderEnemy", playerCharacter->getPosition(),playerCharacter->getScaledWidth(),playerCharacter->getScaledHeight()));
  playerCharacter->attach(dynamic_cast<spiderEnemy*>(actors[1]));
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
  //username in lower lefthand corner
  io.writeText("Landon Byrd", 5, Gamedata::getInstance().getXmlInt("view/height")-25);
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  checkforWeaponCollisions();
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

void Engine::checkforWeaponCollisions(){
  shadowcoll = false;
//check for hits with other actors
  if(playerCharacter->getAnimIndex()!= playerAnim::ATTACKLEFT && playerCharacter->getAnimIndex()!=playerAnim::ATTACKRIGHT) return;
  for ( Drawable* d : actors ) {
    if (playerCharacter !=d && strategy->executeWeapon(*playerCharacter, *d) ) {
      d->collided(true);
      shadowcoll=true;
      static_cast<spiderEnemy*>(d)->explode();
    } else{
      d->collided(false);
    }
  }
}

void Engine::checkForCollisions() {
  coll = false;
//check for hits with other actors
  for ( Drawable* d : actors ) {
    if (playerCharacter !=d && strategy->execute(*playerCharacter, *d) ) {
      coll = true;
      playerCharacter->collided(true);
      playerCharacter->explode();
      playerCharacter->reset();
    } else{
    playerCharacter->collided(false);
    }
  }
}

void Engine::checkBorderCollisions(){
  //check for shadow collisions
    for ( Drawable* d : actors ) {
      if (strategy->executeBorder(*rooms[currentRoom], *d) ) {
        d->shadowCollided(true);
      } else {
        d->shadowCollided(false);
      }
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
