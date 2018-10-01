#pragma once


#include <vector>
#include <string>


class MenuBuild
{

public:

  MenuBuild();
  ~MenuBuild();


  void draw( std::vector< std::string >& loadFiles );

  int getNextAction() { return m_action; }
  void clearAction() { m_action = 0; }

  int m_groundState = 0;
  int m_tutorialState = 0;
  int m_pop = 0;
  int m_showOverlay = -1;
  bool menu_save = false;
  bool menu_load = false;
  bool menu_process = false;

  std::string m_cityName;

  int m_startGame = 0;
  int m_loadMap = 0;
  
  int m_clearMap = 0;

protected:

  void drawHeader();
  void drawBuild();
  void drawIntro( std::vector< std::string >& loadFiles );
  void drawMenuLoad( std::vector< std::string >& loadFiles );
  void drawMessage();

  int m_showIntro = 1;
  int m_action = 0;

  int m_newGame = 0;
  int m_loadGame = 0;
};
