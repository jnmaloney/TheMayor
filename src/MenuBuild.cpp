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


void MenuBuild::drawIntro()
{
  ImGui::Begin("Header", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  ImGui::PushFont(
    ImGui::GetIO().Fonts->Fonts[1]
  );

  ImGui::Text("The Mayor");
  ImGui::PopFont();

  ImGui::Text("A Game Jam Game in development");

  if (ImGui::Button("Start"))
  {
    m_showIntro = 0;
    m_tutorialState = 1;
  }
  if (ImGui::Button("Skip Tutorial"))
  {
    m_showIntro = 0;
    m_tutorialState = 0;
  }

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
  ImVec2 sizeC(110.f, 30.f);

  static std::locale comma_locale(std::locale(), new comma_numpunct());
  std::stringstream ss;
  ss.imbue(comma_locale);
  ss << std::fixed << m_pop;
  std::string pop = std::string("Pop: ") + ss.str();

  ImGui::SetNextWindowPos(pos, 0, pivot);
  ImGui::SetNextWindowSize(ImVec2(window.width, 40), 0);
  ImGui::Begin("Header", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  float spacing1 = 0.5f * window.width - sizeA.x - 0.5f * sizeB.x;
  float spacing2 = 0.5f * window.width - 0.5f * sizeB.x - sizeC.x;

  ImGui::Text("", sizeA);
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


void MenuBuild::draw()
{
  if (m_showIntro)
  {
    drawIntro();
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
      "Data Hub", "",
      "Data Node", "",
      "Bulldoze", "",
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
