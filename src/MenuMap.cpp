#include "MenuMap.h"
#include "imgui.h"
#include "WindowManager.h"
#include "Texture.h"


MenuMap::MenuMap()
{}


MenuMap::~MenuMap()
{}


void MenuMap::draw(Texture* array[][10])
{
  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

  ImVec2 pos(0.f, 0.f);
  ImVec2 pivot(0, 0);
  ImGui::SetNextWindowPos(pos, 0, pivot);
  ImGui::SetNextWindowSize(ImVec2(WindowManager::getInstance()->width, WindowManager::getInstance()->height));
  ImGui::Begin("Dialog", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);


  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      Texture* t = array[i][j];
      ImTextureID my_tex_id = (ImTextureID)t->getID();
      float my_tex_w = 256;
      float my_tex_h = 256;

      if (my_tex_id)
      {
        //ImGui::SetCursorScreenPos(ImVec2(WindowManager::getInstance()->width - 256 * i, 0 + j * 256));
        //ImGui::SetCursorScreenPos(ImVec2(0, 0));
        ImGui::SetCursorScreenPos(ImVec2(256 * (i), 0 + j * 256));
        ImGui::Image(
          my_tex_id,
          ImVec2(my_tex_w, my_tex_h),
          ImVec2(0,0),
          ImVec2(1,1),
          ImColor(255,255,255,255),
          ImColor(255,255,255,0));
      }
    }
  }

  ImGui::End(); // Window
  ImGui::PopStyleVar(4);
}
