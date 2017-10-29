#ifndef twowaymultisprite__H
#define twowaymultisprite__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class twowaymultiSprite : public Drawable {
public:
  twowaymultiSprite(const std::string&);
  twowaymultiSprite(const twowaymultiSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

private:

 std::vector<Image *> imagesRight;
 std::vector<Image *> imagesLeft;
 std::vector<Image *> images;

protected:
  //std::vector<Image *> images;
 

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  twowaymultiSprite& operator=(const twowaymultiSprite&);
};
#endif
