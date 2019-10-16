#pragma once


#include "graphics.h"
#include "Mesh.h"


class HeightMap_MeshTile : public Mesh
{
public:
  HeightMap_MeshTile();
  ~HeightMap_MeshTile();

  int mWidth = 0;
  int mHeight = 0;
  //GLuint getID() { return m_texture; }

  double mMinH = 0;
  double mMaxH = 0;

  void createGeometry(int i, int j, int w, int h, double* data);

protected:

  //GLuint m_texture = 0;
};
