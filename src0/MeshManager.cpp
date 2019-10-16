#include "MeshManager.h"
#include "Texture.h"


static MeshManager* s_singleton = 0;


MeshManager* MeshManager::get()
{
  return s_singleton;
}


MeshManager::MeshManager()
{
  s_singleton = this;
}


MeshManager::~MeshManager()
{

}


void MeshManager::init()
{
  printf("Beginning MeshManager::init\n");
  //return;

  m_indexMap[0] = 0;

  mesh_files.push_back("data/square.obj");
  mesh_files.push_back("data/lightmap/tree_lightmap.obj");
  mesh_files.push_back("data/HousesPackLowPoly_house1.obj");
  mesh_files.push_back("data/3D Road Pack/Models/roadTile_277-edit2.obj");
  mesh_files.push_back("data/3D Road Pack/Models/roadTile_299-edit2.obj");
  mesh_files.push_back("data/3D Road Pack/Models/roadTile_148-edit2.obj");
  mesh_files.push_back("data/3D Road Pack/Models/roadTile_260-edit2.obj");
  mesh_files.push_back("data/square.obj");
  mesh_files.push_back("data/square.obj");
  m_indexMap[HOUSES] = mesh_files.size();
  mesh_files.push_back("data/HousesPackLowPoly_house2.obj");
  mesh_files.push_back("data/HousesPackLowPoly_house3.obj");
  mesh_files.push_back("data/HousesPackLowPoly_house4.obj");
  mesh_files.push_back("data/HousesPackLowPoly_house5.obj");
  m_indexMap[UTILITY] = mesh_files.size();
  mesh_files.push_back("data/WaterTank-edit.obj");
  mesh_files.push_back("data/Generator.obj");
  mesh_files.push_back("data/EngineHouse.obj");
  mesh_files.push_back("data/SatReceiver.obj");
  mesh_files.push_back("data/cellphone.obj");
  m_indexMap[PIPE] = mesh_files.size();
  mesh_files.push_back("data/pipe.obj");
  mesh_files.push_back("data/pipebend.obj");
  mesh_files.push_back("data/pipeT.obj");
  mesh_files.push_back("data/pipe4ways.obj");
  m_indexMap[CLIFF] = mesh_files.size();
  mesh_files.push_back("data/CliffBottom0.obj");
  mesh_files.push_back("data/CliffBottomCorner0.obj");
  mesh_files.push_back("data/square.obj");
  mesh_files.push_back("data/square.obj");
  m_indexMap[COMM] = mesh_files.size();
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
  m_indexMap[OTHER] = mesh_files.size();
  mesh_files.push_back("data/ring.obj");
  mesh_files.push_back("data/Oak.obj");
  mesh_files.push_back("data/SchemeDown.obj");
  mesh_files.push_back("data/SchemeUp.obj");

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

}


int MeshManager::getIndex(int a, int b)
{
  int x = m_indexMap[a];
  return x + b;
}


Mesh* MeshManager::getMesh(int index)
{
  return m_meshes[index];
}


bool MeshManager::loading()
{
  return m_meshes.size() < mesh_files.size();
}


void MeshManager::load(int* a, int* b)
{
  int i = m_meshes.size();
  *a = i;
  *b = mesh_files.size();

    //for ( int i = 0; i < mesh_files.size(); ++i )
    if (i < mesh_files.size())
    {
      //printf("Loading Mesh: %s/n", mesh_files[i]);

      Mesh* mesh = new Mesh();

      mesh->load_obj(mesh_files[i]);
      mesh->upload_obj();


      if (texture_files[i])
      {
        //printf("Loading Texture: %s/n", texture_files[i]);
        mesh->texture = new Texture();
        mesh->texture->loadPng(texture_files[i]);
      }

      m_meshes.push_back(mesh);
    }

    if (loading()) return;

    // Finalise

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
