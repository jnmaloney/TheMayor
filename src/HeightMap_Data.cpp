#include "HeightMap_Data.h"
#include "PngLoader.h"
#include "HeightMap_MeshTile.h"


HeightMap_Data::HeightMap_Data()
{

}


HeightMap_Data::~HeightMap_Data()
{

}


void HeightMap_Data::HloadPngRaw_fromMemory(const char* data, uint64_t numBytes)
{
  // Load Image
  PngLoader loader;
  loader.load_mem(data, numBytes);
  mWidth = loader.image.width;
  mHeight = loader.image.height;

  // Create Height Map
  m_height = new double[mWidth * mHeight];
  double min_h = 0.00005 * ((256 * 256 * 256 + 256 * 256 + 256) * 0.001);
  double max_h = 0;
  GLubyte* buffer = (GLubyte*)loader.buffer;
  for (int j = 0; j < mHeight; ++j)
  {
    for (int i = 0; i < mWidth; ++i)
    {
      GLubyte R = buffer[i*4 + j * mWidth * 4 + 0];
      GLubyte G = buffer[i*4 + j * mWidth * 4 + 1];
      GLubyte B = buffer[i*4 + j * mWidth * 4 + 2];
      double r = (double)R;
      double g = (double)G;
      double b = (double)B;
      //double h = -10000.0 + ((r * 256. * 256. + g * 256. + b) * 0.1);
      double h = 0.00005 * (r * 256. * 256. + g * 256. + b) * 0.001;
      if (h < min_h) min_h = h;
      if (h > max_h) max_h = h;
      m_height[i + j*mWidth] = h;
    }
  }

  mMinH = min_h;
  mMaxH = max_h;
}


HeightMap_MeshTile* HeightMap_Data::createTile(int i, int j, int w, int h)
{
  HeightMap_MeshTile* tile = new HeightMap_MeshTile();

  tile->createGeometry(i, j, w, h, m_height);

  return tile;
}
