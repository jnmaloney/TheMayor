#pragma once


#include "Mesh.h"
#include <vector>
#include <map>


class MeshManager
{
public:

  static MeshManager* get();

  MeshManager();
  ~MeshManager();

  void init();

  int getIndex(int a, int b);

  Mesh* getMesh(int index);

  enum M
  {
    NONE = 0,
    HOUSES,
    UTILITY,
    COMM,
    PIPE,
    CLIFF,
    OTHER
  };

protected:

  std::map<int, int> m_indexMap;
  std::vector<Mesh*> m_meshes;

};
