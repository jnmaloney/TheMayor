#include "ActionManager.h"
#include <stdio.h>
#include <iostream>


ActionManager::ActionManager(CityMap& map) :
  m_cityMap(map),
  m_tile(0)
{


}


ActionManager::~ActionManager()
{

}


void ActionManager::setNextAction(int i)
{
  m_action = i;
}


void ActionManager::setCursorPos(int i, int j)
{
  m_tile = &m_cityMap.getTile(i, j);
  m_i = i;
  m_j = j;
}


bool clearBush(MapTile& tile)
{
  if (tile.object_type == 1) tile.object_type = 0;
  return tile.object_type == 0 && tile.road == 0;
}


void ActionManager::performTileAction()
{
  if (m_tile == 0) return;

  // Road
  if (m_action == 1)
  {
    if (clearBush(m_tile))
    {
      m_tile->road = 1;
      m_cityMap.checkRoadTile(m_i, m_j);
    }
  }
  // Zone
  else if (m_action == 2)
  {
    if (clearBush(m_tile))
      m_tile->zone = 1;
  }
  // Zone
  else if (m_action == 3)
  {
    if (clearBush(m_tile))
      m_tile->zone = 2;
  }
  // Park
  else if (m_action == 4)
  {
    if (m_tile.road || m_tile.object_type) return;

    m_tile->object_type = 1;
    m_tile->treeAng1 = 1;
    m_tile->treeAng2 = 1;
    m_tile->treeAng3 = 1;
    m_tile->treeAng4 = 1;
  }
  // Water
  else if (m_action == 5)
  {
    if (clearBush(m_tile))
      m_tile->object_type = 2;
  }
  // Power
  else if (m_action == 6)
  {
    if (clearBush(m_tile))
      m_tile->object_type = 3;
  }
  // Power
  else if (m_action == 7)
  {
    if (clearBush(m_tile))
      m_tile->object_type = 4;
  }
  // Data
  else if (m_action == 8)
  {
    if (clearBush(m_tile))
      m_tile->object_type = 5;
  }
  // Data
  else if (m_action == 9)
  {
    if (clearBush(m_tile))
      m_tile->object_type = 6;
  }
  // Bulldoze
  else if (m_action == 10)
  {
    m_tile->object_type = 0;
    m_tile->road = 0;
  }
}
