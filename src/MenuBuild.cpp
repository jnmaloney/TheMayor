#include "MenuBuild.h"
#include "imgui.h"
#include "WindowManager.h"


MenuBuild::MenuBuild()
{}


MenuBuild::~MenuBuild()
{}


void MenuBuild::drawHeader()
{
  WindowManager& window = *WindowManager::getInstance();
  ImVec2 pos(0.f, 0.f);
  ImVec2 pivot(0,0);

  ImVec2 sizeA(10.f, 30.f);
  ImVec2 sizeB(150.f, 30.f);
  ImVec2 sizeC(110.f, 30.f);

  ImGui::SetNextWindowPos(pos, 0, pivot);
  ImGui::SetNextWindowSize(ImVec2(window.width, 40), 0);
  ImGui::Begin("Header", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  float spacing1 = 0.5f * window.width - sizeA.x - 0.5f * sizeB.x;
  float spacing2 = 0.5f * window.width - 0.5f * sizeB.x - sizeC.x;

  ImGui::Text("xE", sizeA);
  ImGui::SameLine(0.0f, spacing1);
  ImGui::Text("Pop: 200 000", sizeB);
  ImGui::SameLine(0.0f, spacing2);
  ImGui::Text("w: 38 000", sizeC);



  ImGui::End();

}


void MenuBuild::draw()
{
  const char* texts[] = {
      "Road", "Build a small road",
      "R Z", "Residential Zone",
      "C Z", "Commercial Zone",
      "Park", "An area of green space",
      "Water", "Build a water tower",
      "Pwr Sub", "Power Substation",
      "Pwr Gen", "Power Generator",
      "Data Hub", "",
      "Data Node", "",
      "Bulldoze", "",
  };

  static int clicked = -1;
  bool doCols = false; //(clicked > -1);

  ImVec2 pos(12.f, 40.f + 12.f);
  //static ImVec2 sizeBase(0.f, 0.f);
  ImVec2 pivot(0,0);
  ImVec2 size(80, 30);
  ImGui::SetNextWindowPos(pos, 0, pivot);
  if (doCols)
    ImGui::SetNextWindowSize(ImVec2(2*96, 380), 0);
  else
    ImGui::SetNextWindowSize(ImVec2(96, 380), 0);
  // if (sizeBase.y > 1.f)
  // {
  //   ImVec2 size;
  //   size.x = sizeBase.x * (doCols ? 1.f : 0.5f);
  //   size.y = sizeBase.y;
  //   ImGui::SetNextWindowSize(size);
  // }

  ImGui::Begin("Build", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);



  if (doCols)
  {
    //ImGui::Columns(2, "mixed");
    ImGui::Columns(2);
    //ImGui::Separator();
  }

  ImGui::Text("Build");

  for (int i = 0; i < 10; ++i)
  {
    int j = 4;
    if (clicked == i) j = 2;

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(j/7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(j/7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(j/7.0f, 0.8f, 0.8f));

    if (ImGui::Button(texts[2*i], size))
    {
      if (clicked == i) clicked = -1;
      else clicked = i;
    }
    if (ImGui::IsItemHovered())
    {
      ImGui::SetTooltip(texts[2*i + 1]);
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
