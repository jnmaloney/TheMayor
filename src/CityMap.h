#pragma once


class CityTile
{
public:
  int isDefault = 0;
  int surface = 0;
  int zone = 0;
  int object_type = 0;
  int road = 0;
  int roadNum = 0;
  int roadAng = 0;
  int treeAng1 = 0;
  int treeAng2 = 0;
  int treeAng3 = 0;
  int treeAng4 = 0;
};


class CityMap
{
public:

  CityMap();
  ~CityMap();

  void generate();

  CityTile& getTile(unsigned int i, unsigned int j);

  void draw();

  void update();

  const unsigned int m_width = 256;
  const unsigned int m_height = 256;


  void checkRoadTiles(int i, int j);

protected:

  void checkRoadTile(int i, int j);

  CityTile defaultTile;
  CityTile tiles[142][142];

  bool m_grow = false;

  int dice();

};
