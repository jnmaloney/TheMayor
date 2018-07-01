#include "CityMap.h"
#include <random>


// const int CityMap::m_width = 24;
// const int CityMap::m_height = 24;

int CityMap::dice()
{
  static std::default_random_engine generator;
  static std::uniform_int_distribution<int> distribution(1, 1000);
  static auto dice2 = std::bind ( distribution, generator );
  return dice2();
}


CityMap::CityMap()
{
  defaultTile.tileType = TILE_ROAD;
  defaultTile.isDefault = true;



}


CityMap::~CityMap()
{


}


void CityMap::generate()
{

  // tiles[0][0].object_type = 1;
  // tiles[1][2].object_type = 2;
   // tiles[3][5].object_type = 3;



  for (int i = 0; i < m_width; ++i)
  {
    for (int j = 0; j < m_height; ++j)
    {
      if (dice() > 420)
      {
        tiles[i][j].tileType = TILE_TREE;
        TreeData & data = (TreeData&)tiles[i][j].data;
        while (data.treeAng1 + data.treeAng2 +
               data.treeAng3 + data.treeAng4 == 0)
        {
          if (dice() > 300) data.treeAng1 = dice();
          if (dice() > 300) data.treeAng2 = dice();
          if (dice() > 300) data.treeAng3 = dice();
          if (dice() > 300) data.treeAng4 = dice();
        }
      }
    }
  }

  //
  // tiles[5][5].object_type = 3;
  //
  // tiles[8][11].object_type = 2;
  // tiles[8][11].treeAng1 = 1;
  //
  // tiles[8][12].object_type = 2;
  // tiles[8][12].treeAng1 = 2;
  //
  // tiles[8][13].object_type = 2;
  // tiles[8][13].treeAng1 = 3;
  //
  // tiles[8][14].object_type = 2;
  // tiles[8][14].treeAng1 = 4;
  //
  // tiles[8][15].object_type = 2;
  // tiles[8][15].treeAng1 = 5;


}


CityTile& CityMap::getTile(unsigned int i, unsigned int j)
{
  if (i < m_width && j < m_height)
  {
    return tiles[i][j];
  }

  return defaultTile;

}


void CityMap::update()
{
  int powerCount = 0;
  m_pop = 0;
  nHouseRoads = 0;
  nHouseWatered = 0;
  nHousePowered = 0;
  nHouseData = 0;
  nHasJobs = 0;
  int jobCount = 0;

  for (int i = 0; i < m_width; ++i)
  {
    for (int j = 0; j < m_height; ++j)
    {

      CityTile& tile = tiles[i][j];

      // Residence
      if (tiles[i][j].tileType == TILE_ZONE)
      {
        ZoneData& data = (ZoneData&)tile.data;

        if (data.zone == 1)
        {
          // Check road
          CityTile& t_n = getTile(i, j - 1);
          CityTile& t_s = getTile(i, j + 1);
          CityTile& t_e = getTile(i + 1, j);
          CityTile& t_w = getTile(i - 1, j);

          int num = 0;
          if (t_n.tileType == TILE_ROAD) num += 1;
          if (t_s.tileType == TILE_ROAD) num += 1;
          if (t_e.tileType == TILE_ROAD) num += 1;
          if (t_w.tileType == TILE_ROAD) num += 1;

          if (num == 0) continue;
          ++nHouseRoads;

          // Check power / water
          if (0 == (tile.surfaceUtilityFlag & (1 << 2))) continue;
          ++nHouseWatered;
          if (0 == (tile.surfaceUtilityFlag & (1 << 1))) continue;
          ++nHousePowered;

          if (tile.surfaceUtilityFlag & (1 << 3)) ++nHouseData;
          if (data.buildAng2 > 35 && 0 == (tile.surfaceUtilityFlag & (1 << 3))) continue;
          if (data.buildAng2 > 39 && m_jobs) continue;

          if (m_grow && dice() < 100) {} else continue;

          {
            // Grow
            data.buildAng2 += 1;
            if (data.buildAng2 > 49) data.buildAng2 = 49;
            data.buildAng1 = 1 + data.buildAng2 / 10;
          }
        }
        else if (data.zone == 2)
        {
          // Check power / water
          if (0 == (tile.surfaceUtilityFlag & (1 << 1))) continue;
          if (0 == (tile.surfaceUtilityFlag & (1 << 2))) continue;

          if (data.buildAng2 > 8 && 0 == (tile.surfaceUtilityFlag & (1 << 3))) continue;

          // Check road
          CityTile& t_n = getTile(i, j - 1);
          CityTile& t_s = getTile(i, j + 1);
          CityTile& t_e = getTile(i + 1, j);
          CityTile& t_w = getTile(i - 1, j);

          int num = 0;
          if (t_n.tileType == TILE_ROAD) num += 1;
          if (t_s.tileType == TILE_ROAD) num += 1;
          if (t_e.tileType == TILE_ROAD) num += 1;
          if (t_w.tileType == TILE_ROAD) num += 1;

          if (num == 0) continue;

          {
            // Grow
            data.buildAng2 += 1;
            if (data.buildAng2 > 19) data.buildAng2 = 19;
            data.buildAng1 = 6 + data.buildAng2 / 10;
          }
        }
      }

      // Job counting
      if (tile.tileType == TILE_ZONE)
      {
        ZoneData& data = (ZoneData&)tiles[i][j].data;
        if (data.zone == 1)
        {
          jobCount -= data.buildAng2;
        }
        if (data.zone == 2)
        {
          jobCount += 100.f * data.buildAng2;
          nHasJobs = 1;
        }
      }


      // Power counting...
      // Consume power
      if (tile.tileType == TILE_ZONE &&
          (tile.surfaceUtilityFlag & (1<<1)))
      {
        ZoneData& data = (ZoneData&)tiles[i][j].data;
        powerCount -= data.buildAng2;
        m_pop += data.buildAng2;
      }
      // Add power
      else if (tiles[i][j].tileType == TILE_BUILD)
      {
        BuildData& data = (BuildData&)tiles[i][j].data;
        if (data.resourceType == 4)
        {
          powerCount += data.resourceValue;
        }
      }

    }
  }

  m_grow = (powerCount > 0);
  m_jobs = (jobCount > 0);
}
