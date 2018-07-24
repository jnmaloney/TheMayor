#pragma once


#include "RenderQueue.h"
#include "CityMap.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include <vector>


class CityMapRenderer
{
public:
  CityMapRenderer();
  ~CityMapRenderer();

  void loadMeshes();

  void generateQueue();

  void tick();

  RenderQueue m_renderQueue;
  RenderQueue m_renderQueueAdd;

  CityMap* m_cityMap = 0;
  RenderSystem* m_rs = 0;

protected:

  std::vector<Mesh*> m_meshes;
};
