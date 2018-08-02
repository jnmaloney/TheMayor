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
  mesh_files.push_back("data/HousesPackLowPoly_house1.obj");
  mesh_files.push_back("data/3D Road Pack/Models/roadTile_277-edit2.obj");
  mesh_files.push_back("data/3D Road Pack/Models/roadTile_299-edit2.obj");
  mesh_files.push_back("data/3D Road Pack/Models/roadTile_148-edit2.obj");
  mesh_files.push_back("data/3D Road Pack/Models/roadTile_260-edit2.obj");
  mesh_files.push_back("data/square.obj");
  mesh_files.push_back("data/square.obj");
  mesh_files.push_back("data/HousesPackLowPoly_house2.obj");
  mesh_files.push_back("data/HousesPackLowPoly_house3.obj");
  mesh_files.push_back("data/HousesPackLowPoly_house4.obj");
  mesh_files.push_back("data/HousesPackLowPoly_house5.obj");
  mesh_files.push_back("data/WaterTank-edit.obj");
  mesh_files.push_back("data/Generator.obj");
  mesh_files.push_back("data/EngineHouse.obj");
  mesh_files.push_back("data/SatReceiver.obj");
  mesh_files.push_back("data/cellphone.obj");
  mesh_files.push_back("data/pipe.obj");
  mesh_files.push_back("data/pipebend.obj");
  mesh_files.push_back("data/pipeT.obj");
  mesh_files.push_back("data/pipe4ways.obj");
  mesh_files.push_back("data/CliffBottom0.obj");
  mesh_files.push_back("data/CliffBottomCorner0.obj");
  mesh_files.push_back("data/square.obj");
  mesh_files.push_back("data/square.obj");
  mesh_files.push_back("data/comm_s_1.obj");
  mesh_files.push_back("data/comm_s_2.obj");
  mesh_files.push_back("data/comm_s_3.obj");
  mesh_files.push_back("data/comm_m_1.obj");
  mesh_files.push_back("data/comm_m_2.obj");
  mesh_files.push_back("data/comm_m_3.obj");
  mesh_files.push_back("data/comm_m_4.obj");
  mesh_files.push_back("data/comm_l_1.obj");
  mesh_files.push_back("data/comm_l_2.obj");
  mesh_files.push_back("data/comm_l_3.obj");
  mesh_files.push_back("data/comm_l_4.obj");
  mesh_files.push_back("data/ring.obj");
  mesh_files.push_back("data/Oak.obj");
  mesh_files.push_back("data/SchemeDown.obj");
  mesh_files.push_back("data/SchemeUp.obj");

  std::vector<const char*> texture_files;
  texture_files.push_back("data/RPGTileset.png");
  texture_files.push_back("data/lightmap/lightmap.png");
  texture_files.push_back("data/house1.png");
  texture_files.push_back(0);
  texture_files.push_back(0);
  texture_files.push_back(0);
  texture_files.push_back(0);
  texture_files.push_back(0);
  texture_files.push_back(0);
  texture_files.push_back("data/house2.png");
  texture_files.push_back("data/house3.png");
  texture_files.push_back("data/house4.png");
  texture_files.push_back("data/house5.png");
  texture_files.push_back("data/WaterTank-edit.png");
  texture_files.push_back("data/Generator.png");
  texture_files.push_back("data/EngineHouse.png");
  texture_files.push_back("data/SatR.png");
  texture_files.push_back("data/cellphone.png");
  texture_files.push_back("data/Pipe.png");
  texture_files.push_back("data/PipeBend.png");
  texture_files.push_back("data/PipeT.png");
  texture_files.push_back("data/Pipe4ways.png");
  texture_files.push_back("data/Cliff0.png");
  texture_files.push_back("data/CliffBottomCorner0.png");
  texture_files.push_back(0);
  texture_files.push_back(0);
  texture_files.push_back("data/comm_s_1.png");
  texture_files.push_back("data/comm_s_2.png");
  texture_files.push_back("data/comm_s_3.png");
  texture_files.push_back("data/comm_m_1.png");
  texture_files.push_back("data/comm_m_2.png");
  texture_files.push_back("data/comm_m_3.png");
  texture_files.push_back("data/comm_m_4.png");
  texture_files.push_back("data/comm_l_1.png");
  texture_files.push_back("data/comm_l_2.png");
  texture_files.push_back("data/comm_l_3.png");
  texture_files.push_back("data/comm_l_4.png");
  texture_files.push_back(0);
  texture_files.push_back("data/Oak.png");
  texture_files.push_back("data/SchemeDown.png");
  texture_files.push_back("data/SchemeUp.png");

  for ( int i = 0; i < mesh_files.size(); ++i )
  {
    Mesh* mesh = new Mesh();

    mesh->load_obj(mesh_files[i]);
    mesh->upload_obj();

    if (texture_files[i])
    {
      mesh->texture = new Texture();
      mesh->texture->loadPng(texture_files[i]);
    }

    m_meshes.push_back(mesh);
  }

  {
    Mesh* mesh = new Mesh();
    mesh->load_obj("data/ring.obj");
    mesh->upload_obj();
    m_meshes.push_back(mesh);
  }


  //
  // Pallettes
  //
  static Pallette roadPallette;
  roadPallette.colours.push_back(glm::vec3(0.2f));
  roadPallette.colours.push_back(glm::vec3(0.5f));
  m_meshes[3]->pallette = &roadPallette;
  m_meshes[4]->pallette = &roadPallette;
  m_meshes[5]->pallette = &roadPallette;
  m_meshes[6]->pallette = &roadPallette;

  static Pallette zoneResPallette;
  zoneResPallette.colours.push_back(glm::vec3(0.2f, 1.f, 0.2f));
  m_meshes[7]->pallette = &zoneResPallette;

  static Pallette zoneComPallette;
  zoneComPallette.colours.push_back(glm::vec3(0.2f, 0.2f, 1.f));
  m_meshes[8]->pallette = &zoneComPallette;

  static Pallette redbrickPallette;
  redbrickPallette.colours.push_back(glm::vec3(140.f/255.f, 94.f/255.f, 76.f/255.f));
  redbrickPallette.colours.push_back(glm::vec3(216.f/255.f, 209.f/255.f, 157.f/255.f));
  redbrickPallette.colours.push_back(glm::vec3(124.f/255.f, 75.f/255.f, 61.f/255.f));
  redbrickPallette.colours.push_back(glm::vec3(216.f/255.f, 209.f/255.f, 157.f/255.f));
  m_meshes[13]->pallette = &redbrickPallette;
  m_meshes[14]->pallette = &redbrickPallette;
  m_meshes[15]->pallette = &redbrickPallette;
  m_meshes[16]->pallette = &redbrickPallette;
  m_meshes[17]->pallette = &redbrickPallette;

  static Pallette redbrickPalletteC;
  redbrickPalletteC.colours.push_back(glm::vec3(187.f/255.f, 126.f/255.f, 102.f/255.f));
  redbrickPalletteC.colours.push_back(glm::vec3(216.f/255.f, 209.f/255.f, 157.f/255.f));
  redbrickPalletteC.colours.push_back(glm::vec3(140.f/255.f, 94.f/255.f, 76.f/255.f));
  m_meshes[22]->pallette = &redbrickPalletteC;
  m_meshes[23]->pallette = &redbrickPalletteC;

  static Pallette redbrickPalletteA;
  redbrickPalletteA.colours.push_back(glm::vec3(216.f/255.f, 209.f/255.f, 157.f/255.f));
  static Pallette redbrickPalletteB;
  redbrickPalletteB.colours.push_back(glm::vec3(140.f/255.f, 94.f/255.f, 76.f/255.f));
  //redbrickPalletteB.colours.push_back(glm::vec3(124.f/255.f, 75.f/255.f, 61.f/255.f));

  m_meshes[24]->pallette = &redbrickPalletteB;
  m_meshes[25]->pallette = &redbrickPalletteA;

  static Pallette housePallette;
  housePallette.colours.push_back(glm::vec3(255.f/255.f, 109.f/255.f, 157.f/255.f)); // Roof
  housePallette.colours.push_back(glm::vec3(116.f/255.f, 157.f/255.f, 157.f/255.f)); // Trim
  housePallette.colours.push_back(glm::vec3(255.f/255.f, 255.f/255.f, 255.f/255.f)); // Wall
  housePallette.colours.push_back(glm::vec3(24.f/255.f, 75.f/255.f, 251.f/255.f)); // Window / Extra

  static Pallette housePalletteL;
  housePalletteL.colours.push_back(glm::vec3(226.f/255.f, 231.f/255.f, 221.f/255.f)); // Roof / Surface
  housePalletteL.colours.push_back(glm::vec3(116.f/255.f, 157.f/255.f, 157.f/255.f)); // Trim
  housePalletteL.colours.push_back(glm::vec3(255.f/255.f, 255.f/255.f, 255.f/255.f)); // Wall
  housePalletteL.colours.push_back(glm::vec3(24.f/255.f, 75.f/255.f, 251.f/255.f)); // Window / Extra

  static Pallette housePalletteMax;
  housePalletteMax.colours.push_back(glm::vec3(176.f/255.f, 112.f/255.f, 105.f/255.f)); // Red Brick
  housePalletteMax.colours.push_back(glm::vec3(161.f/255.f, 77.f/255.f, 58.f/255.f)); // Red Brick
  //housePalletteMax.colours.push_back(glm::vec3(106.f/255.f, 46.f/255.f, 42.f/255.f)); // Red Brick
  //housePalletteMax.colours.push_back(glm::vec3(140.f/255.f, 55.f/255.f, 62.f/255.f)); // Red Brick
  housePalletteMax.colours.push_back(glm::vec3(196.f/255.f, 138.f/255.f, 105.f/255.f)); // Sandstone
  housePalletteMax.colours.push_back(glm::vec3(211.f/255.f, 131.f/255.f, 119.f/255.f)); // Terra
  housePalletteMax.colours.push_back(glm::vec3(239.f/255.f, 239.f/255.f, 232.f/255.f)); // White
  housePalletteMax.colours.push_back(glm::vec3(224.f/255.f, 213.f/255.f, 198.f/255.f)); // White
  housePalletteMax.colours.push_back(glm::vec3(239.f/255.f, 235.f/255.f, 231.f/255.f)); // White
  housePalletteMax.colours.push_back(glm::vec3(219.f/255.f, 213.f/255.f, 209.f/255.f)); // White
  //housePalletteMax.colours.push_back(glm::vec3(196.f/255.f, 112.f/255.f, 39.f/255.f)); // Orange
  housePalletteMax.colours.push_back(glm::vec3(200.f/255.f, 193.f/255.f, 171.f/255.f)); // Stone
  housePalletteMax.colours.push_back(glm::vec3(149.f/255.f, 156.f/255.f, 153.f/255.f)); // Stone
  housePalletteMax.colours.push_back(glm::vec3(131.f/255.f, 197.f/255.f, 201.f/255.f)); // Stone

  housePalletteMax.colours.push_back(glm::vec3(24.f/255.f, 75.f/255.f, 251.f/255.f)); // Window / Extra

  housePalletteMax.colours.push_back(glm::vec3(116.f/255.f, 157.f/255.f, 157.f/255.f)); // Trim
  housePalletteMax.colours.push_back(glm::vec3(160.f/255.f, 160.f/255.f, 160.f/255.f)); // Trim
  housePalletteMax.colours.push_back(glm::vec3(100.f/255.f, 100.f/255.f, 100.f/255.f)); // Trim

  //housePalletteMax.colours.push_back(glm::vec3(159.f/255.f, 217.f/255.f, 215.f/255.f)); // Tint
  housePalletteMax.colours.push_back(glm::vec3(140.f/255.f, 55.f/255.f, 62.f/255.f)); // Red Brick
  housePalletteMax.colours.push_back(glm::vec3(197.f/255.f, 204.f/255.f, 192.f/255.f)); // Tint
  housePalletteMax.colours.push_back(0.7f*glm::vec3(219.f/255.f, 210.f/255.f, 219.f/255.f)); // Tint
  housePalletteMax.colours.push_back(0.7f*glm::vec3(203.f/255.f, 206.f/255.f, 190.f/255.f)); // Tint
  housePalletteMax.colours.push_back(0.7f*glm::vec3(219.f/255.f, 203.f/255.f, 189.f/255.f)); // Tint

  // extra (20)
  housePalletteMax.colours.push_back(glm::vec3(197.f/255.f, 204.f/255.f, 192.f/255.f)); // zotzenhein roof
  housePalletteMax.colours.push_back(glm::vec3(237.f/255.f, 179.f/255.f, 158.f/255.f)); // zotzenhein roof
  housePalletteMax.colours.push_back(glm::vec3(119.f/255.f, 126.f/255.f, 133.f/255.f)); // bognor roof
  housePalletteMax.colours.push_back(glm::vec3(135.f/255.f, 21.f/255.f, 31.f/255.f)); // quebec roof
  housePalletteMax.colours.push_back(glm::vec3(142.f/255.f, 153.f/255.f, 153.f/255.f)); // quebec roof
  housePalletteMax.colours.push_back(glm::vec3(251.f/255.f, 252.f/255.f, 252.f/255.f)); // zotzenhein wall
  housePalletteMax.colours.push_back(glm::vec3(254.f/255.f, 254.f/255.f, 254.f/255.f)); // zotzenhein wall
  housePalletteMax.colours.push_back(glm::vec3(194.f/255.f, 175.f/255.f, 145.f/255.f)); // bognor wall
  housePalletteMax.colours.push_back(glm::vec3(153.f/255.f, 138.f/255.f, 107.f/255.f)); // quebec wall
  housePalletteMax.colours.push_back(glm::vec3(171.f/255.f, 165.f/255.f, 155.f/255.f)); // quebec wall

  m_meshes[2]->pallette = &housePalletteMax;
  m_meshes[9]->pallette = &housePalletteMax;
  m_meshes[10]->pallette = &housePalletteMax;
  m_meshes[11]->pallette = &housePalletteMax;
  m_meshes[12]->pallette = &housePalletteMax;

  static Pallette commSmallPallette;
  commSmallPallette.colours.push_back(glm::vec3(24.f/255.f, 75.f/255.f, 251.f/255.f)); // Window / Extra
  commSmallPallette.colours.push_back(glm::vec3(116.f/255.f, 209.f/255.f, 157.f/255.f)); // green
  commSmallPallette.colours.push_back(glm::vec3(216.f/255.f, 109.f/255.f, 157.f/255.f));
  commSmallPallette.colours.push_back(glm::vec3(140.f/255.f, 140.f/255.f, 140.f/255.f)); // wall

  static Pallette commSmallPalletteB;
  commSmallPalletteB.colours.push_back(glm::vec3(116.f/255.f, 209.f/255.f, 157.f/255.f)); // green
  commSmallPalletteB.colours.push_back(glm::vec3(140.f/255.f, 140.f/255.f, 140.f/255.f)); // wall
  commSmallPalletteB.colours.push_back(glm::vec3(24.f/255.f, 75.f/255.f, 251.f/255.f)); // Window
  commSmallPalletteB.colours.push_back(glm::vec3(216.f/255.f, 109.f/255.f, 157.f/255.f));

  static Pallette commSmallPalletteC;
  commSmallPalletteC.colours.push_back(glm::vec3(216.f/255.f, 109.f/255.f, 157.f/255.f));
  commSmallPalletteC.colours.push_back(glm::vec3(140.f/255.f, 140.f/255.f, 140.f/255.f)); // wall
  commSmallPalletteC.colours.push_back(glm::vec3(24.f/255.f, 75.f/255.f, 251.f/255.f)); // Window
  commSmallPalletteC.colours.push_back(glm::vec3(116.f/255.f, 209.f/255.f, 157.f/255.f)); // green

  static Pallette commMediumPallette;
  commMediumPallette.colours.push_back(glm::vec3(216.f/255.f, 109.f/255.f, 157.f/255.f));
  commMediumPallette.colours.push_back(glm::vec3(116.f/255.f, 209.f/255.f, 157.f/255.f)); // green
  commMediumPallette.colours.push_back(glm::vec3(140.f/255.f, 140.f/255.f, 140.f/255.f)); // wall
  commMediumPallette.colours.push_back(glm::vec3(24.f/255.f, 75.f/255.f, 251.f/255.f)); // Window / Extra

  static Pallette commMediumPalletteB;
  commMediumPalletteB.colours.push_back(glm::vec3(216.f/255.f, 109.f/255.f, 157.f/255.f));
  commMediumPalletteB.colours.push_back(glm::vec3(140.f/255.f, 140.f/255.f, 140.f/255.f)); // wall
  commMediumPalletteB.colours.push_back(glm::vec3(24.f/255.f, 75.f/255.f, 251.f/255.f)); // Window / Extra
  commMediumPalletteB.colours.push_back(glm::vec3(116.f/255.f, 209.f/255.f, 157.f/255.f)); // green

  static Pallette commMediumPalletteMax;
  commMediumPalletteMax.colours.push_back(glm::vec3(216.f/255.f, 109.f/255.f, 157.f/255.f));
  commMediumPalletteMax.colours.push_back(glm::vec3(140.f/255.f, 140.f/255.f, 140.f/255.f)); // wall
  commMediumPalletteMax.colours.push_back(glm::vec3(150.f/255.f, 150.f/255.f, 155.f/255.f)); // wall
  commMediumPalletteMax.colours.push_back(glm::vec3(161.f/255.f, 161.f/255.f, 170.f/255.f)); // wall
  commMediumPalletteMax.colours.push_back(glm::vec3(24.f/255.f, 75.f/255.f, 251.f/255.f)); // Window / Extra
  commMediumPalletteMax.colours.push_back(glm::vec3(116.f/255.f, 209.f/255.f, 157.f/255.f)); // green
  commMediumPalletteMax.colours.push_back(glm::vec3(157.f/255.f, 116.f/255.f, 209.f/255.f)); // blue
  commMediumPalletteMax.colours.push_back(glm::vec3(209.f/255.f, 157.f/255.f, 116.f/255.f)); // red

  // m_meshes[ 2]->pallette = &housePallette;
  // m_meshes[ 9]->pallette = &housePallette;
  // m_meshes[10]->pallette = &housePallette;
  // m_meshes[11]->pallette = &housePallette;
  // m_meshes[12]->pallette = &housePallette;
  m_meshes[26]->pallette = &commSmallPalletteC;
  m_meshes[27]->pallette = &commSmallPalletteB;
  m_meshes[28]->pallette = &commSmallPallette;
  m_meshes[29]->pallette = &commMediumPalletteMax;
  m_meshes[30]->pallette = &commMediumPalletteMax;
  m_meshes[31]->pallette = &commMediumPalletteMax;
  m_meshes[32]->pallette = &commMediumPalletteMax;
  m_meshes[33]->pallette = &commMediumPalletteMax; // L
  m_meshes[34]->pallette = &commMediumPalletteMax;
  m_meshes[35]->pallette = &commMediumPalletteMax;
  m_meshes[36]->pallette = &commMediumPalletteMax;

  static Pallette oakPallette;
