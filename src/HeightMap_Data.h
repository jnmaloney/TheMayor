#pragma once


#include "graphics.h"
#include "Mesh.h"


class HeightMap_MeshTile;


class HeightMap_Data
{
public:
  HeightMap_Data();
  ~HeightMap_Data();

  void HloadPngRaw_fromMemory(const char* data, uint64_t numBytes);

  int mWidth = 0;
  int mHeight = 0;
  //GLuint getID() { return m_texture; }

  double mMinH = 0;
  double mMaxH = 0;

  HeightMap_MeshTile* createTile(int i, int j, int w, int h);

protected:

  double* m_height;
};
