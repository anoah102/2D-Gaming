#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "player.h"
#include "multisprite.h"
#include "twowaymultiSprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"

Engine::~Engine() { 
  delete player;
  delete ship;
  delete spinstar;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  tp("tp", Gamedata::getInstance().getXmlInt("tp/factor") ),
  ground("ground", Gamedata::getInstance().getXmlInt("ground/factor") ),
  viewport( Viewport::getInstance() ),
  
  //mrpoopsheet(new twowaymultiSprite("poopsheet")),
  player(new Player("poopsheet")),
  ship(new Sprite("Ship")),
  spinstar(new MultiSprite("SpinningStar")),
  currentSprite(0),
  makeVideo( false )
{
  vecDrawable.push_back(ship);
  //vecDrawable.push_back(mrpoopsheet);
  vecDrawable.push_back(spinstar);

  
  Viewport::getInstance().setObjectToTrack(player);
  
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  


  world.draw();
  tp.draw();
  ground.draw();



  for(auto &it : vecDrawable)
    it->draw();
  std::stringstream ss;
  
  ss << "FPS: " << clock.getFps();
  io.writeText(ss.str(), 30, 60);
  ss.str(std::string());
  ss << "Ali Noah";
  SDL_Color color = {170,0,140, 255};
  io.writeText(ss.str(), 30, 450, color);
  player->draw();
  viewport.draw();

  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  
  player->update(ticks);
  //for(auto &it : vecDrawable)
   // it->update(ticks);
  spinstar->updateInPlace(ticks);
  world.update();
  tp.update();
  ground.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 2;
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(ship);
  }
  else {
    Viewport::getInstance().setObjectToTrack(player);
  }
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
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