//  oakPallette.colours.push_back(glm::vec3(159.f / 255.f, 175.f / 255.f,  108.f / 255.f));// Leaf Green
  oakPallette.colours.push_back(glm::vec3(156.f / 255.f, 181.f / 255.f,  75.f / 255.f));
  oakPallette.colours.push_back(glm::vec3(129.f/255.f, 108.f/255.f, 108.f/255.f));

  m_meshes[38]->pallette = &oakPallette;
}


void CityMapRenderer::generateQueue(int groundState)
{
  if (groundState == 0)
  {
    generateQueue0();
  }
  else if (groundState == 1)
  {
    generateQueue1();
  }
  else if (groundState == 2)
  {
    generateQueue_overlay1();
  }
  else if (groundState == 3)
  {
    generateQueue_overlay2();
  }
  else if (groundState == 4)
  {
    generateQueue_overlay3();
  }
  else if (groundState == 5)
  {
    generateQueue_overlay4();
  }
  else if (groundState == 6)
  {
    generateQueue_overlay5();
  }
}


void CityMapRenderer::generateQueue0()
{
  //
  // Update the main queue
  //
  m_renderQueue.clear();

  for (int i = 0; i < m_cityMap->m_width; ++i)
  {
    for (int j = 0; j < m_cityMap->m_height; ++j)
    {
      // if (onscreen)
      float m_zoom = m_rs->m_zoom;
      float dx = i - m_rs->m_cameraX;
      float dy = j - m_rs->m_cameraY;

      //float fx = 1.f / 145.f * 1920.f / m_rs->m_window.width; /* 13.2 */
      //float fy = 1.f / 115.f * 1080.f / m_rs->m_window.height; /* 9.4 */
      float fx = 13.2f / m_rs->m_window.width;
      float fy = 9.4f / m_rs->m_window.height;

      if ((dx + dy + 1)*m_zoom*fy > -1.f && (dx + dy - 1)*m_zoom*fy < 1.f &&
          (dx - dy + 1)*m_zoom*fx > -1.f && (dx - dy - 1)*m_zoom*fx < 1.f)
      {
        glm::mat4 local = glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f));

        // Tile
        if (m_cityMap->getTile(i, j).zone != 0 && m_cityMap->getTile(i, j).object_type == 0)
        {
          // Zone tile
          if (m_cityMap->getTile(i, j).zone == 1)
          {
            m_renderQueue.setProgram(m_rs->programObject2);
            m_renderQueue.setMesh(m_meshes[7]);
            m_renderQueue.setMVP(local);
          }
          else
          {
            m_renderQueue.setProgram(m_rs->programObject2);
            m_renderQueue.setMesh(m_meshes[8]);
            m_renderQueue.setMVP(local);
          }
        }
        else
        {
          // Grass tile
          m_renderQueue.setProgram(m_rs->programObject);
          m_renderQueue.setMesh(m_meshes[0]);
          m_renderQueue.setMVP(local);
        }

        // Draw Tile
        drawTile(
          m_cityMap->getTile(i, j),
          local
        );

      }
    }
  }
}


