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

  bool active() { return m_action > 0; }

  bool continueAction() { return m_action == 1 || m_action == 2 || m_action == 3; }

protected:

  int m_action = 0;
  CityTile* m_tile;
  int m_i = -1;
  int m_j = -1;

};
