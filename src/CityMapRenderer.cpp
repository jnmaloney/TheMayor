#include "CityMapRenderer.h"
#include "Texture.h"


CityMapRenderer::CityMapRenderer()
{

}


CityMapRenderer::~CityMapRenderer()
{

}


void CityMapRenderer::loadMeshes()
{
  // {
  //   Mesh* mesh = new Mesh();
  //   //mesh->beSquare();
  //   mesh->load_obj("data/square.obj");
  //   mesh->upload_obj();
  //   m_meshes.push_back(mesh);
  // }

  std::vector<const char*> mesh_files;
  mesh_files.push_back("data/square.obj");
  mesh_files.push_back("data/lightmap/tree_lightmap.obj");
  mesh_files.push_back("data/lightmap/house1_lightmap.obj");
  // mesh_files.push_back("data/HousesPackLowPoly_house1.obj");
  // mesh_files.push_back("data/HousesPackLowPoly_house2.obj");
  // mesh_files.push_back("data/HousesPackLowPoly_house3.obj");
  // mesh_files.push_back("data/HousesPackLowPoly_house4.obj");
  // mesh_files.push_back("data/HousesPackLowPoly_house5.obj");

  std::vector<const char*> texture_files;
  texture_files.push_back("data/RPGTileset.png");
  texture_files.push_back("data/lightmap/lightmap.png");
  texture_files.push_back("data/lightmap/lightmap_house1.png");
  // texture_files.push_back("data/HousesPackLowPoly_house1.png");
  // texture_files.push_back("data/HousesPackLowPoly_house2.png");
  // texture_files.push_back("data/HousesPackLowPoly_house3.png");
  // texture_files.push_back("data/HousesPackLowPoly_house4.png");
  // texture_files.push_back("data/HousesPackLowPoly_house5.png");

  for ( int i = 0; i < mesh_files.size(); ++i )
  {
    Mesh* mesh = new Mesh();

    mesh->load_obj(mesh_files[i]);
    mesh->upload_obj();

    mesh->texture = new Texture();
    mesh->texture->loadPng(texture_files[i]);

    m_meshes.push_back(mesh);
  }

  {
    Mesh* mesh = new Mesh();
    mesh->load_obj("data/ring.obj");
    mesh->upload_obj();
    m_meshes.push_back(mesh);
  }
}


void CityMapRenderer::generateQueue()
{
  m_renderQueue = RenderQueue();


  m_renderQueue.setProgram(m_rs->programObject);

  // g_renderQueue.setMesh(&g_meshTree);
  // g_renderQueue.setMVP(glm::translate(glm::mat4(1.0f), glm::vec3(1.f, 0.f, 0.f)));
  //
  // g_renderQueue.setMesh(&g_meshHouse);
  // g_renderQueue.setMVP(glm::translate(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 0.f)));

  for (int i = 0; i < 10; ++i)
  {
    for (int j = 0; j < 10; ++j)
    {
      // Tile
      m_renderQueue.setMesh(m_meshes[0]);
      m_renderQueue.setMVP(glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f)));

      // Tree
      if (m_cityMap->getTile(i, j).object_type == 1)
      {
        /*
        m_renderQueue.setMesh(m_meshes[1]);
        m_renderQueue.setMVP(
          glm::scale(
          glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f)
        ), glm::vec3(0.2f, 0.2f, 0.2f)));
        */
        CityTile& tile = m_cityMap->getTile(i, j);
        glm::mat4 local = glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f));

        m_renderQueue.setMesh(m_meshes[1]);

        glm::mat4 sublocal = glm::scale(local, glm::vec3(0.25f));

        glm::mat4 sublocal2 = glm::translate(sublocal, glm::vec3(1.0f, 1.0f, 0.0f));
        if (tile.treeAng1)
        {
          sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng1), glm::vec3(0.f, 0.f, 1.f));
          m_renderQueue.setMVP(sublocal2);
        }

        sublocal2 = glm::translate(sublocal, glm::vec3(1.0f, -1.0f, 0.0f));
        if (tile.treeAng2)
        {
          sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng2), glm::vec3(0.f, 0.f, 1.f));
          m_renderQueue.setMVP(sublocal2);
        }

        sublocal2 = glm::translate(sublocal, glm::vec3(-1.0f, 1.0f, 0.0f));
        if (tile.treeAng3)
        {
          sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng3), glm::vec3(0.f, 0.f, 1.f));
          m_renderQueue.setMVP(sublocal2);
        }

        sublocal2 = glm::translate(sublocal, glm::vec3(-1.0f, -1.0f, 0.0f));
        if (tile.treeAng4)
        {
          sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng4), glm::vec3(0.f, 0.f, 1.f));
          m_renderQueue.setMVP(sublocal2);
        }
      }
    }
  }
}


