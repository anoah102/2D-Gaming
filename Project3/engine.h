#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"

class Player;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();
  Engine& operator=(const Engine&) = delete;

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World world;
  World tp;
  World ground;
  Viewport& viewport;

  std::vector<Drawable*> vecDrawable;

  Player* player;
  Drawable* ship;
  Drawable* mrpoopsheet;
  Drawable* spinstar;

  int currentSprite;

  bool makeVideo;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&);
  
  void printScales() const;
  void checkForCollisions();
};
