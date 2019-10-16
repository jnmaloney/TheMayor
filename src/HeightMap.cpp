#include "HeightMap.h"
#include "PngLoader.h"
//#include "PngUtils.h"


HeightMap::HeightMap()
{

}


HeightMap::~HeightMap()
{

}


void HeightMap::HloadPngRaw(const char* file)
{
  // Load Image
  PngLoader loader;
  loader.load(file);
  mWidth = loader.image.width;
  mHeight = loader.image.height;
}


void HeightMap::HloadPngRaw_fromMemory(const char* data, uint64_t numBytes)
{
  // Load Image
  PngLoader loader;
  loader.load_mem(data, numBytes);
  mWidth = loader.image.width;
  mHeight = loader.image.height;

  // Create Height Map
  double* height = new double[mWidth * mHeight];
  GLubyte* height_byte = new GLubyte[mWidth * mHeight];
  double min_h = -10000 + ((256 * 256 * 256 + 256 * 256 + 256) * 0.1);
  double max_h = -10000;
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
      double h = -10000.0 + ((r * 256. * 256. + g * 256. + b) * 0.1);
      if (h < min_h) min_h = h;
      if (h > max_h) max_h = h;
      height[i + j*mWidth] = h;
      //height_byte[i + j*mWidth] = B;
      int hhh = 100. * (r * 256. * 256. + g * 256. + b) / (256. * 256.);
      if (hhh > 255) hhh = 255;
      height_byte[i + j*mWidth] = (GLubyte)hhh;
      //height_byte[i + j*mWidth] = 0.5 * G + (0.5/255.0) * B;
    }
  }

  // Fill height map into data
  //unsigned char* col = new unsigned char[mWidth * mHeight * 4];
  GLubyte* col = new unsigned char[loader.size];
  for (int j = 0; j < mHeight; ++j)
  {
    for (int i = 0; i < mWidth; ++i)
    {
      //printf("(%i, %f)   \n", height_byte[i + j*mWidth], height[i + j*mWidth]);
      //GLubyte H1 = (GLubyte)height[i + j*mWidth];
      GLubyte H2 = (GLubyte)height_byte[i + j*mWidth];
      // if (H1 != H2)
      // {
      //   printf("(%i, %f) = %i?    \n", height_byte[i + j*mWidth], height[i + j*mWidth], H1);
      // }
      //GLubyte H = 255 * (height[i + j*mWidth] - min_h) / (max_h - min_h);
      //if (height[i + j*mWidth] > -9000) H = 128;
      //if (height[i + j*mWidth] == min_h) H = 128;
      col[i*4 + j * mWidth * 4 + 0] = H2;
      col[i*4 + j * mWidth * 4 + 1] = H2;//buffer[i*4 + j * mWidth * 4 + 1];
      col[i*4 + j * mWidth * 4 + 2] = H2;//buffer[i*4 + j * mWidth * 4 + 2];
      col[i*4 + j * mWidth * 4 + 3] = 255;
    }
  }

  // Create Texture
  glActiveTexture(GL_TEXTURE1);

  glGenTextures(1, &m_texture);

  glBindTexture(GL_TEXTURE_2D, m_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    mWidth,
    mHeight,
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    col);

  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, m_texture);

  //delete[] col;
  delete[] height;
  delete[] height_byte;
}
