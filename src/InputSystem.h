#pragma once


#include "RenderSystem.h"
#include "CityMap.h"
#include "Menu.h"


class InputSystem
{
public:
  InputSystem(RenderSystem* rs);
  ~InputSystem();

  void cursor_pos_callback(double xpos, double ypos);
  void mouse_button_callback(int button, int action, int mods);

  void setCityMap(CityMap* map) { m_cityMap = map; }

  // Step through the story logic
  void frame();

protected:

  RenderSystem* m_rs;
  Menu* m_menu;

  double m_cursor_x = 0.0;
  double m_cursor_y = 0.0;
  double m_clickstart_x = 0.0;
  double m_clickstart_y = 0.0;
  int m_clickflags = 0;

  CityMap* m_cityMap = 0;

  bool m_allowDrag = false;

};
