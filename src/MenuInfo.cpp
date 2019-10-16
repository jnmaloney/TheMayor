#include "MenuInfo.h"
#include "imgui.h"
#include "WindowManager.h"
#include "Texture.h"
#include "time.h"


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
  //ImGui::Text("%s", "Brisbane");
  static char InputBuf[256] = "";
  if (ImGui::InputText( "",
                        InputBuf,
                        IM_ARRAYSIZE(InputBuf),
                        ImGuiInputTextFlags_EnterReturnsTrue,
                        NULL,
                        NULL))
  {
    m_location = std::string(InputBuf);
    resetLocation = 1;
  }
  ImGui::SetKeyboardFocusHere(-1);
  ImGui::PopFont();
  ImGui::Text("%s", "Postcode: ----");
  ImGui::Text("latitude \t%.3f", m_lat);
  ImGui::Text("longitude \t%.3f", m_lon);
  ImGui::Text("%s ", "Temperature:");
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
  ImGui::Text("%.1f°C", m_temp - 272.15);
  ImGui::PopFont();
  ImGui::Text("%s", "Cloud Cover:");
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
  ImGui::Text("%.0f%%", m_clouds);
  ImGui::PopFont();
  ImGui::Text("%s", "Visibility:");
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
  ImGui::Text("%.1f%%", 0.01 * m_visibility);
  ImGui::PopFont();
  ImGui::Text("%s", "Solar Intensity:");
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
  ImGui::Text("%s", "---");
  ImGui::PopFont();
  ImGui::Text("%s", "Daylight Hours:");
  ImGui::Text("%s", asctime(localtime((time_t*)&m_sunrise)));
  ImGui::Text("%s", asctime(localtime((time_t*)&m_sunset)));
}
