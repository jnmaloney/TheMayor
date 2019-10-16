#pragma once


#include "graphics.h"
#include "Mesh.h"


class HeightMap_Mesh : public Mesh
{
public:
  HeightMap_Mesh();
  ~HeightMap_Mesh();

  void HloadPngRaw(const char* file);
  void HloadPngRaw_fromMemory(const char* data, uint64_t numBytes);

  int mWidth = 0;
  int mHeight = 0;
  //GLuint getID() { return m_texture; }

  double mMinH = 0;
  double mMaxH = 0;


protected:

  //GLuint m_texture = 0;
};
