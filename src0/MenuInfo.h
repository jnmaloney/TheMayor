#pragma once


#include <vector>
#include <string>


class Texture;


class MenuInfo
{

public:

  MenuInfo();
  ~MenuInfo();


  void draw();

protected:

  void drawLocation();
};