void CityMapRenderer::generateQueue1()
{
  //
  // Update the underground queue
  //
  m_renderQueue.clear();

  for (int i = 0; i < m_cityMap->m_width; ++i)
  {
    for (int j = 0; j < m_cityMap->m_height; ++j)
    {
      // if (onscreen)
      float m_zoom = m_rs->m_zoom;
      float dx = i - m_rs->m_cameraX;
      float dy = j - m_rs->m_cameraY;

      //float fx = 1.f / 145.f * 1920.f / m_rs->m_window.width; /* 13.2 */
      //float fy = 1.f / 115.f * 1080.f / m_rs->m_window.height; /* 9.4 */
      float fx = 13.2f / m_rs->m_window.width;
      float fy = 9.4f / m_rs->m_window.height;

      if ((dx + dy + 1)*m_zoom*fy > -1.f && (dx + dy - 1)*m_zoom*fy < 1.f &&
          (dx - dy + 1)*m_zoom*fx > -1.f && (dx - dy - 1)*m_zoom*fx < 1.f)
      {
        glm::mat4 local = glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f));
        CityTile& tile = m_cityMap->getTile(i, j);

        // TODO - show Utility radius

        // ....

        // Resource Box [Foundational]
        if (tile.object_type == 2) // Water
        {
          m_renderQueue.setProgram(m_rs->programObject);
          m_renderQueue.setMesh(m_meshes[39]);
          m_renderQueue.setDiffuse(glm::vec3(0.5f, 0.5f, 1.0f));
          m_renderQueue.setMVP(glm::scale(local, glm::vec3(0.47)));
        }
        if (tile.object_type == 3) // Power
        {
          m_renderQueue.setProgram(m_rs->programObject);
          m_renderQueue.setMesh(m_meshes[39]);
          m_renderQueue.setDiffuse(glm::vec3(1.f, 0.5f, 0.5f));
          m_renderQueue.setMVP(glm::scale(local, glm::vec3(0.47)));
        }
        if (tile.object_type == 4) //
        {
          m_renderQueue.setProgram(m_rs->programObject);
          m_renderQueue.setMesh(m_meshes[40]);
          if (tile.utilityFlag & (1<<2))
            m_renderQueue.setDiffuse(glm::vec3(1.f, 0.5f, 0.5f));
          else
            m_renderQueue.setDiffuse(glm::vec3(0.5f, 0.25f, 0.25f));
          m_renderQueue.setMVP(glm::scale(local, glm::vec3(0.27)));
        }
        if (tile.object_type == 5) // Data
        {
          m_renderQueue.setProgram(m_rs->programObject);
          m_renderQueue.setMesh(m_meshes[39]);
          m_renderQueue.setDiffuse(glm::vec3(0.5f, 1.f, 0.5f));
          m_renderQueue.setMVP(glm::scale(local, glm::vec3(0.47)));
        }
        if (tile.object_type == 6) //
        {
          m_renderQueue.setProgram(m_rs->programObject);
          m_renderQueue.setMesh(m_meshes[40]);
          if (tile.utilityFlag & (1<<3))
            m_renderQueue.setDiffuse(glm::vec3(0.5f, 1.f, 0.5f));
          else
            m_renderQueue.setDiffuse(glm::vec3(0.25f, 0.5f, 0.25f));
          m_renderQueue.setMVP(glm::scale(local, glm::vec3(0.27)));
        }

        if (tile.pipeType)
        {
          glm::mat4 sublocal = glm::translate(local, glm::vec3(0.f, 0.f, -0.175f));
          //sublocal = glm::scale(sublocal, glm::vec3(1.5f, 1.5f, 1.5f));
          sublocal = glm::scale(sublocal, glm::vec3(0.5f, 0.5f, 0.5f));
          sublocal = glm::rotate(sublocal,
            (float)tile.pipeAng * 1.57079632679f,
            glm::vec3(0.f, 0.f, 1.f));

          glm::vec3 bright;
          if (tile.pipeType == 1) bright = glm::vec3(96.0/255.0, 146.0/255.0, 184.0/255.0);
          if (tile.pipeType == 2) bright = glm::vec3(184.0/255.0, 96.0/255.0, 96.0/255.000);
          if (tile.pipeType == 3) bright = glm::vec3(96.0/255.0, 184.0/255.0, 118.0/255.00);
          glm::vec3 dull;
          if (tile.pipeType == 1) dull = glm::vec3(64.0/255.0, 98.0/255.0, 123.0/255.0);
          if (tile.pipeType == 2) dull = glm::vec3(123.0/255.0, 64.0/255.0, 64.0/255.000);
          if (tile.pipeType == 3) dull = glm::vec3(64.0/255.0, 123.0/255.0, 79.0/255.00);

          m_renderQueue.setProgram(m_rs->programObject);
          m_renderQueue.setMesh(m_meshes[18 + tile.pipeNum]);
          if (tile.utilityFlag & (1<<tile.pipeType))
            m_renderQueue.setDiffuse(bright);
          else
            m_renderQueue.setDiffuse(dull);
          m_renderQueue.setMVP(sublocal);

          // pipe_meshes[tile.pipeRend]->draw(uniformDiffuse);
          // if (tile.utilityFlag & (1<<tile.pipeType))
          //   pipe_meshes[tile.pipeNum]->drawWireframe(uniformDiffuse, tile.pipeType);
          // else
          //   pipe_meshes[tile.pipeNum]->drawWireframe(uniformDiffuse, 10 + tile.pipeType);

          // ground under pipe
          m_renderQueue.setProgram(m_rs->programObject2);
          m_renderQueue.setMesh(m_meshes[25]);
          m_renderQueue.setMVP(glm::translate(local, glm::vec3(0.f, 0.f, -0.330f)));
        }
        else // Draw cliffs
        {
          //local = glm::scale(local, glm::vec3(0.25f, 0.25f, 0.25f));
          local = glm::scale(local, glm::vec3(0.5f, 0.5f, 0.5f));
          // local = glm::scale(local, glm::vec3(0.5f, 0.5f, 0.5f));
          // local = glm::translate(local, glm::vec3(0.f, 0.f, -1.0f));

          float h = 0.64f;
          float h2 = -0.65f;

          glm::mat4 sublocal = glm::translate(local, glm::vec3(0.5f, -0.5f, h2));
          if (tile.un1)
          {
            drawCliffSide(sublocal, tile.un1, false);
          }
          else
          {
            // Top ground (no cliffs)
            sublocal = glm::translate(sublocal, glm::vec3(0.0f, 0.0f, h));
            m_renderQueue.setProgram(m_rs->programObject2);
            m_renderQueue.setMesh(m_meshes[24]);
            m_renderQueue.setMVP(sublocal);
          }

          sublocal = glm::translate(local, glm::vec3(0.5f, 0.5f, h2));
          if (tile.un2)
          {
            sublocal = glm::rotate(sublocal, (float)1.57079632679f, glm::vec3(0.f, 0.f, 1.0f));
            drawCliffSide(sublocal, tile.un2, false);
          }
          else
          {
            // Top ground (no cliffs)
            sublocal = glm::translate(sublocal, glm::vec3(0.0f, 0.0f, h));
            m_renderQueue.setProgram(m_rs->programObject2);
            m_renderQueue.setMesh(m_meshes[24]);
            m_renderQueue.setMVP(sublocal);
          }

          sublocal = glm::translate(local, glm::vec3(-0.5f, 0.5f, h2));
          if (tile.un4)
          {
            sublocal = glm::rotate(sublocal, (float)2.f * 1.57079632679f, glm::vec3(0.f, 0.f, 1.0f));

            drawCliffSide(sublocal, tile.un4, false);
          }
          else
          {
            // Top ground (no cliffs)
            sublocal = glm::translate(sublocal, glm::vec3(0.0f, 0.0f, h));
            m_renderQueue.setProgram(m_rs->programObject2);
            m_renderQueue.setMesh(m_meshes[24]);
            m_renderQueue.setMVP(sublocal);
          }

          sublocal = glm::translate(local, glm::vec3(-0.5f, -0.5f, h2));
          if (tile.un3)
          {
            sublocal = glm::rotate(sublocal, (float)3.f * 1.57079632679f, glm::vec3(0.f, 0.f, 1.0f));

            drawCliffSide(sublocal, tile.un3, false);
          }
          else
          {
            // Top ground (no cliffs)
            sublocal = glm::translate(sublocal, glm::vec3(0.0f, 0.0f, h));
            m_renderQueue.setProgram(m_rs->programObject2);
            m_renderQueue.setMesh(m_meshes[24]);
            m_renderQueue.setMVP(sublocal);
          }
        }
      }
    }
  }
}


