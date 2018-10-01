#pragma once


#include <vector>
#include <string>


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
  int pipeType = 0;
  int pipeNum = 0;
  int pipeAng = 0;
  int un1 = 0;
  int un2 = 0;
  int un3 = 0;
  int un4 = 0;
  int surfaceUtilityFlag = 0;
  int utilityFlag = 0;
  int airPollution = 0;
  bool foundational = false;
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
  void checkPipeTiles(int i, int j);

  static float getTileValue(const CityTile& tile);

  int m_pop = 0;

  int m_connected = 0;
  int m_watered = 0;
  int m_powered = 0;
  int m_powered_surface = 0;
  int m_data_connected = 0;
  int m_comm = 0;


  void loadFile(std::string cityName);
  void saveFile(std::string cityName);
  static void availableLoadFiles(std::vector<std::string>& files);

  void processTrees();
  int dice();

  bool ready = false;

protected:

  void checkRoadTile(int i, int j);
  void checkPipeTile(int i, int j);
  void checkUndergroundTile(int i, int j);
  int checkUndergroundTile(const CityTile& a, const CityTile& b, const CityTile& c);

  CityTile defaultTile;
  CityTile tiles[256][256];

  bool m_grow = false;

  void save();
  void load();

};
