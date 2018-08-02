#include "Simulation.h"
#include "CityMap.h"


void Simulation::doSkyStep(CityMap* a_map)
{
  int* v = new int[a_map->m_width * a_map->m_height];

  int source = 0;
  float D = 0.25f;

  // Compute
  for (int i = 0; i < a_map->m_width; ++i)
  {
    for (int j = 0; j < a_map->m_height; ++j)
    {
      source = 0;

      CityTile& tile = a_map->getTile(i, j);
      CityTile& tileS = a_map->getTile(i, j-1);
      CityTile& tileN = a_map->getTile(i, j+1);
      CityTile& tileE = a_map->getTile(i+1, j);
      CityTile& tileW = a_map->getTile(i-1, j);
      int c = i * a_map->m_height + j;

      if (tile.roadNum && tile.isDefault == 0) source += 15; // ROAD

      //if (tile.zone) source += 150; // BUILD
      if (tile.zone == 1) source += 3 * tile.treeAng1;
      if (tile.zone == 2) source += 8 * tile.treeAng1;
      if (tile.object_type == 3) source += 250;
      if (tile.object_type == 4) source += 150;
      if (tile.object_type == 5) source += 150;
      if (tile.object_type == 6) source +=  50;

      if (tile.object_type == 1) source -= 15; // TREE

      v[c] = tile.airPollution + source +
        D * (tileN.airPollution + tileS.airPollution + tileE.airPollution + tileW.airPollution - 4 * tile.airPollution);

      if (v[c] > 255) v[c] = 255;
      if (v[c] < 0) v[c] = 0;
    }
  }

  // Apply
  for (int i = 0; i < a_map->m_width; ++i)
  {
    for (int j = 0; j < a_map->m_height; ++j)
    {
      int c = i * a_map->m_height + j;
      a_map->getTile(i, j).airPollution = v[c];
    }
  }

}


void Simulation::doConnectivity(CityMap* a_map, int resource)
{
  // Disconect the map
  for (int i = 0; i < a_map->m_width; ++i)
  {
    for (int j = 0; j < a_map->m_height; ++j)
    {
      CityTile& tile = a_map->getTile(i, j);

      // Unset
      tile.surfaceUtilityFlag &= ~(1<<resource);

      //if (!tile.foundational)

      if (tile.object_type == 2 ||
      tile.object_type == 3 ||
      tile.object_type == 5) continue;

      // Unconnect
      {
        tile.utilityFlag &= ~(1<<resource);
      }
    }
  }

  // Check each tile and if it can be connected, apply connections recursively
  for (int i = 0; i < a_map->m_width; ++i)
  {
    for (int j = 0; j < a_map->m_height; ++j)
    {
      CityTile& tile = a_map->getTile(i, j);
      CityTile& tileS = a_map->getTile(i, j-1);
      CityTile& tileN = a_map->getTile(i, j+1);
      CityTile& tileE = a_map->getTile(i+1, j);
      CityTile& tileW = a_map->getTile(i-1, j);

      if (0 == (tile.utilityFlag & (1<<resource)) && tile.pipeType == resource) // check that bit is not set
      {
        // Check any neighbours are connected
        if (tileS.utilityFlag & (1<<resource)) { connect(a_map, i, j, resource); }
        if (tileN.utilityFlag & (1<<resource)) { connect(a_map, i, j, resource); }
        if (tileE.utilityFlag & (1<<resource)) { connect(a_map, i, j, resource); }
        if (tileW.utilityFlag & (1<<resource)) { connect(a_map, i, j, resource); }
      }
    }
  }

  doSurfaceUtilities(a_map, resource);
}


void Simulation::connect(CityMap* a_map, int i, int j, int resource)
{
  CityTile& tile = a_map->getTile(i, j);
  if (tile.utilityFlag & (1<<resource)) return; // Check if set already
  if (tile.pipeType != resource) return; // Check if can be connected

  // Do the connection and apply to neightbours
  tile.utilityFlag |= (1<<resource);

  connect(a_map, i+1, j, resource);
  connect(a_map, i-1, j, resource);
  connect(a_map, i, j-1, resource);
  connect(a_map, i, j+1, resource);
}


void Simulation::doSurfaceUtilities(CityMap* a_map, int resource)
{
  //
  for (int i = 0; i < a_map->m_width; ++i)
  {
    for (int j = 0; j < a_map->m_height; ++j)
    {
      CityTile& tile = a_map->getTile(i, j);

      if (resource == 1)
      {
        // Water
        if (tile.pipeType == 1 && tile.utilityFlag & (1<<resource))
        {
          engageRadius(a_map, i, j, 4, resource);
        }
      }

      if (resource == 2)
      {
        // Power
        if (tile.object_type == 4 && tile.utilityFlag & (1<<resource))
        {
          engageRadius(a_map, i, j, 16, resource);
        }
      }

      if (resource == 3)
      {
        // Data
        if (tile.object_type == 6 && tile.utilityFlag & (1<<resource))
        {
          engageRadius(a_map, i, j, 10, resource);
        }
      }
    }
  }
}


void Simulation::engageRadius(CityMap* a_map, int x, int y, int r, int resource)
{
  for (int i = x - r; i <= x + r; ++i)
  {
    for (int j = y - r; j <= y + r; ++j)
    {
      if ((i-x)*(i-x) + (j-y)*(j-y) > (r+0.5f)*(r+0.5f)) continue;

      CityTile& tile = a_map->getTile(i, j);
      tile.surfaceUtilityFlag |= (1<<resource);
    }
  }
}
