#pragma once
#include <string>
#include <vector>

#include "graphics.h"

#include "CityMap.h"


class Mesh;
class Menu;


// Controls the shaders and render loops
class RenderSystem
{

public:
  RenderSystem();
  ~RenderSystem();

  void draw();


  // Camera move callback
  void move(double, double);
  void cursor_pos(double, double);
  void zoom(int);

  void setCityMap(CityMap* map) { m_cityMap = map; }

  bool performTileAction(int i);

  Menu* getMenu() { return m_menu; }
  static void drawObjModel_square();

  void hideToolCursor();
  void showToolCursor();

protected:

  // Passes -depr
  void draw1Pass();
  void draw2Pass();

  void drawUI();

  // Fullscreen present
  void RenderQuad();

  // Scene drawing
  void drawTile(int i, int j, glm::mat4 MVP);
  void drawObjModel_obj();
  void draw_bbox();

  //
  void checkRoadTile(int, int);
  void checkPipeTiles(int, int);
  void checkPipeTile(int, int);
  void checkUndergroundTile(int i, int j);
  int checkUndergroundTile(const CityTile& a, const CityTile& b, const CityTile& c);

  void drawCliffSide(glm::mat4 MVP, char i, bool h = true);


  // Vars

  long m_shader;
  GLuint programObject;

  long m_shader2;
  GLuint m_programObject2;

  unsigned int m_default_framebuffer = 0;

  // G-buffer stuff
  unsigned int m_gBuffer = 0; // FBO
  unsigned int m_depBuffer = 0;
  unsigned int m_norBuffer = 0;
  unsigned int m_colBuffer = 0;
  unsigned int m_posBuffer = 0;

  // View settings
  double m_cameraX = 50;
  double m_cameraY = 50;

  double m_cursorX = 0;
  double m_cursorY = 0;

  // City Map
  CityMap* m_cityMap = 0;

  int m_zoomFactor = 3;
  float m_zoom = 0.5 + 0.5 * (1<<3);

  // Meshes
  std::vector<Mesh*> road_meshes;
  std::vector<Mesh*> house_meshes;
  std::vector<Mesh*> resource_meshes;
  std::vector<Mesh*> pipe_meshes;
  std::vector<Mesh*> cliff_meshes;
  std::vector<Mesh*> tree_meshes;

  //
  Menu* m_menu = 0;

  bool m_showToolCursor = false;
};
