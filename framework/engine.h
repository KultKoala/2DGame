#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "HUD.h"

class Player;
class CollisionStrategy;
class Room;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  Viewport& viewport;
  std::vector<World*> worldBackgrounds;
  std::vector<Room*> rooms;
  int currentRoom;
  Player *playerCharacter;
  HUD hud;

  bool makeVideo;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
};