void CityMapRenderer::drawCliffSide(glm::mat4 MVP, char i, bool hideOutDebug)
{
  glm::mat4 local;
  //MVP = glm::scale(MVP, glm::vec3(0.666f, 0.666f, 0.666f));

  // float colour0[] = { 97.f / 255.f, 94.f / 255.f,  95.f / 255.f }; // Granite
  // glUniform3fv(uniformDiffuse, 1, colour0);
  // drawObjModel_square();

  int cliff0 = 22;
  int cliff1 = 23;

  if (hideOutDebug) return;

  // Base
  m_renderQueue.setProgram(m_rs->programObject2);
  m_renderQueue.setMesh(m_meshes[25]);
  m_renderQueue.setMVP(glm::translate(MVP, glm::vec3(0.f, 0.f, -0.01f)));

  if (i == 1) // corner
  {
    local = glm::rotate(MVP, (float)1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    local = glm::translate(local, glm::vec3(0.5f, 0.5f, 0.0f));
    local = glm::scale(local, glm::vec3(0.333, 0.333, 0.333));

    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[cliff1]);
    m_renderQueue.setMVP(local);
  }
  if (i == 2)
  {
    local = glm::rotate(MVP, (float)1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    local = glm::translate(local, glm::vec3(0.5f, 0.0f, 0.0f));
    local = glm::scale(local, glm::vec3(0.333, 0.333, 0.333));

    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[cliff0]);
    m_renderQueue.setMVP(local);
  }
  if (i == 3)
  {
    local = glm::rotate(MVP, (float)2.f * 1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    local = glm::translate(local, glm::vec3(0.5f, 0.0f, 0.0f));
    local = glm::scale(local, glm::vec3(0.333, 0.333, 0.333));

    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[cliff0]);
    m_renderQueue.setMVP(local);
  }
  if (i == 4) // inner corner
  {
    local = glm::rotate(MVP, (float)2.f * 1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    local = glm::translate(local, glm::vec3(0.5f, 0.0f, 0.0f));
    local = glm::scale(local, glm::vec3(0.333, 0.333, 0.333));

    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[cliff0]);
    m_renderQueue.setMVP(local);

    local = glm::rotate(MVP, (float)1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    local = glm::translate(local, glm::vec3(0.5f, 0.0f, 0.0f));
    local = glm::scale(local, glm::vec3(0.333, 0.333, 0.333));

    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[cliff0]);
    m_renderQueue.setMVP(local);
  }
}


