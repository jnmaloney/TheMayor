#pragma once


class MenuBuild
{

public:

  MenuBuild();
  ~MenuBuild();

  void drawHeader();

  void draw();

  int getNextAction() { return m_action; }

protected:

  int m_action = 0;

};
