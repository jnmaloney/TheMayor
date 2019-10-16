#include "HeightMap_Mesh.h"
#include "PngLoader.h"


HeightMap_Mesh::HeightMap_Mesh()
{

}


HeightMap_Mesh::~HeightMap_Mesh()
{

}


void HeightMap_Mesh::HloadPngRaw(const char* file)
{
  // Load Image
  PngLoader loader;
  loader.load(file);
  mWidth = loader.image.width;
  mHeight = loader.image.height;
}


void HeightMap_Mesh::HloadPngRaw_fromMemory(const char* data, uint64_t numBytes)
{
  // Load Image
  PngLoader loader;
  loader.load_mem(data, numBytes);
  mWidth = loader.image.width;
  mHeight = loader.image.height;

  // Create Height Map
  double* height = new double[mWidth * mHeight];
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
      height[i + j*mWidth] = h;

      // int hhh = 100. * (r * 256. * 256. + g * 256. + b) / (256. * 256.);
      // if (hhh > 255) hhh = 255;
      // height_byte[i + j*mWidth] = (GLubyte)hhh;
    }
  }

  // fake
  // mMinH = height[0];
  // mMaxH = height[0];
  mMinH = min_h;
  mMaxH = max_h;

  // Fill height map into data
  m_vertP.resize(mWidth * mHeight);
  m_vertT.resize(mWidth * mHeight);
  m_vertN.resize(mWidth * mHeight);
  for (int j = 0; j < mHeight; ++j)
  {
    for (int i = 0; i < mWidth; ++i)
    {
      //GLubyte H2 = (GLubyte)height_byte[i + j*mWidth];
      double H2 = height[i + j*mWidth];
      glm::vec4 p;
      p.x = i;
      p.y = j;
      p.z = H2;
      p.w = 1.0;
      m_vertP[i + j*mWidth] = p;

      glm::vec2 t;
      t.x = (float)i / (float)mWidth;
      t.y = (float)j / (float)mHeight;
      m_vertT[i + j*mWidth] = t;

      glm::vec3 n;
      p.x = 0;
      p.y = 0;
      p.z = 1;
      m_vertN[i + j*mWidth] = n;
    }
  }

  std::vector<GLushort>  list1;
  // for (int j = 0; j < mHeight - 1; ++j)
  // {
  //   for (int i = 0; i < mWidth - 1; ++i)
  //   {
  //     int t0 = i + (j+0) * mWidth;
  //     int t1 = i + (j+1) * mWidth;
  //
  //     list1.push_back(t0);
  //     list1.push_back(t1);
  //     list1.push_back(t0+1);
  //
  //     list1.push_back(t0+1);
  //     list1.push_back(t1);
  //     list1.push_back(t1+1);
  //   }
  // }
  for (int j = 128; j < 128 + 8; ++j)
  {
    for (int i = 128; i < 128 + 8; ++i)
    {
      int t0 = i + (j+0) * mWidth;
      int t1 = i + (j+1) * mWidth;

      list1.push_back(t0);
      list1.push_back(t1);
      list1.push_back(t0+1);

      list1.push_back(t0+1);
      list1.push_back(t1);
      list1.push_back(t1+1);
    }
  }
  m_matElements["list1"] = list1;


  // Do something
  upload_obj();

  // Clean up
  delete[] height;
}
