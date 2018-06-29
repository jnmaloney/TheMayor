#include "Menu.h"
#include "RenderSystem.h"
#include <iostream>
#include "Mesh.h"
#include <stdlib.h>


Menu::Menu()
{
  for (int i = 0; i < 10; ++i)
  {
    std::string filename = "data/number" + std::to_string(i) + ".obj";
    Mesh* pMeshLoad = new Mesh();
    pMeshLoad->load_obj(filename.c_str());
    pMeshLoad->upload_obj();
    m_numbers.push_back(pMeshLoad);
  }


  std::string names[] = {
    "data/wordRoad.obj",
    "data/wordResi.obj",
    "data/wordPower.obj",
    "data/wordPark.obj",
    "data/wordBulldoze.obj",
  };
  for (int i = 0; i < 5; ++i)
  {
    Mesh* pMeshLoad = new Mesh();
    pMeshLoad->load_obj(names[i].c_str());
    pMeshLoad->upload_obj();
    m_words.push_back(pMeshLoad);
  }

}


Menu::~Menu()
{


}


bool Menu::mouseMove(double xpos, double ypos)
{
  int pixelWidth = 90;
  int pixelHeight = 90;

  //std::cout<<xpos << ", "<<ypos<<std::endl;

  for (int i = 0; i < m_nTools; ++i)
  {
    float x = (i % 2) ? 99.5 : 29.5;
    float y = (i - (i % 2)) * 32.0 + 35.5;


    if (xpos > x && xpos < x + 45 &&
        ypos > y && ypos < y + 45)
    {
      m_hover = i;

      return true;
    }
  }

  m_hover = -1;
  return false;
}


bool Menu::mouseButton(int button, int action, int mods)
{
  if (action == 1)
  {
    if (m_hover != -1)
    {
      if (m_select == m_hover) m_select = -1;
      else m_select = m_hover;
      return true;
    }
  }

  return false;
}


void Menu::draw(GLuint uniformDiffuse, GLuint uniformMVP)
{
  int pixelWidth = 90;
  int pixelHeight = 90;
  int screen_width = 1280;
  int screen_height = 720;

  float colour1[] = { 238.f / 255.f, 88.f / 255.f,  88.f / 255.f };
  float colour2[] = { 221.f / 255.f, 221.f / 255.f,  94.f / 255.f };
  float colour3[] = { 158.f / 255.f, 158.f / 255.f,  158.f / 255.f };

  for (int i = 0; i < m_nTools; ++i)
  {
    float x = (i % 2) ? 238.5 : 110.5;
    float y = (i - (i % 2)) * 64.0 + 110.5;

    if (i == m_select) glUniform3fv(uniformDiffuse, 1, colour1);
    else if (i == m_hover) glUniform3fv(uniformDiffuse, 1, colour2);
    else glUniform3fv(uniformDiffuse, 1, colour3);

    glm::mat4 Model =
      glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 1.0, 0)) *
      glm::scale(glm::mat4(1.0f), glm::vec3(1.0 / screen_width,  1.0 / screen_height, 1.0)) *
      glm::translate(glm::mat4(1.0f), glm::vec3(x, -y, 0)) *
      glm::scale(glm::mat4(1.0f), glm::vec3(pixelWidth, pixelHeight, 1.0));

    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &Model[0][0]);

    RenderSystem::drawObjModel_square();

    Model =
      glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 1.0, 0)) *
      glm::scale(glm::mat4(1.0f), glm::vec3(1.0 / screen_width,  1.0 / screen_height, 1.0)) *
      glm::translate(glm::mat4(1.0f), glm::vec3(x-38, -y, 0)) *
      glm::scale(glm::mat4(1.0f), glm::vec3(pixelWidth, -pixelHeight, 1.0));
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &Model[0][0]);
    m_words[i]->draw(uniformDiffuse);
  }

}
