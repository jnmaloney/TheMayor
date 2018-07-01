
enum TILE_TYPE
{
  TILE_VOID = 0,
  TILE_ROAD,
  TILE_TREE,
  TILE_ZONE,  // Residential zone
  TILE_BUILD, // Special building
};


class CityTile
{
public:
  char isDefault = 0;
  unsigned char airPollution = 0;
  TILE_TYPE tileType = TILE_VOID;
  char data[8];
  char pipeType = 0;
  char foundational = 0;
  char pipeRend = 0;
  char pipeAng = 0;
  char un1 = 0;
  char un2 = 0;
  char un3 = 0;
  char un4 = 0;
  char utilityFlag = 0;
  char surfaceUtilityFlag = 0;
};


class TreeData
{
public:
  char treeAng1 = 0;
  char treeAng2 = 0;
  char treeAng3 = 0;
  char treeAng4 = 0;
  char park = 0;
};


class ZoneData
{
public:
  char surface = 0;
  char zone = 0;
  char object_type = 0;
  char buildAng1 = 0;
  char buildAng2 = 0;
};

class BuildData
{
public:
  char resourceType = 0;
  int resourceValue = 0;
};

class RoadData
{
public:
    char road = 0;
    char roadNum = 0;
    char roadAng = 0;
};
