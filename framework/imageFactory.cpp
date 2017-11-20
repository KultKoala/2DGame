#include "ioMod.h"
#include "vector2f.h"
#include "renderContext.h"

ImageFactory& ImageFactory::getInstance() {
  static ImageFactory instance;
  return instance;
}

ImageFactory::~ImageFactory() {
  // Free single image containers
  std::map<std::string, SDL_Surface*>::iterator ptr = surfaces.begin();
  while ( ptr != surfaces.end() ) {
    SDL_FreeSurface(ptr->second);
    ++ptr;
  }

  std::map<std::string, SDL_Texture*>::iterator ptr2 = textures.begin();
  while(ptr2 != textures.end()){
     SDL_DestroyTexture(ptr2->second);
     ++ptr2;
  }

  std::map<std::string, Image*>::iterator ptr3 = images.begin();
  while (ptr3 != images.end()){
    delete ptr3->second;
    ++ptr3;
  }

  // Free multi-image containers
  std::map<std::string,std::vector<SDL_Surface*>>::iterator ptr4 = multiSurfaces.begin();
  while( ptr4 != multiSurfaces.end()) {
    for (unsigned int i = 0; i < ptr4->second.size(); ++i) {
      SDL_FreeSurface( ptr4->second[i] );
    }
    ++ptr4;
  }

  std::map<std::string,std::vector<SDL_Texture*>>::iterator ptr5 = multiTextures.begin();
  while (ptr5 != multiTextures.end() ) {
    for (unsigned int i = 0; i < ptr5->second.size(); ++i) {
      SDL_DestroyTexture( ptr5->second[i] );
    }
    ++ptr5;
  }

  std::map<std::string,std::vector<Image*>>::iterator ptr6 = multiImages.begin();
  while ( ptr6 != multiImages.end() ) {
    for (unsigned int i = 0; i < ptr6->second.size(); ++i) {
      delete ptr6->second[i];
    }
    ++ptr6;
  }
}

Image* ImageFactory::getImage(const std::string& name) {
    std::map<std::string, Image*>::const_iterator it = images.find(name);
  if ( it == images.end() ) {
    SDL_Surface * const surface =
      IOmod::getInstance().readSurface( gdata.getXmlStr(name+"/file"));
    bool transparency = gdata.getXmlBool(name+"/transparency");
    if ( transparency ) {
      int keyColor = SDL_MapRGBA(surface->format, 255, 0, 255, 255);
      SDL_SetColorKey(surface, SDL_TRUE, keyColor);
    }
    surfaces[name] = surface;
    RenderContext* renderContext  = RenderContext::getInstance();
    SDL_Texture * const texture =
      SDL_CreateTextureFromSurface(renderContext->getRenderer(), surface);
    Image * const image =new Image(surface, texture);
    images[name] = image;
    return image;
  }
  else {
    return it->second;
  }
}


std::vector<Image*> ImageFactory::getImages(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Image*> >::const_iterator
    pos = multiImages.find(name);
  if ( pos != multiImages.end() ) {
    return pos->second;
  }

  IOmod& iomod = IOmod::getInstance();
  RenderContext* renderContext  = RenderContext::getInstance();
  std::string sheetPath = gdata.getXmlStr(name+"/file");
  SDL_Surface* spriteSurface = iomod.readSurface(sheetPath);
  bool transparency = gdata.getXmlBool(name+"/transparency");

  // It wasn't in the map, so we have to make the vector of Images:
  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  std::vector<Image*> images;
  std::vector<SDL_Surface*> surfaces;
  std::vector<SDL_Texture*> textures;
  images.reserve(numberOfFrames);
  surfaces.reserve(numberOfFrames);
  textures.reserve(numberOfFrames);

  int width = spriteSurface->w/numberOfFrames;
  int height = spriteSurface->h;

  if(  gdata.checkTag(name+"/imageWidth")
    && gdata.checkTag(name+"/imageHeight") ){
    width  = gdata.getXmlInt(name+"/imageWidth");
    height = gdata.getXmlInt(name+"/imageHeight");
  }

  SpriteSheet sheet(spriteSurface,width,height);

  for (unsigned i = 0; i < numberOfFrames; ++i) {
    SDL_Surface* surface = sheet[i];
    if ( transparency ) {
      int keyColor = SDL_MapRGBA(spriteSurface->format, 255, 0, 255, 255);
      SDL_SetColorKey(surface, SDL_TRUE, keyColor);
    }
    SDL_Texture* texture =
      SDL_CreateTextureFromSurface(renderContext->getRenderer(),surface);
    surfaces.push_back( surface );
    textures.push_back( texture );
    images.push_back( new Image(surface, texture) );
  }
  multiSurfaces[name] = surfaces;
  multiTextures[name] = textures;
  multiImages[name] = images;
  return images;
}

// std::vector<Image*> ImageFactory::getImages(const std::string& name, const std::string& anim) {
//   // First search map to see if we've already made it:
//   std::map<std::string, std::vector<Image*> >::const_iterator
//     pos = multiImages.find(name);
//   if ( pos != multiImages.end() ) {
//     return pos->second;
//   }
//
//   IOmod& iomod = IOmod::getInstance();
//   RenderContext* renderContext  = RenderContext::getInstance();
//   std::string sheetPath = gdata.getXmlStr(name+"/"+anim+"/file");
//   SDL_Surface* spriteSurface = iomod.readSurface(sheetPath);
//   bool transparency = gdata.getXmlBool(name+"/"+anim+"/transparency");
//
//   // It wasn't in the map, so we have to make the vector of Images:
//   unsigned numberOfFrames = gdata.getXmlInt(name+"/"+anim+"/frames");
//   std::vector<Image*> images;
//   std::vector<SDL_Surface*> surfaces;
//   std::vector<SDL_Texture*> textures;
//   images.reserve(numberOfFrames);
//   surfaces.reserve(numberOfFrames);
//   textures.reserve(numberOfFrames);
//
//   int width = spriteSurface->w/numberOfFrames;
//   int height = spriteSurface->h;
//
//   if(  gdata.checkTag(name+"/"+anim+"/imageWidth")
//     && gdata.checkTag(name+"/"+anim+"/imageHeight") ){
//     width  = gdata.getXmlInt(name+"/"+anim+"/imageWidth");
//     height = gdata.getXmlInt(name+"/"+anim+"/imageHeight");
//   }
//
//   SpriteSheet sheet(spriteSurface,width,height);
//
//   for (unsigned i = 0; i < numberOfFrames; ++i) {
//     SDL_Surface* surface = sheet[i];
//     if ( transparency ) {
//       int keyColor = SDL_MapRGBA(spriteSurface->format, 255, 0, 255, 255);
//       SDL_SetColorKey(surface, SDL_TRUE, keyColor);
//     }
//     SDL_Texture* texture =
//       SDL_CreateTextureFromSurface(renderContext->getRenderer(),surface);
//     surfaces.push_back( surface );
//     textures.push_back( texture );
//     images.push_back( new Image(surface, texture) );
//   }
//   multiSurfaces[name] = surfaces;
//   multiTextures[name] = textures;
//   multiImages[name] = images;
//   return images;
// }
