#pragma once


class MenuBuild
{

public:

  MenuBuild();
  ~MenuBuild();


  void draw();

  int getNextAction() { return m_action; }
  void clearAction() { m_action = 0; }

  int m_groundState = 0;
  int m_tutorialState = 0;
  int m_pop = 0;
  int m_showOverlay = -1;

protected:

  void drawHeader();
  void drawBuild();
  void drawIntro();
  void drawMessage();

  int m_showIntro = 1;
  int m_action = 0;


};
