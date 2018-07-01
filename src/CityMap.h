#pragma once


#include "TileTypes.h"


class CityMap
{
public:

  CityMap();
  ~CityMap();

  void generate();

  CityTile& getTile(unsigned int i, unsigned int j);

  void update();

  int getPop() { return m_pop; }

  const unsigned int m_width = 142;
  const unsigned int m_height = 142;

  int nHouseRoads = 0;
  int nHouseWatered = 0;
  int nHousePowered = 0;
  int nHouseData = 0;
  int nHasJobs = 0;

protected:

  CityTile defaultTile;
  CityTile tiles[142][142];

  bool m_grow = false;
  bool m_jobs = true;

  int m_pop = 0;

  int dice();

};
