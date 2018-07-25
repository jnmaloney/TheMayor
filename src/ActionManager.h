#pragma once


#include "CityMap.h"


class ActionManager
{
public:

  ActionManager(CityMap& map);
  ~ActionManager();

  CityMap& m_cityMap;

  void setNextAction(int i);

  void performTileAction();

  void setCursorPos(int i, int j);

protected:

  void checkRoadTiles(int i, int j);
  void checkRoadTile(int i, int j);

  int m_action = 0;
  CityTile* m_tile;
  int m_i = -1;
  int m_j = -1;

};
