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

  double m_lon = 0;
  double m_lat = 0;
  double m_temp = 0;
  double m_clouds = 0;
  double m_visibility = 0;
  int m_sunrise = 0;
  int m_sunset = 0;

  std::string m_location;

  int resetLocation = 0;

protected:

  void drawLocation();
};
