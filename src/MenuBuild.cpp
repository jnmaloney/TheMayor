#include "MenuBuild.h"
#include "imgui.h"
#include "WindowManager.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <locale>
#include <iostream>
#include <iomanip>
#include <sstream>


class comma_numpunct : public std::numpunct<char>
{
  protected:
    virtual char do_thousands_sep() const
    {
        return ' ';
    }

    virtual std::string do_grouping() const
    {
        return "\03";
    }
};


MenuBuild::MenuBuild()
{}


MenuBuild::~MenuBuild()
{}


  static int TextEditCallbackStub(ImGuiTextEditCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
  {
    return 0;
    // ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
    // return console->TextEditCallback(data);
  }


void MenuBuild::drawIntro( std::vector< std::string >& loadFiles )
{
  ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
  ImGui::Begin("Title", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());

  ImGui::PushFont(
    ImGui::GetIO().Fonts->Fonts[1]
  );

  ImGui::Text("The Mayor");
  ImGui::PopFont();

  if (m_newGame)
  {
    static char str0[128] = "";
    ImGui::Text("");
    ImGui::Text("Name your city");

    ImGui::PushItemWidth(300);
    ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
    ImGui::PopItemWidth();
    m_cityName = str0;

    ImGui::Text("");

    static bool show_tut = true;
    ImGui::Checkbox("Tutorial", &show_tut);

    ImGui::Text("Map Size"); ImGui::SameLine();
    enum Mode
    {
        Mode_Copy,
        Mode_Move,
        Mode_Swap
    };
    static int mode = 0;
    if (ImGui::RadioButton("Small", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
    if (ImGui::RadioButton("Medium", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
    if (ImGui::RadioButton("Large", mode == Mode_Swap)) { mode = Mode_Swap; }

    if (ImGui::Button("Start Game"))
    {
      if (m_cityName.length())
      {
        // Starting the game
        m_tutorialState = show_tut ? 1 : 0;
        m_showIntro = 0;
        m_startGame = 1;
      }
      else
      {
        ImGui::SameLine(); ImGui::Text("Give your city a name");
      }
    }

    if (ImGui::Button("< Back"))
    {
      m_newGame = 0;
    }

  }
  else if (m_loadGame)
  {
    ImGui::Text("");
    ImGui::Text("Load game");
    ImGui::Text("");

    for (auto i : loadFiles)
    {
      if (i.length() > 5 &&
          i.substr(i.length() - 5, i.length()) == ".city")
      {}
      else continue;

      if (ImGui::Button(i.substr(0, i.length() - 5).c_str()))
      {
        // Starting the game
        m_cityName = i.substr(0, i.length() - 5);
        m_loadMap = 1;
        m_showIntro = 0;
        m_startGame = 1;
      }
    }

    ImGui::Text("");

    if (ImGui::Button("< Back"))
    {
      m_loadGame = 0;
    }
  }
  else
  {
    ImGui::Text("A Game Jam Game in development");
    ImGui::Text("");
    ImGui::Text("v 1.2.1");
    ImGui::Text("");

    // if (ImGui::Button("Process"))
    // {
    //   menu_process = true;
    // }

    if (ImGui::Button("New Game"))
    {
      m_newGame = 1;
    }

    if (ImGui::Button("Load Game"))
    {
      m_loadGame = 1;
    }
  }

  // for (auto i : loadFiles)
  // {
  //   ImGui::Text(i.c_str());
  // }
  //
  // if (ImGui::Button("Start"))
  // {
  //   m_showIntro = 0;
  //   m_tutorialState = 1;
  // }
  // if (ImGui::Button("Skip Tutorial"))
  // {
  //   m_showIntro = 0;
  //   m_tutorialState = 0;
  // }

  ImGui::PopItemWidth();

  ImGui::End();
}



void MenuBuild::drawMenuLoad( std::vector< std::string >& loadFiles )
{
  ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
  ImGui::Begin("Load", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());

  ImGui::PushFont(
    ImGui::GetIO().Fonts->Fonts[1]
  );

  ImGui::Text("Load Game");
  ImGui::PopFont();

  {
    ImGui::Text("");
    ImGui::Text("Load game");
    ImGui::Text("");

    for (auto i : loadFiles)
    {
      if (i.length() > 5 &&
          i.substr(i.length() - 5, i.length()) == ".city")
      {}
      else continue;

      if (ImGui::Button(i.substr(0, i.length() - 5).c_str()))
      {
        // Starting the game
        m_cityName = i.substr(0, i.length() - 5);
        menu_load = 0;
        m_loadMap = 1;
        m_clearMap = 1;
      }
    }

    ImGui::Text("");

    if (ImGui::Button("< Back"))
    {
      menu_load = 0;
    }
  }

  ImGui::PopItemWidth();

  ImGui::End();
}


void MenuBuild::drawMessage()
{
  WindowManager& window = *WindowManager::getInstance();

  //ImVec2 size(150.f, 40.f);
  ImVec2 pos(window.width - 254.f, 60.f + 40.f + 12.f);
  ImVec2 pivot(0, 0);
  ImGui::SetNextWindowPos(pos, 0, pivot);
  //ImGui::SetNextWindowSize(size, 0);

  ImGui::Begin("Message", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
  ImGui::SameLine(220.0f, 1.f);
  //ImGui::Text(" ", size);
  ImGui::Text("");

  if (m_tutorialState == 1)
  {
    ImGui::TextWrapped("Welcome to your new city!\n\nWell, there's nothing here yet.\n\nUse the zoning tool to designate an area for citizens to live. Build roads to connect residential areas.");
  }
  else if (m_tutorialState == 2)
  {
    ImGui::TextWrapped("As the mayor, you are responsible for providing everything the city needs. Make sure there is a supply of fresh water by building a water tower. The range of the water tower can be extended with underground pipes.");
  }
  else if (m_tutorialState == 3)
  {
    ImGui::TextWrapped("The city needs a power station.\n\nYou can use the menu to view the connectivity of the city's utilities.");
  }
  else if (m_tutorialState == 4)
  {
    ImGui::TextWrapped("A power station produces high voltage electricity.\n\nTo distribute the power locally it must be connected to a substation.");
  }
  else if (m_tutorialState == 5)
  {
    ImGui::TextWrapped("Now you have a basic functioning city. Make sure your residents have jobs by designating a commercial zone.");
  }
  else if (m_tutorialState == 6)
  {
    ImGui::TextWrapped("You can increase the operating efficiency of businesses by making sure you have a functioning communications network.\n\nThe city's data infrastructure is built in two stages. The Data Hub feeds into the Data Node, which in turn supplies nearby buildings.\n\nPlace a Data Hub and Data Node and connect them underground.");
  }
  else if (m_tutorialState == 7)
  {
    ImGui::TextWrapped("Your city is expanding!\n\nReach 5 000 population.");
    if (ImGui::Button("End Tutorial"))
      m_tutorialState = 0;
  }

  ImGui::End();
}


void MenuBuild::drawHeader()
{
  WindowManager& window = *WindowManager::getInstance();
  ImVec2 pos(0.f, 0.f);
  ImVec2 pivot(0, 0);

  ImVec2 sizeA(10.f, 30.f);
  ImVec2 sizeB(150.f, 30.f);
  ImVec2 sizeC(180.f, 30.f);

  static std::locale comma_locale(std::locale(), new comma_numpunct());
  std::stringstream ss;
  ss.imbue(comma_locale);
  ss << std::fixed << m_pop;
  std::string pop = std::string("Pop: ") + ss.str();

  //ImGui::SetNextWindowPos(pos, 0, pivot);
  //ImGui::SetNextWindowSize(ImVec2(window.width, 0), 0);
  //ImGui::Begin("Header", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Save")) menu_save = true;
      if (ImGui::MenuItem("Load")) menu_load = true;
      ImGui::EndMenu();
    }

    float spacing1 = 0.5f * window.width - 0.5f * sizeB.x;
    int spacing2 = window.width - sizeC.x;

    ImGui::SameLine(0.0f, spacing1);
    ImGui::Text(pop.c_str(), sizeB);

    ImGui::SameLine(spacing2);
    const char* texts[] = {
        "Land Value",
        "Air Pollution",
        "Utility: Water",
        "Utility: Power",
        "Utility: Data",
      };

    const char* letters = "ABCDE";

    for (int n = 0; n < 5; n++)
    {
        char buf[32];
        sprintf(buf, " %c ", letters[n]);

        if (ImGui::Selectable(buf, m_showOverlay == n, 0, ImVec2(25, 24)))
        {
          m_showOverlay = (m_showOverlay == n) ? -1 : n; // Toggle
          m_action = 0;
        }
        if (ImGui::IsItemHovered())
        {
          ImGui::SetTooltip(texts[n]);
        }
        ImGui::SameLine();
    }

    ImGui::EndMainMenuBar();
  }
  //ImGui::End();
  return;

  float spacing1 = 0.5f * window.width - sizeA.x - 0.5f * sizeB.x;
  float spacing2 = 0.5f * window.width - 0.5f * sizeB.x - sizeC.x;

  //ImGui::Text("", sizeA);

//  if (ImGui::BeginMainMenuBar())
  {
    //if (ImGui::BeginMenu("File"))
    if (ImGui::Button("File"))
    {
      ImGui::OpenPopup("FilePopup");
      // ImGui::MenuItem("Save", NULL, &menu_save);
      // ImGui::MenuItem("Load", NULL, &menu_load);
    }
      if (ImGui::BeginPopup("FilePopup"))
      {
        if (ImGui::MenuItem("Save")) menu_save = true;
        if (ImGui::MenuItem("Load")) menu_load = true;
        ImGui::EndPopup();
      }
      //ImGui::EndMenu();
//    ImGui::EndMainMenuBar();
  }

  ImGui::SameLine(0.0f, spacing1);
  //ImGui::Text("Pop: 200 000", sizeB);
  ImGui::Text(pop.c_str(), sizeB);
  ImGui::SameLine(0.0f, spacing2);
  ImGui::Text("", sizeC);

  const char* texts[] = {
      "Land Value",
      "Air Pollution",
      "Utility: Water",
      "Utility: Power",
      "Utility: Data",
    };

  for (int n = 0; n < 5; n++)
  {
      char buf[32];
      sprintf(buf, ":%d:", n);
      ImGui::SameLine();

      if (ImGui::Selectable(buf, m_showOverlay == n, 0, ImVec2(25, 24)))
      {
        m_showOverlay = (m_showOverlay == n) ? -1 : n; // Toggle
        m_action = 0;
      }
      if (ImGui::IsItemHovered())
      {
        ImGui::SetTooltip(texts[n]);
      }
  }

  ImGui::End();
}


void MenuBuild::draw( std::vector< std::string >& loadFiles )
{
  if (m_showIntro)
  {
    drawIntro(loadFiles);
  }
  else if (menu_load)
  {
    drawMenuLoad(loadFiles);
  }
  else
  {
    drawBuild();
    drawHeader();
    if (m_tutorialState) drawMessage();
  }


}


void MenuBuild::drawBuild()
{
  const char* texts[] = {
      "Road", "Build a small road",
      "R Z", "Residential Zone",
      "C Z", "Commercial Zone",
      "Park", "An area of green space",
      "Water", "Build a water tower",
      "Pwr Gen", "Power Generator",
      "Pwr Sub", "Power Substation",
      "Data Hub", "Central data system",
      "Data Node", "Local data system",
      "Bulldoze", "Flatten it",
  };

  const char* texts2[] = {
      "Water", "",
      "Power", "",
      "Data", "",
      "Bulldoze", "",
  };

  {
    WindowManager& window = *WindowManager::getInstance();

    //ImVec2 size(150.f, 40.f);
    ImVec2 pos(window.width - 154.f, 40.f + 12.f);
    ImVec2 pivot(0, 0);
    ImGui::SetNextWindowPos(pos, 0, pivot);

    ImGui::Begin("Ground", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (m_groundState == 0)
    {
      if (ImGui::Button("Surface"))
      {
        m_groundState = 1;
        m_action = 0;
      }
    }
    else
    {
      if (ImGui::Button("Underground"))
      {
        m_groundState = 0;
        m_action = 0;
      }
    }

    ImGui::End();
  }

  bool doCols = false; //(clicked > -1);

  ImVec2 pos(12.f, 40.f + 12.f);
  //static ImVec2 sizeBase(0.f, 0.f);
  ImVec2 pivot(0,0);
  ImVec2 size(80, 30);
  ImGui::SetNextWindowPos(pos, 0, pivot);
  // if (doCols)
  //   ImGui::SetNextWindowSize(ImVec2(2*96, 380), 0);
  // else
  //   ImGui::SetNextWindowSize(ImVec2(96, 380), 0);
  // if (sizeBase.y > 1.f)
  // {
  //   ImVec2 size;
  //   size.x = sizeBase.x * (doCols ? 1.f : 0.5f);
  //   size.y = sizeBase.y;
  //   ImGui::SetNextWindowSize(size);
  // }

  ImGui::Begin("Build", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

  if (doCols)
  {
    //ImGui::Columns(2, "mixed");
    ImGui::Columns(2);
    //ImGui::Separator();
  }

  ImGui::Text("Build");

  if (m_groundState == 0)
    for (int i = 0; i < 10; ++i)
    {
      // Check which buttons to skip
      if (m_tutorialState)
      {
        if (m_tutorialState == 1)
        {
          if (i != 0 && i != 1 && i != 9) continue;
        }
        if (m_tutorialState == 2)
        {
          if (i != 0 && i != 1 && i != 4 && i != 9) continue;
        }
        if (m_tutorialState == 3)
        {
          if (i != 0 && i != 1 && i != 4 && i != 5 && i != 9) continue;
        }
        if (m_tutorialState == 4)
        {
          if (i != 0 && i != 1 && i != 4 && i != 5 && i != 6 && i != 9) continue;
        }
        if (m_tutorialState == 5)
        {
          if (i == 7 || i == 8) continue;
        }
      }

      // Do stuff

      int j = 4;
      if (m_action == i + 1) j = 2;

      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(j/7.0f, 0.6f, 0.6f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(j/7.0f, 0.7f, 0.7f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(j/7.0f, 0.8f, 0.8f));

      if (ImGui::Button(texts[2*i], size))
      {
        if (m_action == i + 1) m_action = 0;
        else m_action = i + 1;
      }
      if (ImGui::IsItemHovered())
      {
        ImGui::SetTooltip(texts[2*i + 1]);
      }

      ImGui::PopStyleColor(3);
    }
  else if (m_groundState == 1)
    for (int i = 0; i < 4; ++i)
    {
      int j = 4;
      if (m_action == i + 101) j = 2;

      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(j/7.0f, 0.6f, 0.6f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(j/7.0f, 0.7f, 0.7f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(j/7.0f, 0.8f, 0.8f));

      if (ImGui::Button(texts2[2*i], size))
      {
        if (m_action == i + 101) m_action = 0;
        else m_action = i + 101;
      }
      if (ImGui::IsItemHovered())
      {
        ImGui::SetTooltip(texts2[2*i + 1]);
      }

      ImGui::PopStyleColor(3);
    }

  //ImGui::End();
  if (doCols) ImGui::NextColumn();

  if (doCols)
  {
  //   pos.x += 75;
  //   ImGui::SetNextWindowPos(pos, 0, pivot);
  //
  //   ImGui::Begin(texts[2*clicked], NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);


    for (int i = 0; i < 9; ++i)
    {
      if (ImGui::Button(""))
      {
      }
      if (ImGui::IsItemHovered())
      {
        ImGui::SetTooltip("");
      }
    }
  }

  ImGui::End();

  // if (sizeBase.y < 1.f)
  // {
  //   sizeBase = ImGui::GetWindowSize();
  // }

}
