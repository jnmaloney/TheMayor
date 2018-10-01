#include "ActionManager.h"
#include <stdio.h>
#include <iostream>
#include "Simulation.h"


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


bool clearBush(CityTile* tile)
{
  if (tile->object_type == 1)
  {
    tile->object_type = 0;
    tile->treeAng1 = 0;
    tile->treeAng2 = 0;
    tile->treeAng3 = 0;
    tile->treeAng4 = 0;
  }
  return tile->object_type == 0 && tile->road == 0;
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
      m_tile->zone = 0;
      m_cityMap.checkRoadTiles(m_i, m_j);
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
    if (m_tile->road || m_tile->object_type) return;

    m_tile->object_type = 1;
    m_tile->treeAng1 = 1;
    m_tile->treeAng2 = 1;
    m_tile->treeAng3 = 1;
    m_tile->treeAng4 = 1;
  }
  // Water
  else if (m_action == 5)
  {
    if (clearBush(m_tile) && (m_tile->pipeType == 0 || m_tile->pipeType == 1))
    {
      m_tile->object_type = 2;
      m_tile->pipeType = 1;
      m_tile->foundational = true;
      m_tile->utilityFlag |= 1<<1;
      Simulation::doConnectivity(&m_cityMap, 1);
      m_cityMap.checkPipeTiles(m_i, m_j);
    }
  }
  // Power
  else if (m_action == 6)
  {
    if (clearBush(m_tile) && (m_tile->pipeType == 0 || m_tile->pipeType == 2))
    {
      m_tile->object_type = 3;
      m_tile->pipeType = 2;
      m_tile->foundational = true;
      m_tile->utilityFlag |= 1<<2;
      Simulation::doConnectivity(&m_cityMap, 2);
      m_cityMap.checkPipeTiles(m_i, m_j);
    }
  }
  // Power
  else if (m_action == 7)
  {
    if (clearBush(m_tile) && (m_tile->pipeType == 0 || m_tile->pipeType == 2))
    {
      m_tile->object_type = 4;
      m_tile->pipeType = 2;
      m_tile->foundational = true;
      Simulation::doConnectivity(&m_cityMap, 2);
      m_cityMap.checkPipeTiles(m_i, m_j);
    }
  }
  // Data
  else if (m_action == 8)
  {
    if (clearBush(m_tile) && (m_tile->pipeType == 0 || m_tile->pipeType == 3))
    {
      m_tile->object_type = 5;
      m_tile->pipeType = 3;
      m_tile->foundational = true;
      m_tile->utilityFlag |= 1<<3;
      Simulation::doConnectivity(&m_cityMap, 3);
      m_cityMap.checkPipeTiles(m_i, m_j);
    }
  }
  // Data
  else if (m_action == 9)
  {
    if (clearBush(m_tile) && (m_tile->pipeType == 0 || m_tile->pipeType == 3))
    {
      m_tile->object_type = 6;
      m_tile->pipeType = 3;
      m_tile->foundational = true;
      Simulation::doConnectivity(&m_cityMap, 3);
      m_cityMap.checkPipeTiles(m_i, m_j);
    }
  }
  // Bulldoze
  else if (m_action == 10)
  {
    m_tile->object_type = 0;
    m_tile->road = 0;
    m_tile->zone = 0;
    m_cityMap.checkRoadTiles(m_i, m_j);

    if (m_tile->foundational)
    {
      m_tile->foundational = false;
      int t = m_tile->pipeType;
      m_tile->pipeType = 0;
      Simulation::doConnectivity(&m_cityMap, t);
      m_cityMap.checkPipeTiles(m_i, m_j);
    }
  }

  // - Underground -

  //
  else if (m_action == 101)
  {
    if (m_tile->pipeType == 0) m_tile->pipeType = 1;
    m_cityMap.checkPipeTiles(m_i, m_j);
    Simulation::doConnectivity(&m_cityMap, 1);
  }
  else if (m_action == 102)
  {
    if (m_tile->pipeType == 0) m_tile->pipeType = 2;
    m_cityMap.checkPipeTiles(m_i, m_j);
    Simulation::doConnectivity(&m_cityMap, 2);
  }
  else if (m_action == 103)
  {
    if (m_tile->pipeType == 0) m_tile->pipeType = 3;
    m_cityMap.checkPipeTiles(m_i, m_j);
    Simulation::doConnectivity(&m_cityMap, 3);
  }
  // Bulldoze
  else if (m_action == 104)
  {
    if (m_tile->foundational) return;

    if (m_tile->pipeType)
    {
      int t = m_tile->pipeType;
      m_tile->pipeType = 0;
      Simulation::doConnectivity(&m_cityMap, t);
      m_cityMap.checkPipeTiles(m_i, m_j);
    }
  }
}


void ActionManager::beginActionCommand()
{
  m_lineTool.reset();

  if (m_action == 1 || m_action == 2 || m_action == 3 ||
      m_action == 101 || m_action == 102 || m_action == 103)
      {
        m_dragEnabled = true;
      }
  else m_dragEnabled = false;


  if (m_dragEnabled)
  {
    m_lineTool.reset();
    m_lineTool.setPoint(m_i, m_j);
  }
  else
  {
    performTileAction();
  }
}


void ActionManager::releaseActionCommand()
{
  if (m_dragEnabled)
  {
    int x1, x2, y1, y2;
    if (!m_lineTool.getLine(x1, y1, x2, y2))
    {
      m_lineTool.reset();
      m_dragEnabled = false;
      m_action = 0;
      return;
    }

    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    for (int i = x1; i <= x2; ++i)
    {
      for (int j = y1; j <= y2; ++j)
      {
        setCursorPos(i, j);
        performTileAction();
      }
    }
  }
  m_lineTool.reset();
  m_dragEnabled = false;
  m_action = 0;
}


void ActionManager::cancelActionCommand()
{
  m_dragEnabled = false;
  m_action = 0;
  m_lineTool.reset();
}


void ActionManager::cursorPos(int x, int y)
{
  if (m_dragEnabled && m_action > 0)
  {
    //performTileAction();
    m_lineTool.setPoint(x, y);
  }
}