void CityMapRenderer::tick()
{
  //
  // Update the main queue
  //
  m_renderQueue.clear();
  m_renderQueue.setProgram(m_rs->programObject);

  for (int i = 0; i < m_cityMap->m_width; ++i)
  {
    for (int j = 0; j < m_cityMap->m_height; ++j)
    {
      // if (onscreen)
      float m_zoom = m_rs->m_zoom;
      float dx = i - m_rs->m_cameraX;
      float dy = j - m_rs->m_cameraY;

      float fx = 1.f / 145.f * 1920.f / m_rs->m_window.width;
      float fy = 1.f / 115.f * 1080.f / m_rs->m_window.height;

      if ((dx + dy + 1)*m_zoom*fy > -1.f && (dx + dy - 1)*m_zoom*fy < 1.f &&
          (dx - dy + 1)*m_zoom*fx > -1.f && (dx - dy - 1)*m_zoom*fx < 1.f)
      {
        // Draw Tile

        // Tile
        m_renderQueue.setMesh(m_meshes[0]);
        m_renderQueue.setMVP(glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f)));

        // Tree
        if (m_cityMap->getTile(i, j).object_type == 1)
        {
          /*
          m_renderQueue.setMesh(m_meshes[1]);
          m_renderQueue.setMVP(
            glm::scale(
            glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f)
          ), glm::vec3(0.2f, 0.2f, 0.2f)));
          */
          CityTile& tile = m_cityMap->getTile(i, j);
          glm::mat4 local = glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f));

          m_renderQueue.setMesh(m_meshes[1]);

          glm::mat4 sublocal = glm::scale(local, glm::vec3(0.25f));

          glm::mat4 sublocal2 = glm::translate(sublocal, glm::vec3(1.0f, 1.0f, 0.0f));
          if (tile.treeAng1)
          {
            sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng1), glm::vec3(0.f, 0.f, 1.f));
            sublocal2 = glm::scale(sublocal2, glm::vec3(0.45f));
            m_renderQueue.setMVP(sublocal2);
          }

          sublocal2 = glm::translate(sublocal, glm::vec3(1.0f, -1.0f, 0.0f));
          if (tile.treeAng2)
          {
            sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng2), glm::vec3(0.f, 0.f, 1.f));
            sublocal2 = glm::scale(sublocal2, glm::vec3(0.45f));
            m_renderQueue.setMVP(sublocal2);
          }

          sublocal2 = glm::translate(sublocal, glm::vec3(-1.0f, 1.0f, 0.0f));
          if (tile.treeAng3)
          {
            sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng3), glm::vec3(0.f, 0.f, 1.f));
            sublocal2 = glm::scale(sublocal2, glm::vec3(0.45f));
            m_renderQueue.setMVP(sublocal2);
          }

          sublocal2 = glm::translate(sublocal, glm::vec3(-1.0f, -1.0f, 0.0f));
          if (tile.treeAng4)
          {
            sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng4), glm::vec3(0.f, 0.f, 1.f));
            sublocal2 = glm::scale(sublocal2, glm::vec3(0.45f));
            m_renderQueue.setMVP(sublocal2);
          }
        }
      }
    }
  }

  //
  // Update the Cursor object
  //
  m_renderQueueAdd.clear();
  m_renderQueueAdd.setProgram(m_rs->programObject);
  m_renderQueueAdd.setMesh(m_meshes[3]); // Ring
  float x = (int)(m_rs->m_cursorX + 0.5f);
  float y = (int)(m_rs->m_cursorY + 0.5f);
  //m_renderQueueAdd.setMVP(glm::translate(glm::mat4(1.0f), glm::vec3(1.f * x, 1.f * y, 0.f)));
  m_renderQueueAdd.setMVP(
    //glm::scale(
      glm::translate(
        glm::mat4(1.0f),
        glm::vec3(1.f * x, 1.f * y, 0.1f))
      //glm::vec3(0.2f, 0.2f, 0.2f))
  );
}
