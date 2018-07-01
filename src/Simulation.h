#pragma once


class CityMap;


class Simulation
{
public:
  static void doSkyStep(CityMap* a_map);

  static void doConnectivity(CityMap* a_map, int resource);


protected:

  static void doSurfaceUtilities(CityMap* a_map, int resource);
  static void engageRadius(CityMap* a_map, int i, int j, int r, int resource);

  static void connect(CityMap* a_map, int i, int j, int resource);
};
