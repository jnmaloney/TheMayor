#include "HeightMap_MeshTile.h"
#include "PngLoader.h"


HeightMap_MeshTile::HeightMap_MeshTile()
{

}


HeightMap_MeshTile::~HeightMap_MeshTile()
{

}


void HeightMap_MeshTile::createGeometry(int a_i, int a_j, int a_w, int a_h, double* data)
{
  mWidth = a_w;
  mHeight = a_h;

  // Fill height map into data
  m_vertP.resize(mWidth * mHeight);
  m_vertT.resize(mWidth * mHeight);
  m_vertN.resize(mWidth * mHeight);
  for (int j = 0; j < a_h; ++j)
  {
    for (int i = 0; i < a_w; ++i)
    {
      double H2 = data[(a_i+i) + (a_j+j)*256]; // <-- special data number 256
      glm::vec4 p;
      //p.x = a_i + i;
      p.x = 256 - (a_i + i);
      p.y = a_j + j;
      //p.y = 256 - (a_j + j); // Invert y
      p.z = H2;
      p.w = 1.0;
      m_vertP[i + j*mWidth] = p;

      glm::vec2 t;
      t.x = (float)i / (float)(a_w - 1);
      t.y = (float)j / (float)(a_h - 1);
      m_vertT[i + j*mWidth] = t;

      glm::vec3 n;
      p.x = 0;
      p.y = 0;
      p.z = 1;
      m_vertN[i + j*mWidth] = n;
    }
  }

  std::vector<GLushort>  list1;
  for (int j = 0; j < a_h - 1; ++j)
  {
    for (int i = 0; i < a_w - 1; ++i)
    {
      int t0 = i + (j+0) * a_w;
      int t1 = i + (j+1) * a_w;

      list1.push_back(t0);
      list1.push_back(t1);
      list1.push_back(t0+1);

      list1.push_back(t0+1);
      list1.push_back(t1);
      list1.push_back(t1+1);
    }
  }
  m_matElements["list1"] = list1;

  // Create the mesh from data
  upload_obj();
}
