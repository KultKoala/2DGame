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

  delete playerCharacter;

  for(auto background:worldBackgrounds){
    delete background;
  }
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  viewport( Viewport::getInstance() ),
  worldBackgrounds(),
  rooms(),
  currentRoom(1),
  playerCharacter(new Player("playerCharacter")),
  hud(renderer),
  makeVideo( false )
{
  rooms.push_back(new Room("SnowyCorridor"));
  rooms.push_back(new Room("DarkLogCabin"));
  playerCharacter->setCurrentRoom(rooms,doorPlace::W,currentRoom);
  Viewport::getInstance().setObjectToTrack(rooms[0]);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {

  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_RenderClear(renderer);

  for(auto room:rooms){
    room->draw();
  }
  playerCharacter->draw();

  hud.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto room:rooms){
    room->update(ticks);
  }
  for(auto world:worldBackgrounds){
    world->update();
  }

  playerCharacter->update(ticks);

  viewport.update(); // always update viewport last
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
