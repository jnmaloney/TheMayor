#pragma once


#include "graphics.h"
#include <vector>


class Mesh;


enum Actions
{
  ACTION_NONE = -1,
  ACTION_ROAD,
  ACTION_PARK,
  ACTION_RESI,
  ACTION_COMM,
  ACTION_WATER,
  ACTION_POWER2,
  ACTION_DATA2,
  ACTION_POWER,
  ACTION_DATA,
  ACTION_BULLDOZE,
};




class Menu
{
public:

  Menu();
  ~Menu();

  bool mouseMove(double xpos, double ypos);
  bool mouseButton(int button, int action, int mods);

  void draw(GLuint uniformDiffuse, GLuint uniformMVP);

  int getSelected() { return m_select; }
  int getEarthLayer() { return m_earthLayer; }
  int getHover() { return m_hover; }

  bool hasMoney() { return m_money > 0; }
  void costMoney(int cost) { m_money -= cost; }
  void addMoney(int add) { m_money += add; }

  void setPop(int a_pop);

  int m_message = 1;

  void advance();

protected:

  int m_hover = -1;
  int m_select = -1;

  std::vector<Mesh*> m_numbers;
  std::vector<Mesh*> m_words;
  std::vector<Mesh*> m_icons;
  std::vector<Mesh*> m_icons2; // Pipe layer
  std::vector<Mesh*> m_messages;

  int m_nTools = 10;
  bool m_showTools[10] =
  { true, true,
    true, false,
    false, false,
    false, false,
    false, true  };

  // Resource
  int m_money = 1e6;
  int m_pop = 0;

  int m_earthLayer = 1;

};