void CityMapRenderer::tick()
{
  //
  // Update the Cursor object
  //
  m_renderQueueAdd.clear();

  m_renderQueueAdd.setProgram(m_rs->programObject2);
  m_renderQueueAdd.setMesh(m_meshes[37]); // Ring
  float x = (int)(m_rs->m_cursorX + 0.5f);
  float y = (int)(m_rs->m_cursorY + 0.5f);
  //m_renderQueueAdd.setMVP(glm::translate(glm::mat4(1.0f), glm::vec3(1.f * x, 1.f * y, 0.f)));
  m_renderQueueAdd.setDiffuse(glm::vec3(0.79, 0.79, 0.2));
  m_renderQueueAdd.setMVP(
    //glm::scale(
      glm::translate(
        glm::mat4(1.0f),
        glm::vec3(1.f * x, 1.f * y, 0.1f))
      //glm::vec3(0.2f, 0.2f, 0.2f))
  );

}


void CityMapRenderer::drawTile(CityTile& tile, glm::mat4& local)
{

  // Tree
  if (tile.object_type == 1)
  {
    m_renderQueue.setProgram(m_rs->programObject);

    if (tile.treeAng1 == 1 && tile.treeAng2 == 1 && tile.treeAng3 == 1 && tile.treeAng4 == 1)
      m_renderQueue.setMesh(m_meshes[38]); // Oak
    else
      m_renderQueue.setMesh(m_meshes[1]); // Pine

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

  // Road
  if (tile.road)
  {
    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.33f));
    sublocal = glm::rotate(sublocal,
      (float)tile.roadAng * 1.57079632679f,
      glm::vec3(0.f, 0.f, 1.f));

    m_renderQueue.setProgram(m_rs->programObject2);
    m_renderQueue.setMesh(m_meshes[3 + tile.roadNum]);

    m_renderQueue.setMVP(sublocal);
  }

  // Water
  if (tile.object_type == 2)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[13]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.75f));

    m_renderQueue.setMVP(sublocal);
  }

  // Pwr
  if (tile.object_type == 3)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[14]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.64f));

    m_renderQueue.setMVP(sublocal);
  }

  // Pwr
  if (tile.object_type == 4)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[15]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.16f));

    m_renderQueue.setMVP(sublocal);
  }

  // Data
  if (tile.object_type == 5)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[16]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.75f));

    m_renderQueue.setMVP(sublocal);
  }

  // Data
  if (tile.object_type == 6)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[17]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.50f));

    m_renderQueue.setMVP(sublocal);
  }

  // House setting
  local = glm::rotate(local,
    (float)tile.treeAng3 * 1.57079632679f,
    glm::vec3(0.f, 0.f, 1.f));

  // Houses
  if (tile.object_type == 101)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[2]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 5;
    int r2 = (j + i*i + j*j) % 5;
    int r3 = (i*j + i) % 2;

    std::vector<int> p_id;
    // p_id.push_back(15 + r2); // Roof
    // p_id.push_back(12 + r3); // Trim
    // p_id.push_back(0 + r1); // Wall
    // p_id.push_back(11); // Window
    p_id.push_back(20 + r1); // Roof
    p_id.push_back(12 + r3); // Trim
    p_id.push_back(25 + r1); // Wall
    p_id.push_back(11); // Window
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 102)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[9]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 5;
    int r2 = (j + i*i + j*j) % 5;
    int r3 = (i*j + i) % 2;

    std::vector<int> p_id;
    // p_id.push_back(15 + r2); // Roof
    // p_id.push_back(12 + r3); // Trim
    // p_id.push_back(0 + r1); // Wall
    // p_id.push_back(11); // Window
    p_id.push_back(20 + r1); // Roof
    p_id.push_back(12 + r3); // Trim
    p_id.push_back(25 + r1); // Wall
    p_id.push_back(11); // Window
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 103)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[10]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 5;
    int r2 = (j + i*i + j*j) % 5;
    int r3 = (i*j + i) % 2;

    std::vector<int> p_id;
    // p_id.push_back(15 + r2); // Roof
    // p_id.push_back(12 + r3); // Trim
    // p_id.push_back(0 + r1); // Wall
    // p_id.push_back(11); // Window
    p_id.push_back(20 + r1); // Roof
    p_id.push_back(12 + r3); // Trim
    p_id.push_back(25 + r1); // Wall
    p_id.push_back(11); // Window
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 104)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[11]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 11;
    int r2 = (j + i*i + j*j) % 4;
    int r3 = (i*j + i) % 3;

    std::vector<int> p_id;
    p_id.push_back(6 + r2); // Roof
    p_id.push_back(12 + r3); // Trim
    p_id.push_back(0 + r1); // Wall
    p_id.push_back(11); // Window
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 105)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[12]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 11;
    int r2 = (j + i*i + j*j) % 4;
    int r3 = (i*j + i) % 3;

    std::vector<int> p_id;
    p_id.push_back(6 + r2); // Roof
    p_id.push_back(12 + r3); // Trim
    p_id.push_back(0 + r1); // Wall
    p_id.push_back(11); // Window
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 200)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[26]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1f));

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 201)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[27]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1f));

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 202)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[28]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1f));

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 210)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[29]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1f));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 3;
    int r2 = (j + i*i + j*j) % 3;

    std::vector<int> p_id;
    p_id.push_back(0);
    p_id.push_back(1 + r1); // wall
    p_id.push_back(4); // Window
    p_id.push_back(5 + r2); // trim
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 211)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[30]); // different

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1f));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 3;
    int r2 = (j + i*i + j*j) % 3;

    std::vector<int> p_id;
    p_id.push_back(0);
    p_id.push_back(5 + r2); // trim
    p_id.push_back(1 + r1); // wall
    p_id.push_back(4); // Window
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 212)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[31]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1f));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 3;
    int r2 = (j + i*i + j*j) % 3;

    std::vector<int> p_id;
    p_id.push_back(0);
    p_id.push_back(1 + r1); // wall
    p_id.push_back(4); // Window
    p_id.push_back(5 + r2); // trim
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 213)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[32]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1f));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 3;
    int r2 = (j + i*i + j*j) % 3;

    std::vector<int> p_id;
    p_id.push_back(0);
    p_id.push_back(1 + r1); // wall
    p_id.push_back(4); // Window
    p_id.push_back(5 + r2); // trim
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 220)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[33]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1f));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 3;
    int r2 = (j + i*i + j*j) % 3;

    std::vector<int> p_id;
    p_id.push_back(0);
    p_id.push_back(1 + r1); // wall
    p_id.push_back(4); // Window
    p_id.push_back(5 + r2); // trim
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 221)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[34]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1f));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 3;
    int r2 = (j + i*i + j*j) % 3;

    std::vector<int> p_id;
    p_id.push_back(0);
    p_id.push_back(1 + r1); // wall
    p_id.push_back(4); // Window
    p_id.push_back(5 + r2); // trim
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 222)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[35]); // different

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1f));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 3;
    int r2 = (j + i*i + j*j) % 3;

    std::vector<int> p_id;
    p_id.push_back(0);
    p_id.push_back(5 + r2); // trim
    p_id.push_back(1 + r1); // wall
    p_id.push_back(4); // Window
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }

  if (tile.object_type == 223)
  {
    m_renderQueue.setProgram(m_rs->programObject);
    m_renderQueue.setMesh(m_meshes[36]);

    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.1f));

    int i = local[3][0];
    int j = local[3][1];
    int r1 = (i + i*i + j*j) % 3;
    int r2 = (j + i*i + j*j) % 3;

    std::vector<int> p_id;
    p_id.push_back(0);
    p_id.push_back(1 + r1); // wall
    p_id.push_back(4); // Window
    p_id.push_back(5 + r2); // trim
    m_renderQueue.setPallette(p_id);

    m_renderQueue.setMVP(sublocal);
  }
}


