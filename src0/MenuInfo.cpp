#include "MenuInfo.h"
#include "imgui.h"
#include "WindowManager.h"
#include "Texture.h"


MenuInfo::MenuInfo()
{}


MenuInfo::~MenuInfo()
{}


void MenuInfo::draw()
{
  //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1, 0.1, 0.1, 0.7));

  ImVec2 pos(42.f, 42.f);
  ImVec2 pivot(0, 0);
  ImGui::SetNextWindowPos(pos, 0, pivot);
  ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  drawLocation();

  ImGui::End(); // Window

  //ImGui::PopStyleColor();

}


void MenuInfo::drawLocation()
{
  ImGui::Text("%s", "Location:");
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
  ImGui::Text("%s", "Brisbane");
  ImGui::PopFont();
  ImGui::Text("%s", "Postcode:");
  ImGui::Text("%s", "lat, long:");
  ImGui::Text("%s", "Temperature:");
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
  ImGui::Text("%s", "30");
  ImGui::PopFont();
  ImGui::Text("%s", "Cloudy:");
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
  ImGui::Text("%s", "None");
  ImGui::PopFont();
  ImGui::Text("%s", "Solar Intensity:");
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
  ImGui::Text("%s", "1.0");
  ImGui::PopFont();
  ImGui::Text("%s", "Daylight Hours:");
  ImGui::Text("%s", "06:00");
  ImGui::Text("%s", "21:59");
}
