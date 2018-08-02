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
  defaultTile.road = 1;
  defaultTile.isDefault = true;

  //tiles = new CityTile[m_width * m_height];

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
        tiles[i][j].object_type = 1;
        while (tiles[i][j].treeAng1 + tiles[i][j].treeAng2 +
               tiles[i][j].treeAng3 + tiles[i][j].treeAng4 == 0)
        {
          if (dice() > 300) tiles[i][j].treeAng1 = dice();
          if (dice() > 300) tiles[i][j].treeAng2 = dice();
          if (dice() > 300) tiles[i][j].treeAng3 = dice();
          if (dice() > 300) tiles[i][j].treeAng4 = dice();
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


bool growRes(int dv, int l)
{
  // l1  40 20%
  // l2  80 40%
  // l3 120 60%
  // l4 140 80%
  // l5 200
  if (dv > l * 40) return true;

  return false;
}

bool growCom(int dv, int l)
{
  // l1  40 20%
  // l2  80 40%
  // l3 120

  if (l == 1 && dv > 60) return true;
  if (l == 2 && dv > 120) return true;
  if (l == 3) return false;

  return true;
}


void CityMap::update()
{
  int powerCount = 0;
  int pop = 0;
  m_connected = 0;
  m_watered = 0;

  for (int i = 0; i < m_width; ++i)
  {
    for (int j = 0; j < m_height; ++j)
    {
      // Utility value
      int dval = 0;
      if (tiles[i][j].zone)
      {
        int x = 1;
        if (tiles[i][j].surfaceUtilityFlag & 1<<1) ++x;
        if (tiles[i][j].surfaceUtilityFlag & 1<<2) ++x;
        if (tiles[i][j].surfaceUtilityFlag & 1<<3) ++x;

        float v = getTileValue(tiles[i][j]);

        dval = 200 * v;
      }

      // Residence
      if (tiles[i][j].zone == 1)
      {
        // Check road
        CityTile& t_n = getTile(i, j - 1);
        CityTile& t_s = getTile(i, j + 1);
        CityTile& t_e = getTile(i + 1, j);
        CityTile& t_w = getTile(i - 1, j);

        int num = 0;
        if (t_n.road) num += 1;
        if (t_s.road) num += 1;
        if (t_e.road) num += 1;
        if (t_w.road) num += 1;

        if (num) {
          m_connected++;

          // Grow
          if (m_grow && dice() < dval)
          {
            // Value check
            if (growRes(dval, tiles[i][j].treeAng1))
            {
              tiles[i][j].treeAng2 += 1;

              if (tiles[i][j].treeAng2 > 49) tiles[i][j].treeAng2 = 49;
              int newObj = 1 + tiles[i][j].treeAng2 / 10;
              if (newObj != tiles[i][j].treeAng1)
              {
                tiles[i][j].treeAng1 = newObj;

                // Set angle to face road
                if (t_s.road)
                {
                  tiles[i][j].treeAng3 = 0;
                }
                else if (t_e.road)
                {
                  tiles[i][j].treeAng3 = 3;
                }
                else if (t_n.road)
                {
                  tiles[i][j].treeAng3 = 2;
                }
                else
                {
                  tiles[i][j].treeAng3 = 1;
                }
              }
            }
          }
        }
      }

      // Comm
      else if (tiles[i][j].zone == 2)
      {
        // Check road
        CityTile& t_n = getTile(i, j - 1);
        CityTile& t_s = getTile(i, j + 1);
        CityTile& t_e = getTile(i + 1, j);
        CityTile& t_w = getTile(i - 1, j);

        int num = 0;
        if (t_n.road) num += 1;
        if (t_s.road) num += 1;
        if (t_e.road) num += 1;
        if (t_w.road) num += 1;

        if (num)
        {
          // Grow
          if (tiles[i][j].treeAng1 < 3 && m_grow && dice() < dval)
          {
            if (growCom(dval, tiles[i][j].treeAng1))
            {
              tiles[i][j].treeAng2 += 1;
              if (tiles[i][j].treeAng2 > 19 || tiles[i][j].treeAng1 == 0)
              {
                tiles[i][j].treeAng2 = 0;
                tiles[i][j].treeAng1++;

                if (tiles[i][j].treeAng1 == 1)
                {
                    tiles[i][j].object_type = 200 + dice() % 3;
                }
                if (tiles[i][j].treeAng1 == 2)
                {
                    tiles[i][j].object_type = 210 + dice() % 4;
                }
                if (tiles[i][j].treeAng1 == 3)
                {
                    tiles[i][j].object_type = 220 + dice() % 4;
                }

                // Set angle to face road
                if (t_s.road)
                {
                  tiles[i][j].treeAng3 = 0;
                }
                else if (t_e.road)
                {
                  tiles[i][j].treeAng3 = 3;
                }
                else if (t_n.road)
                {
                  tiles[i][j].treeAng3 = 2;
                }
                else
                {
                  tiles[i][j].treeAng3 = 1;
                }
              }
            }
          }
        }
      }

      // Power counting
      if (tiles[i][j].zone) powerCount -= tiles[i][j].treeAng2;
      if (tiles[i][j].object_type == 3) powerCount += 500000;

      // Pop counting
      if (tiles[i][j].zone == 1) pop += tiles[i][j].treeAng2;

      // Water counting
      if (tiles[i][j].zone == 1 && tiles[i][j].surfaceUtilityFlag & (1<<1))
      {
        m_watered++;
      }

      // Check
      if (tiles[i][j].zone == 1 && tiles[i][j].surfaceUtilityFlag & (1<<2))
      {
        m_powered_surface = 1;
      }
      if (tiles[i][j].zone == 2)
      {
        m_comm = 1;
      }
      if (tiles[i][j].zone == 2 && tiles[i][j].surfaceUtilityFlag & (1<<3))
      {
        m_data_connected = 1;
      }

      // Assign objects to house tiles?
      if (tiles[i][j].zone == 1 && tiles[i][j].treeAng2 > 0)
      {
        int t = tiles[i][j].treeAng2 / 10;
        tiles[i][j].object_type = 101 + t;
      }
    }
  }

  m_grow = (powerCount > 0);
  m_pop = pop;
  m_powered = powerCount;
}


void CityMap::checkRoadTiles(int ci, int cj)
{
  checkRoadTile(ci, cj);
  checkRoadTile(ci, cj+1);
  checkRoadTile(ci, cj-1);
  checkRoadTile(ci-1, cj);
  checkRoadTile(ci+1, cj);
}


void CityMap::checkRoadTile(int i, int j)
{
  CityTile& t = getTile(i, j);
  if (t.road == 0) return;

  CityTile& t_n = getTile(i, j - 1);
  CityTile& t_s = getTile(i, j + 1);
  CityTile& t_e = getTile(i + 1, j);
  CityTile& t_w = getTile(i - 1, j);

  int num = 0;
  if (t_n.road) num += 1;
  if (t_s.road) num += 1;
  if (t_e.road) num += 1;
  if (t_w.road) num += 1;

  int x = 0;
  if (t_n.road) x += 1;
  if (t_s.road) x += 2;
  if (t_e.road) x += 4;
  if (t_w.road) x += 8;

  t.roadNum = num;

  // ..nsew
  if (x == 0b0000)
  {
    t.roadNum = 0;
    t.roadAng = 0;
  }
  else if (x == 0b0001)
  {
    t.roadNum = 0;
    t.roadAng = 1;
  }
  else if (x == 0b0010)
  {
    t.roadNum = 0;
    t.roadAng = 1;
  }
  else if (x == 0b0011)
  {
    t.roadNum = 0;
    t.roadAng = 1;
  }
  else if (x == 0b0100)
  {
    t.roadNum = 0;
    t.roadAng = 0;
  }
  else if (x == 0b0101)
  {
    t.roadNum = 1;
    t.roadAng = 0;
  }
  else if (x == 0b0110)
  {
    t.roadNum = 1;
    t.roadAng = 1;
  }
  else if (x == 0b0111)
  {
    t.roadNum = 2;
    t.roadAng = 0;
  }
  else if (x == 0b1000)
  {
    t.roadNum = 0;
    t.roadAng = 0;
  }
  else if (x == 0b1001)
  {
    t.roadNum = 1;
    t.roadAng = 3;
  }
  else if (x == 0b1010)
  {
    t.roadNum = 1;
    t.roadAng = 2;
  }
  else if (x == 0b1011)
  {
    t.roadNum = 2;
    t.roadAng = 2;
  }
  else if (x == 0b1100)
  {
    t.roadNum = 0;
    t.roadAng = 0;
  }
  else if (x == 0b1101)
  {
    t.roadNum = 2;
    t.roadAng = 3;
  }
  else if (x == 0b1110)
  {
    t.roadNum = 2;
    t.roadAng = 1;
  }
  else if (x == 0b1111)
  {
    t.roadNum = 3;
    t.roadAng = 0;
  }
}


void CityMap::checkPipeTiles(int i, int j)
{
  checkPipeTile(i, j);
  checkPipeTile(i+1, j);
  checkPipeTile(i-1, j);
  checkPipeTile(i, j+1);
  checkPipeTile(i, j-1);

  checkUndergroundTile(i, j);
  checkUndergroundTile(i+1, j);
  checkUndergroundTile(i-1, j);
  checkUndergroundTile(i, j+1);
  checkUndergroundTile(i, j-1);

  checkUndergroundTile(i+1, j+1);
  checkUndergroundTile(i-1, j+1);
  checkUndergroundTile(i+1, j-1);
  checkUndergroundTile(i-1, j-1);
}


void CityMap::checkPipeTile(int i, int j)
{
  CityTile& tile = getTile(i, j);

  if (tile.pipeType == 0) return;
  if (tile.foundational)
  {
    tile.pipeNum = 3;
    return;
  }

  CityTile& t_n = getTile(i, j - 1);
  CityTile& t_s = getTile(i, j + 1);
  CityTile& t_e = getTile(i + 1, j);
  CityTile& t_w = getTile(i - 1, j);

  int x = 0;
  if (t_n.pipeType == tile.pipeType) x += 1;
  if (t_s.pipeType == tile.pipeType) x += 2;
  if (t_e.pipeType == tile.pipeType) x += 4;
  if (t_w.pipeType == tile.pipeType) x += 8;

  // ..nsew
  if (x == 0b0000)
  {
    tile.pipeNum = 0;
    tile.pipeAng = 0;
  }
  else if (x == 0b0001)
  {
    tile.pipeNum = 0;
    tile.pipeAng = 1;
  }
  else if (x == 0b0010)
  {
    tile.pipeNum = 0;
    tile.pipeAng = 1;
  }
  else if (x == 0b0011)
  {
    tile.pipeNum = 0;
    tile.pipeAng = 1;
  }
  else if (x == 0b0100)
  {
    tile.pipeNum = 0;
    tile.pipeAng = 0;
  }
  else if (x == 0b0101)
  {
    tile.pipeNum = 1;
    tile.pipeAng = 0;
  }
  else if (x == 0b0110)
  {
    tile.pipeNum = 1;
    tile.pipeAng = 1;
  }
  else if (x == 0b0111)
  {
    tile.pipeNum = 2;
    tile.pipeAng = 0;
  }
  else if (x == 0b1000)
  {
    tile.pipeNum = 0;
    tile.pipeAng = 0;
  }
  else if (x == 0b1001)
  {
    tile.pipeNum = 1;
    tile.pipeAng = 3;
  }
  else if (x == 0b1010)
  {
    tile.pipeNum = 1;
    tile.pipeAng = 2;
  }
  else if (x == 0b1011)
  {
    tile.pipeNum = 2;
    tile.pipeAng = 2;
  }
  else if (x == 0b1100)
  {
    tile.pipeNum = 0;
    tile.pipeAng = 0;
  }
  else if (x == 0b1101)
  {
    tile.pipeNum = 2;
    tile.pipeAng = 3;
  }
  else if (x == 0b1110)
  {
    tile.pipeNum = 2;
    tile.pipeAng = 1;
  }
  else if (x == 0b1111)
  {
    tile.pipeNum = 3;
    tile.pipeAng = 0;
  }
}


void CityMap::checkUndergroundTile(int i, int j)
{
  CityTile& tile = getTile(i, j);

  if (tile.pipeType)
  {
    return;
  }

  CityTile& t_n = getTile(i, j - 1);
  CityTile& t_s = getTile(i, j + 1);
  CityTile& t_e = getTile(i + 1, j);
  CityTile& t_w = getTile(i - 1, j);

  CityTile& t_ne = getTile(i + 1, j - 1);
  CityTile& t_se = getTile(i + 1, j + 1);
  CityTile& t_nw = getTile(i - 1, j - 1);
  CityTile& t_sw = getTile(i - 1, j + 1);

  char a1 = checkUndergroundTile(t_n, t_e, t_ne);
  char a2 = checkUndergroundTile(t_e, t_s, t_se);
  char a3 = checkUndergroundTile(t_w, t_n, t_nw);
  char a4 = checkUndergroundTile(t_s, t_w, t_sw);

  tile.un1 = a1;
  tile.un2 = a2;
  tile.un3 = a3;
  tile.un4 = a4;
}


int CityMap::checkUndergroundTile(const CityTile& a, const CityTile& b, const CityTile& c)
{
  if (a.pipeType && b.pipeType) return 1; // corner
  if (a.pipeType) return 2; // edge
  if (b.pipeType) return 3; // edge
  if (c.pipeType) return 4; // inner corner

  return 0; //  nothing
}


float CityMap::getTileValue(const CityTile& tile)
{
  int x = 1;
  if (tile.surfaceUtilityFlag & 1<<1) ++x;
  if (tile.surfaceUtilityFlag & 1<<2) ++x;
  if (tile.surfaceUtilityFlag & 1<<3) ++x;

  return    0.0f +
            0.25f * (float)x *
            (1.f - (float)tile.airPollution / 255.f);
}