void CityMapRenderer::generateQueue_overlay1()
{
  // Land Value
  m_renderQueue.clear();

  for (int i = 0; i < m_cityMap->m_width; ++i)
  {
    for (int j = 0; j < m_cityMap->m_height; ++j)
    {
      // if (onscreen)
      float m_zoom = m_rs->m_zoom;
      float dx = i - m_rs->m_cameraX;
      float dy = j - m_rs->m_cameraY;

      //float fx = 1.f / 145.f * 1920.f / m_rs->m_window.width; /* 13.2 */
      //float fy = 1.f / 115.f * 1080.f / m_rs->m_window.height; /* 9.4 */
      float fx = 13.2f / m_rs->m_window.width;
      float fy = 9.4f / m_rs->m_window.height;

      if ((dx + dy + 1)*m_zoom*fy > -1.f && (dx + dy - 1)*m_zoom*fy < 1.f &&
          (dx - dy + 1)*m_zoom*fx > -1.f && (dx - dy - 1)*m_zoom*fx < 1.f)
      {
        glm::mat4 local = glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f));

        CityTile& tile = m_cityMap->getTile(i, j);

        // Tile
        m_renderQueue.setProgram(m_rs->programObject2);
        m_renderQueue.setMesh(m_meshes[0]);

        // Utility value
        int x = 1;
        if (tile.surfaceUtilityFlag & 1<<1) ++x;
        if (tile.surfaceUtilityFlag & 1<<2) ++x;
        if (tile.surfaceUtilityFlag & 1<<3) ++x;

        float v = CityMap::getTileValue(tile);

        float colour0[] = { 1.f, 0.15f, 0.15f };
        float colour1[] = { 1.f, 0.15f, 0.15f };
        float colour2[] = { 0.55f, 0.75f, 0.15f };
        float colour3[] = { 0.15f, 1.f, 0.15f };
        float colour4[] = { 0.15f, 0.15f, 1.0f };
        glm::vec3 colourB;

        if (v < 0.25f)
        {
          v = (v) * 4.f;
          colourB.x = (1.f - v) * colour0[0] + v * colour1[0];
          colourB.y = (1.f - v) * colour0[1] + v * colour1[1];
          colourB.z = (1.f - v) * colour0[2] + v * colour1[2];
        }
        else if (v < 0.5f)
        {
          v = (v - 0.25f) * 4.f;
          colourB.x = (1.f - v) * colour1[0] + v * colour2[0];
          colourB.y = (1.f - v) * colour1[1] + v * colour2[1];
          colourB.z = (1.f - v) * colour1[2] + v * colour2[2];
        }
        else if (v < 0.75f)
        {
          v = (v - 0.5f) * 4.f;
          colourB.x = (1.f - v) * colour2[0] + v * colour3[0];
          colourB.y = (1.f - v) * colour2[1] + v * colour3[1];
          colourB.z = (1.f - v) * colour2[2] + v * colour3[2];
        }
        else
        {
          v = (v - 0.75f) * 4.f;
          colourB.x = (1.f - v) * colour3[0] + v * colour4[0];
          colourB.y = (1.f - v) * colour3[1] + v * colour4[1];
          colourB.z = (1.f - v) * colour3[2] + v * colour4[2];
        }

        m_renderQueue.setDiffuse(colourB);
        m_renderQueue.setMVP(local);

        // Road
        if (tile.road)
        {
          glm::mat4 sublocal = glm::scale(local, glm::vec3(0.33f));
          sublocal = glm::rotate(sublocal,
            (float)tile.roadAng * 1.57079632679f,
            glm::vec3(0.f, 0.f, 1.f));

          m_renderQueue.setProgram(m_rs->programObject2);
          m_renderQueue.setMesh(m_meshes[3 + tile.roadNum]);

          m_renderQueue.setMVP(sublocal);
        }
      }
    }
  }
}


