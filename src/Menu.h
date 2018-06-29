#pragma once


#include "graphics.h"
#include <vector>


class Mesh;


class Menu
{
public:

  Menu();
  ~Menu();

  bool mouseMove(double xpos, double ypos);
  bool mouseButton(int button, int action, int mods);

  void draw(GLuint uniformDiffuse, GLuint uniformMVP);

  int getSelected() { return m_select; }

  bool hasMoney() { return m_money > 0; }
  void costMoney(int cost) { m_money -= cost; }
  void addMoney(int add) { m_money += add; }

protected:

  int m_hover = -1;
  int m_select = -1;

  std::vector<Mesh*> m_numbers;
  std::vector<Mesh*> m_words;

  int m_nTools = 5;

  // Resource
  int m_money = 1e6;
  int m_pop = 0;

};
