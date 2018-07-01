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


  {
    std::string names[] = {
      "data/wordRoad.obj",
      "data/wordPark.obj",
      "data/wordResiZone.obj",
      "data/wordCommZone.obj",
      "data/wordWaterTower.obj",
      "data/wordPowerGen.obj",
      "data/wordDataHub.obj",
      "data/wordPowerSub.obj",
      "data/wordDataNode.obj",
      "data/wordBulldoze.obj",
    };
    for (int i = 0; i < m_nTools; ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(names[i].c_str());
      pMeshLoad->upload_obj();
      m_words.push_back(pMeshLoad);
    }
  }

  {
    std::string names[] = {
      "data/iconPower.obj",
      "data/iconRaindrop.obj",
      "data/iconData.obj",
      "data/wordBulldoze.obj",
    };
    for (int i = 0; i < 4; ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(names[i].c_str());
      pMeshLoad->upload_obj();
      m_icons2.push_back(pMeshLoad);
    }
  }

  {
    std::string names[] = {
      "data/IconCloudDay.obj",
      "data/IconGrass1.obj",
      "data/IconPipes.obj",
    };
    for (int i = 0; i < 3; ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(names[i].c_str());
      pMeshLoad->upload_obj();
      m_icons.push_back(pMeshLoad);
    }
  }

  {
    std::string names[] = {
      "data/message1.obj",
      "data/message2.obj",
      "data/message3.obj",
      "data/message4.obj",
      "data/message5.obj",
      "data/message6.obj",
    };
    for (int i = 0; i < 6; ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(names[i].c_str());
      pMeshLoad->upload_obj();
      m_messages.push_back(pMeshLoad);
    }
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

  if (m_earthLayer == 1)  //
  {
    for (int i = 0; i < m_nTools; ++i)
    {
      if (!m_showTools[i]) continue;

      float x = (i % 2) ? 99.5 : 29.5;
      float y = (i - (i % 2)) * 32.0 + 35.5;


      if (xpos > x && xpos < x + 45 &&
          ypos > y && ypos < y + 45)
      {
        m_hover = i;

        return true;
      }
    }
  }
  else if (m_earthLayer == 2)  // Underground
  {
    for (int i = 0; i < 4; ++i)
    {
      float x = 29.5;
      float y = (i) * 64.0 + 35.5;

      if (xpos > x && xpos < x + 45 &&
          ypos > y && ypos < y + 45)
      {
        m_hover = i;

        return true;
      }
    }
  }

  for (int i = 0; i < 3; ++i)
  {
    float x = 1280.f - 80.5;
    float y = i * 64.0 + 30.5;

    if (xpos > x - 25 && xpos < x + 50 &&
        ypos > y + 10 && ypos < y + 60)
    {
      m_hover = 100 + i;

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
    if (m_hover != -1 && m_hover < 100)
    {
      if (m_select == m_hover) m_select = -1;
      else m_select = m_hover;
      return true;
    }

    if (m_hover != -1 && m_hover >= 100)
    {
      m_earthLayer = m_hover - 100;

      // Unselect tool
      m_select = -1;

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

  // Left Menu
  if (m_earthLayer == 1) // Surface
  {
    for (int i = 0; i < m_nTools; ++i)
    {
      if (!m_showTools[i]) continue;

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
  else if (m_earthLayer == 2)  // Underground
  {
    for (int i = 0; i < 4; ++i)
    {
      float x = 110.5;
      float y = (i) * 128.0 + 110.5;

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
        glm::translate(glm::mat4(1.0f), glm::vec3(x-38, -y+38, 0)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1600.0, -1600.0, 1.0));

      if (i == 3)
        Model =
          glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 1.0, 0)) *
          glm::scale(glm::mat4(1.0f), glm::vec3(1.0 / screen_width,  1.0 / screen_height, 1.0)) *
          glm::translate(glm::mat4(1.0f), glm::vec3(x-38, -y, 0)) *
          glm::scale(glm::mat4(1.0f), glm::vec3(pixelWidth, -pixelHeight, 1.0));

      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &Model[0][0]);
      m_icons2[i]->draw(uniformDiffuse);
    }
  }


  // Layer of Earth
  for (int i = 0; i < 3; ++i)
  {
    float x = 2.0f * 1280.f - 160.5;
    float y = i * 128.0 + 60.5;

    if (i == m_earthLayer)        m_icons[i]->setColourSceme(0);
    else if (i == m_hover - 100)  m_icons[i]->setColourSceme(7);
    else                          m_icons[i]->setColourSceme(6);

    glm::mat4 Model;
    Model =
      glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 1.0, 0)) *
      glm::scale(glm::mat4(1.0f), glm::vec3(1.0 / screen_width,  1.0 / screen_height, 1.0)) *
      glm::translate(glm::mat4(1.0f), glm::vec3(x-38, -y, 0)) *
      glm::scale(glm::mat4(1.0f), glm::vec3(3000.0, -3000.0, 1.0));
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &Model[0][0]);
    m_icons[i]->draw(uniformDiffuse);
  }


  // Population
  float offset = 0.f;
  int place = 0;
  while (m_pop)
  {
      char digit = 0;
      int n = m_pop / 10;
      digit = m_pop - n * 10;
      m_pop = n;
      if (place % 3 == 0) offset += 0.5f;
      place += 1;
      offset += 1.f;

      float x = 1280.f - offset * 16.f;
      float y = 42.f;
      glm::mat4       Model =
              glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 1.0, 0)) *
              glm::scale(glm::mat4(1.0f), glm::vec3(1.0 / screen_width,  1.0 / screen_height, 1.0)) *
              glm::translate(glm::mat4(1.0f), glm::vec3(x-38, -y, 0)) *
              glm::scale(glm::mat4(1.0f), glm::vec3(pixelWidth, -pixelHeight, 1.0));

      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &Model[0][0]);
      m_numbers[digit]->draw(uniformDiffuse);
  };

  // Tutorial / Story
  if (m_message)
  {
    float x = 2.f * 1280.f - 475.f;
    float y = 660.f;
    glm::mat4       Model =
            glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 1.0, 0)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(1.0 / screen_width,  1.0 / screen_height, 1.0)) *
            glm::translate(glm::mat4(1.0f), glm::vec3(x-38, -y, 0)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(pixelWidth, -pixelHeight, 1.0));

    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &Model[0][0]);
    m_messages[m_message - 1]->draw(uniformDiffuse);
  }
}


void Menu::setPop(int a_pop)
{
  m_pop = a_pop;
}


void Menu::advance()
{
  ++m_message;
  if (m_message == 6) m_message = 0;

  if (m_message == 2) { m_showTools[4] = true; }
  if (m_message == 3) { m_showTools[5] = true; m_showTools[7] = true; }
  if (m_message == 4) { m_showTools[3] = true; }
  if (m_message == 5) { m_showTools[6] = true; m_showTools[8] = true; }
}