void CityMapRenderer::generateQueue_overlay2()
{
  // Air Quality

  m_renderQueue.clear();

  for (int i = 0; i < m_cityMap->m_width; ++i)
  {
    for (int j = 0; j < m_cityMap->m_height; ++j)
    {
      // if (onscreen)
      float m_zoom = m_rs->m_zoom;
      float dx = i - m_rs->m_cameraX;
      float dy = j - m_rs->m_cameraY;

      //float fx = 1.f / 145.f * 1920.f / m_rs->m_window.width; /* 13.2 */
      //float fy = 1.f / 115.f * 1080.f / m_rs->m_window.height; /* 9.4 */
      float fx = 13.2f / m_rs->m_window.width;
      float fy = 9.4f / m_rs->m_window.height;

      if ((dx + dy + 1)*m_zoom*fy > -1.f && (dx + dy - 1)*m_zoom*fy < 1.f &&
          (dx - dy + 1)*m_zoom*fx > -1.f && (dx - dy - 1)*m_zoom*fx < 1.f)
      {
        glm::mat4 local = glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f));

        CityTile& tile = m_cityMap->getTile(i, j);

        // Tile
        m_renderQueue.setProgram(m_rs->programObject2);
        m_renderQueue.setMesh(m_meshes[0]);

        float colour0[] = { 200.f / 255.f, 224.f / 255.f,  224.f / 255.f }; // Skylight
        float colour1[] = { 205.f / 255.f, 178.f / 255.f,  165.f / 255.f }; // Dusty Rose
        glm::vec3 colourB;

        float a = (float)tile.airPollution / 255.f;
        colourB.x = (1.f - a) * colour0[0] + a * colour1[0];
        colourB.y = (1.f - a) * colour0[1] + a * colour1[1];
        colourB.z = (1.f - a) * colour0[2] + a * colour1[2];

        m_renderQueue.setDiffuse(colourB);
        m_renderQueue.setMVP(local);

        // Road
        if (tile.road)
        {
          glm::mat4 sublocal = glm::scale(local, glm::vec3(0.33f));
          sublocal = glm::rotate(sublocal,
            (float)tile.roadAng * 1.57079632679f,
            glm::vec3(0.f, 0.f, 1.f));

          m_renderQueue.setProgram(m_rs->programObject2);
          m_renderQueue.setMesh(m_meshes[3 + tile.roadNum]);

          m_renderQueue.setMVP(sublocal);
        }

        // Park / Trees
        if (tile.object_type == 1)
        {
          m_renderQueue.setProgram(m_rs->programObject);

          if (tile.treeAng1 == 1 && tile.treeAng2 == 1 && tile.treeAng3 == 1 && tile.treeAng4 == 1)
            m_renderQueue.setMesh(m_meshes[38]); // Oak
          else
            m_renderQueue.setMesh(m_meshes[1]); // Pine

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
}


void CityMapRenderer::generateQueue_overlay3()
{
  // Utility 1

  m_renderQueue.clear();

  for (int i = 0; i < m_cityMap->m_width; ++i)
  {
    for (int j = 0; j < m_cityMap->m_height; ++j)
    {
      // if (onscreen)
      float m_zoom = m_rs->m_zoom;
      float dx = i - m_rs->m_cameraX;
      float dy = j - m_rs->m_cameraY;

      //float fx = 1.f / 145.f * 1920.f / m_rs->m_window.width; /* 13.2 */
      //float fy = 1.f / 115.f * 1080.f / m_rs->m_window.height; /* 9.4 */
      float fx = 13.2f / m_rs->m_window.width;
      float fy = 9.4f / m_rs->m_window.height;

      if ((dx + dy + 1)*m_zoom*fy > -1.f && (dx + dy - 1)*m_zoom*fy < 1.f &&
          (dx - dy + 1)*m_zoom*fx > -1.f && (dx - dy - 1)*m_zoom*fx < 1.f)
      {
        glm::mat4 local = glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f));

        CityTile& tile = m_cityMap->getTile(i, j);

        // Tile
        m_renderQueue.setProgram(m_rs->programObject2);
        m_renderQueue.setMesh(m_meshes[0]);

        if (tile.surfaceUtilityFlag & 1<<1)
        {
          m_renderQueue.setDiffuse(glm::vec3(0.6, 0.969, 0.6));
        }
        else
        {
          m_renderQueue.setDiffuse(glm::vec3(0.69));
        }

        m_renderQueue.setMVP(local);

        // Road
        if (tile.road)
        {
          glm::mat4 sublocal = glm::scale(local, glm::vec3(0.33f));
          sublocal = glm::rotate(sublocal,
            (float)tile.roadAng * 1.57079632679f,
            glm::vec3(0.f, 0.f, 1.f));

          m_renderQueue.setProgram(m_rs->programObject2);
          m_renderQueue.setMesh(m_meshes[3 + tile.roadNum]);

          m_renderQueue.setMVP(sublocal);
        }

        // Water
        if (tile.object_type == 2)
        {
          m_renderQueue.setProgram(m_rs->programObject);
          m_renderQueue.setMesh(m_meshes[13]);

          glm::mat4 sublocal = glm::scale(local, glm::vec3(0.75f));

          m_renderQueue.setDiffuse(glm::vec3(1.0, 1.0, 1.0));
          m_renderQueue.setMVP(sublocal);
        }
      }
    }
  }
}


