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

  void generateQueue(int groundState);

  void tick();

  RenderQueue m_renderQueue;
  RenderQueue m_renderQueueAdd;

  CityMap* m_cityMap = 0;
  RenderSystem* m_rs = 0;

protected:

  void generateQueue0();
  void generateQueue1();
  void generateQueue_overlay1();
  void generateQueue_overlay2();
  void generateQueue_overlay3();
  void generateQueue_overlay4();
  void generateQueue_overlay5();

  void drawTile(CityTile& tile, glm::mat4& local);
  void drawCliffSide(glm::mat4 MVP, char i, bool hideOutDebug);

  std::vector<Mesh*> m_meshes;
};
