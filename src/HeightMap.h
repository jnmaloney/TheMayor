#pragma once


#include "graphics.h"
#include "Texture.h"


class HeightMap : public Texture
{
public:
  HeightMap();
  ~HeightMap();

  void HloadPngRaw(const char* file);
  void HloadPngRaw_fromMemory(const char* data, uint64_t numBytes);

  // int mWidth = 0;
  // int mHeight = 0;
  //GLuint getID() { return m_texture; }


protected:

  //GLuint m_texture = 0;
};