void CityMapRenderer::generateQueue_overlay4()
{
  // Utility 2

  m_renderQueue.clear();

  for (int i = 0; i < m_cityMap->m_width; ++i)
  {
    for (int j = 0; j < m_cityMap->m_height; ++j)
    {
      // if (onscreen)
      float m_zoom = m_rs->m_zoom;
      float dx = i - m_rs->m_cameraX;
      float dy = j - m_rs->m_cameraY;

      //float fx = 1.f / 145.f * 1920.f / m_rs->m_window.width; /* 13.2 */
      //float fy = 1.f / 115.f * 1080.f / m_rs->m_window.height; /* 9.4 */
      float fx = 13.2f / m_rs->m_window.width;
      float fy = 9.4f / m_rs->m_window.height;

      if ((dx + dy + 1)*m_zoom*fy > -1.f && (dx + dy - 1)*m_zoom*fy < 1.f &&
          (dx - dy + 1)*m_zoom*fx > -1.f && (dx - dy - 1)*m_zoom*fx < 1.f)
      {
        glm::mat4 local = glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f));

        CityTile& tile = m_cityMap->getTile(i, j);

        // Tile
        m_renderQueue.setProgram(m_rs->programObject2);
        m_renderQueue.setMesh(m_meshes[0]);

        if (tile.surfaceUtilityFlag & 1<<2)
        {
          m_renderQueue.setDiffuse(glm::vec3(0.6, 0.969, 0.6));
        }
        else
        {
          m_renderQueue.setDiffuse(glm::vec3(0.69));
        }

        m_renderQueue.setMVP(local);

        // Road
        if (tile.road)
        {
          glm::mat4 sublocal = glm::scale(local, glm::vec3(0.33f));
          sublocal = glm::rotate(sublocal,
            (float)tile.roadAng * 1.57079632679f,
            glm::vec3(0.f, 0.f, 1.f));

          m_renderQueue.setProgram(m_rs->programObject2);
          m_renderQueue.setMesh(m_meshes[3 + tile.roadNum]);

          m_renderQueue.setMVP(sublocal);
        }

        // Powers

          // Pwr
          if (tile.object_type == 3)
          {
            m_renderQueue.setProgram(m_rs->programObject);
            m_renderQueue.setMesh(m_meshes[14]);

            glm::mat4 sublocal = glm::scale(local, glm::vec3(0.64f));

            m_renderQueue.setDiffuse(glm::vec3(1.0, 1.0, 1.0));
            m_renderQueue.setMVP(sublocal);
          }

          // Pwr
          if (tile.object_type == 4)
          {
            m_renderQueue.setProgram(m_rs->programObject);
            m_renderQueue.setMesh(m_meshes[15]);

            glm::mat4 sublocal = glm::scale(local, glm::vec3(0.20f));

            m_renderQueue.setDiffuse(glm::vec3(1.0, 1.0, 1.0));
            m_renderQueue.setMVP(sublocal);
          }
      }
    }
  }
}


void CityMapRenderer::generateQueue_overlay5()
{
  // Utility 3

  m_renderQueue.clear();

  for (int i = 0; i < m_cityMap->m_width; ++i)
  {
    for (int j = 0; j < m_cityMap->m_height; ++j)
    {
      // if (onscreen)
      float m_zoom = m_rs->m_zoom;
      float dx = i - m_rs->m_cameraX;
      float dy = j - m_rs->m_cameraY;

      //float fx = 1.f / 145.f * 1920.f / m_rs->m_window.width; /* 13.2 */
      //float fy = 1.f / 115.f * 1080.f / m_rs->m_window.height; /* 9.4 */
      float fx = 13.2f / m_rs->m_window.width;
      float fy = 9.4f / m_rs->m_window.height;

      if ((dx + dy + 1)*m_zoom*fy > -1.f && (dx + dy - 1)*m_zoom*fy < 1.f &&
          (dx - dy + 1)*m_zoom*fx > -1.f && (dx - dy - 1)*m_zoom*fx < 1.f)
      {
        glm::mat4 local = glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.f));

        CityTile& tile = m_cityMap->getTile(i, j);

        // Tile
        m_renderQueue.setProgram(m_rs->programObject2);
        m_renderQueue.setMesh(m_meshes[0]);

        if (tile.surfaceUtilityFlag & 1<<3)
        {
          m_renderQueue.setDiffuse(glm::vec3(0.6, 0.969, 0.6));
        }
        else
        {
          m_renderQueue.setDiffuse(glm::vec3(0.69));
        }

        m_renderQueue.setMVP(local);

        // Road
        if (tile.road)
        {
          glm::mat4 sublocal = glm::scale(local, glm::vec3(0.33f));
          sublocal = glm::rotate(sublocal,
            (float)tile.roadAng * 1.57079632679f,
            glm::vec3(0.f, 0.f, 1.f));

          m_renderQueue.setProgram(m_rs->programObject2);
          m_renderQueue.setMesh(m_meshes[3 + tile.roadNum]);

          m_renderQueue.setMVP(sublocal);
        }

        // Datas

          // Data
          if (tile.object_type == 5)
          {
            m_renderQueue.setProgram(m_rs->programObject);
            m_renderQueue.setMesh(m_meshes[16]);

            glm::mat4 sublocal = glm::scale(local, glm::vec3(0.75f));

            m_renderQueue.setDiffuse(glm::vec3(1.0, 1.0, 1.0));
            m_renderQueue.setMVP(sublocal);
          }

          // Data
          if (tile.object_type == 6)
          {
            m_renderQueue.setProgram(m_rs->programObject);
            m_renderQueue.setMesh(m_meshes[17]);

            glm::mat4 sublocal = glm::scale(local, glm::vec3(0.50f));

            m_renderQueue.setDiffuse(glm::vec3(1.0, 1.0, 1.0));
            m_renderQueue.setMVP(sublocal);
          }
      }
    }
  }
}
