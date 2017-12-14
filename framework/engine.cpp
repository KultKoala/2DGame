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
#include "sound.h"

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
  currentRoom(-1),
  playerCharacter(new Player("playerCharacter")),
  hud(renderer),
  strategy(new PerPixelCollisionStrategy),
  mid_strategy(new MidPointCollisionStrategy),
  coll(false),
  shadowcoll(false),
  makeVideo( false )
{
  worldBackgrounds.push_back(new World("sky", Gamedata::getInstance().getXmlInt("sky/factor") ));
  worldBackgrounds.push_back(new World("mountains", Gamedata::getInstance().getXmlInt("mountains/factor") ));
  worldBackgrounds.push_back(new World("ground", Gamedata::getInstance().getXmlInt("ground/factor") ));
  actors.push_back(playerCharacter);
  actors.push_back(new spiderEnemy("spiderEnemy", playerCharacter->getPosition(),playerCharacter->getScaledWidth(),playerCharacter->getScaledHeight()));
  actors.push_back(new spiderEnemy("spiderEnemy2", playerCharacter->getPosition(),playerCharacter->getScaledWidth(),playerCharacter->getScaledHeight()));
  actors.push_back(new spiderEnemy("spiderEnemy3", playerCharacter->getPosition(),playerCharacter->getScaledWidth(),playerCharacter->getScaledHeight()));
  actors.push_back(new spiderEnemy("spiderEnemy4", playerCharacter->getPosition(),playerCharacter->getScaledWidth(),playerCharacter->getScaledHeight()));
  rooms.push_back(new Room("SnowyCorridor"));
  rooms.push_back(new Room("DarkLogCabin"));
  playerCharacter->attach(dynamic_cast<spiderEnemy*>(actors[1]));
  static_cast<spiderEnemy*>(actors[1])->setCurrentRoom(rooms[0]);
    playerCharacter->attach(dynamic_cast<spiderEnemy*>(actors[2]));
    static_cast<spiderEnemy*>(actors[2])->setCurrentRoom(rooms[0]);
    playerCharacter->attach(dynamic_cast<spiderEnemy*>(actors[3]));
    static_cast<spiderEnemy*>(actors[3])->setCurrentRoom(rooms[1]);
    playerCharacter->attach(dynamic_cast<spiderEnemy*>(actors[4]));
    static_cast<spiderEnemy*>(actors[4])->setCurrentRoom(rooms[1]);
  playerCharacter->setCurrentRoom(rooms,doorPlace::E,currentRoom);
  Viewport::getInstance().setObjectToTrack(rooms[currentRoom]);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {

  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_RenderClear(renderer);

  for(auto room:rooms){
    room->draw();
  }

  for(auto actor : actors) {
    actor->draw();
  }
  hud.draw();
  std::stringstream s;
  std::stringstream s2;
  s<<"Lives remaining "<<playerCharacter->getLives();
  s2<<"Spiders Remaining: "<<spiderEnemy::spiderCount;
  io.writeText(s.str(),0,0);
  io.writeText(s2.str(),0,50);


  //username in lower lefthand corner
  // io.writeText("Landon Byrd", 5, Gamedata::getInstance().getXmlInt("view/height")-25);
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  checkforWeaponCollisions();
  checkForCollisions();
  checkBorderCollisions();
  checkDoors();
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
      static_cast<spiderEnemy*>(d)->reset();
    } else{
      d->collided(false);
    }
  }
}

void Engine::checkForCollisions() {
  coll = false;
//check for hits with other actors
  for ( Drawable* d : actors ) {
    if(playerCharacter->getAnimIndex()== playerAnim::ATTACKLEFT && playerCharacter->getAnimIndex()==playerAnim::ATTACKRIGHT) return;
    if (playerCharacter !=d && strategy->execute(*playerCharacter, *d) ) {
      coll = true;
      playerCharacter->collided(true);
      playerCharacter->explode();
      currentRoom =0;
      Viewport::getInstance().setObjectToTrack(rooms[0]);
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

  void Engine::checkDoors(){
    std::map<doorPlace, Image*> doors = rooms[currentRoom]->getDoors();
      for ( auto door : doors ) {
        int scaledHeight= rooms[currentRoom]->getDoorWidth(door.first);
        int scaledWidth= rooms[currentRoom]->getDoorHeight(door.first);
        Vector2f loc = rooms[currentRoom]->getDoorloc(door.first);
        if (mid_strategy->executeImage(*playerCharacter, *door.second,scaledWidth,scaledHeight,loc) ) {

          //@TODO temporary fix
          playerCharacter->setCurrentRoom(rooms, door.first,currentRoom);
          Viewport::getInstance().setObjectToTrack(rooms[currentRoom]);
        } else {
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
        if (keystate[SDL_SCANCODE_SLASH]) {
            playerCharacter->roll();
        }
        if (keystate[SDL_SCANCODE_PERIOD]) {
          playerCharacter->attack();
        }
        if (keystate[SDL_SCANCODE_G]){
          playerCharacter->toggleGod();
        }
        if(keystate[SDL_SCANCODE_R]){
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

        draw();
        update(ticks);
        if ( makeVideo ) {
          frameGen.makeFrame();
        }
      }
    }
  }
