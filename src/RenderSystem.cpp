#include <RenderSystem.h>
#include "shaders.h"
#include "Mesh.h"
#include <iostream>
#include "Menu.h"
#include "Simulation.h"
#include <array>


// Uniforms OLD
GLint uniformDiffuse;
GLint uniformMVP;
GLint uniform_s_GBuffer;
GLint lightPos, lightCol;


RenderSystem::RenderSystem() : m_menu(new Menu())
{
  //
  glEnable(GL_DEPTH_TEST);

   // Geometry pass shaders

  	const char vertexShaderSource[] =
  		"attribute vec4 vPosition;		                     \n"
      "attribute vec2 vUV; \n"
      "uniform mat4 MVP; \n"
      "uniform vec3 diffuse;  \n"
  		"varying vec3 color;                                 \n" // To FS
      "varying vec2 uv;                                 \n"
  		"void main()                                         \n"
  		"{                                                   \n"
  		"   gl_Position = MVP * vPosition;                         \n"
      "   color = diffuse;             \n"
      "   uv = vUV; \n"
  		"}                                                   \n";

  	const char fragmentShaderSource[] =
  		"precision mediump float;                     \n"
  //		 "precision highp float;                     \n"
  		 "varying vec3 color;                          \n"   // From VS
       "varying vec2 uv;                                 \n"
  		"void main()                                  \n"
  		"{                                            \n"
       "  gl_FragColor = vec4 ( color, 1.0 );        \n"

// "      if (uv.x < 0.333)\n"
//       "{\n"
//     "    if (uv.y < 0.5) gl_FragColor = vec4(111.0/255.0, 192.0/255.0, 211.0/255.0, 1.0);\n"
//     "    else gl_FragColor = vec4(158.0/255.0, 119.0/255.0, 88.0/255.0, 1.0);\n"
//   "    }\n"
//     "  else if (uv.x < 0.666)\n"
//     "  {\n"
//     "    if (uv.y < 0.5) gl_FragColor = vec4(216.0/255.0, 209.0/255.0, 157.0/255.0, 1.0);\n\n"
//       "  else gl_FragColor = vec4(106.0/255.0, 106.0/255.0, 106.0/255.0, 1.0);\n"
//   "    }\n"
//     "  else\n"
//     "  {\n"
//   "      if (uv.y < 0.5) gl_FragColor = vec4(139.0/255.0, 180.0/255.0, 74.0/255.0, 1.0);\n"
//       "  else gl_FragColor = vec4(205.0/255.0, 202.0/255.0, 193.0/255.0, 1.0);\n"
//       "}    \n"

  		"}                                            \n";

  	//load vertex and fragment shaders
  	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderSource);
  	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
  	programObject = buildProgram(vertexShader, fragmentShader, "vPosition", "vNormal");

    Mesh::attribute_v_coord = glGetAttribLocation(programObject, "vPosition");
    Mesh::attribute_v_uv = glGetAttribLocation(programObject, "vUV");

  	//save location of uniform variables
    uniformDiffuse = glGetUniformLocation(programObject, "diffuse");
    uniformMVP = glGetUniformLocation(programObject, "MVP");

		// Set up G-Buffer stuff
		unsigned int SCR_WIDTH = 1280;
		unsigned int SCR_HEIGHT = 720;

		glGenFramebuffers(1, &m_gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

		// - position/color/normal buffer [create]
    GLuint buffer[3];
    glGenTextures(3, buffer);
    m_colBuffer = buffer[0];
    m_norBuffer = buffer[1];
    m_depBuffer = buffer[2];

		glBindTexture(GL_TEXTURE_2D, m_colBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, m_norBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //glBindTexture(GL_TEXTURE_2D, m_depBuffer);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// - position/color/normal buffer [resize]
		glBindTexture(GL_TEXTURE_2D, m_colBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glBindTexture(GL_TEXTURE_2D, m_norBuffer);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RG, GL_FLOAT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);

    //glBindTexture(GL_TEXTURE_2D, m_depBuffer);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glBindTexture(GL_TEXTURE_2D, m_depBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);

		// - fbo [resize]
		glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_norBuffer, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_posBuffer, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depBuffer, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);






    std::vector<const char*> resource_files;
    resource_files.push_back("data/EngineHouse.obj");
    resource_files.push_back("data/WaterTank-edit1.obj");
    resource_files.push_back("data/cellphone.obj");
    resource_files.push_back("data/Generator.obj");
    resource_files.push_back("data/SatReceiver.obj");
    for (int i = 0; i < resource_files.size(); ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(resource_files[i]);
      pMeshLoad->upload_obj();
      pMeshLoad->setColourSceme(3);
      resource_meshes.push_back(pMeshLoad);
    }

    std::vector<const char*> pipe_files;
    pipe_files.push_back("data/Pipe.obj");
    pipe_files.push_back("data/PipeBend.obj");
    pipe_files.push_back("data/PipeT.obj");
    pipe_files.push_back("data/Pipe4ways.obj");
    for (int i = 0; i < pipe_files.size(); ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(pipe_files[i]);
      pMeshLoad->upload_obj();
      pMeshLoad->setColourSceme(3);
      pipe_meshes.push_back(pMeshLoad);
    }

    std::vector<const char*> house_files;
    house_files.push_back("data/HousesPackLowPoly_house1.obj");
    house_files.push_back("data/HousesPackLowPoly_house2.obj");
    house_files.push_back("data/HousesPackLowPoly_house3.obj");
    house_files.push_back("data/HousesPackLowPoly_house4.obj");
    house_files.push_back("data/HousesPackLowPoly_house5.obj");
    house_files.push_back("data/BuildingLowpoly.obj");
    house_files.push_back("data/SkyscraperHex.obj");
    for (int i = 0; i < house_files.size(); ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(house_files[i]);
      pMeshLoad->upload_obj();
      pMeshLoad->setColourSceme(5 + i%5); // House Material
      house_meshes.push_back(pMeshLoad);
    }

    std::vector<const char*> road_files;

    road_files.push_back("data/3D Road Pack/Models/roadTile_277-edit2.obj"); // straight
    road_files.push_back("data/3D Road Pack/Models/roadTile_299-edit2.obj"); // corner
    road_files.push_back("data/3D Road Pack/Models/roadTile_148-edit2.obj"); //  T
    road_files.push_back("data/3D Road Pack/Models/roadTile_260-edit2.obj"); // 4 ways
    // 287 3-way
    for (int i = 0; i < road_files.size(); ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(road_files[i]);
      pMeshLoad->upload_obj();
      pMeshLoad->setColourSceme(1);
      road_meshes.push_back(pMeshLoad);
    }

    std::vector<const char*> cliff_files;
    cliff_files.push_back("data/CliffBottom0.obj");
    cliff_files.push_back("data/CliffBottomCorner0.obj");
    for (int i = 0; i < cliff_files.size(); ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(cliff_files[i]);
      pMeshLoad->upload_obj();
      pMeshLoad->setColourSceme(100);
      cliff_meshes.push_back(pMeshLoad);
    }

    std::vector<const char*> tree_files;
    tree_files.push_back("data/lowpoly_tree/obj/untitled.obj");
    tree_files.push_back("data/Tree1.obj");
    tree_files.push_back("data/Tree2.obj");
    tree_files.push_back("data/Oak1.obj");
    for (int i = 0; i < tree_files.size(); ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(tree_files[i]);
      pMeshLoad->upload_obj();
      pMeshLoad->setColourSceme(2);
      tree_meshes.push_back(pMeshLoad);
    }
}


RenderSystem::~RenderSystem()
{


}


void RenderSystem::drawTile(int i, int j, glm::mat4 MVP)
{
  if (m_cityMap == 0) return;

  glm::mat4 local = glm::rotate(MVP, 3.14f, glm::vec3(0.f, 0.f, 1.f));
  //glm::mat4 local = MVP;
  glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

  // Get the data
  CityTile& tile = m_cityMap->getTile(i, j);

  // Surface
  if (m_menu->getEarthLayer() == 1)
  {
    float colour0[] = { 123.f / 255.f, 179.f / 255.f,  105.f / 255.f }; // Grass Green
    float colour1resi[] = { 88.f / 255.f, 215.f / 255.f,  88.f / 255.f }; //
    float colour1comm[] = { 88.f / 255.f, 88.f / 255.f,  215.f / 255.f }; //

    glUniform3fv(uniformDiffuse, 1, colour0);

    if (tile.tileType == TILE_ZONE)
    {
      if (((ZoneData&)tile.data).buildAng1 == 0)
      {
        if (((ZoneData&)tile.data).zone == 1) glUniform3fv(uniformDiffuse, 1, colour1resi);
        else if (((ZoneData&)tile.data).zone == 2) glUniform3fv(uniformDiffuse, 1, colour1comm);
      }
    }

    drawObjModel_square();

    // Objects
    if (tile.tileType == TILE_TREE)
    {
      TreeData& data = (TreeData&)tile.data;

      glm::mat4 sublocal = glm::scale(local, glm::vec3(0.25f));

      glm::mat4 sublocal2 = glm::translate(sublocal, glm::vec3(1.0f, 1.0f, 0.0f));
      if (data.treeAng1)
      {
        sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * data.treeAng1), glm::vec3(0.f, 0.f, 1.f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal2[0][0]);
        Mesh* pMesh = data.park ? tree_meshes[3] : tree_meshes[data.treeAng1 % 3];
        pMesh->draw(uniformDiffuse);
      }

      sublocal2 = glm::translate(sublocal, glm::vec3(1.0f, -1.0f, 0.0f));
      if (data.treeAng2)
      {
        sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * data.treeAng2), glm::vec3(0.f, 0.f, 1.f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal2[0][0]);
        Mesh* pMesh = data.park ? tree_meshes[3] : tree_meshes[data.treeAng1 % 3];
        pMesh->draw(uniformDiffuse);
      }

      sublocal2 = glm::translate(sublocal, glm::vec3(-1.0f, 1.0f, 0.0f));
      if (data.treeAng3)
      {
        sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * data.treeAng3), glm::vec3(0.f, 0.f, 1.f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal2[0][0]);
        Mesh* pMesh = data.park ? tree_meshes[3] : tree_meshes[data.treeAng1 % 3];
        pMesh->draw(uniformDiffuse);
      }

      sublocal2 = glm::translate(sublocal, glm::vec3(-1.0f, -1.0f, 0.0f));
      if (data.treeAng4)
      {
        sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * data.treeAng4), glm::vec3(0.f, 0.f, 1.f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal2[0][0]);
        Mesh* pMesh = data.park ? tree_meshes[3] : tree_meshes[data.treeAng1 % 3];
        pMesh->draw(uniformDiffuse);
      }
    }
    if (tile.tileType == TILE_ZONE)
    {
      ZoneData& data = (ZoneData&)tile.data;
      glm::mat4 local = glm::rotate(MVP, 0.f, glm::vec3(0.f, 0.f, 1.f));
      if (data.zone == 1)
        local = glm::scale(local, glm::vec3(0.35f, 0.35f, 0.35f));
      if (data.zone == 2)
        local = glm::scale(local, glm::vec3(2.5f, 2.5f, 2.5f));
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

      int i = data.buildAng1 % (house_meshes.size() + 1);
      if (i > 0)
      {
        house_meshes[i - 1]->draw(uniformDiffuse);
      }
    }
    if (tile.tileType == TILE_BUILD)
    {
      BuildData& data = (BuildData&)tile.data;

      if (data.resourceType == 1) // Power substation
      {
        glm::mat4 local = glm::rotate(MVP, -1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
        local = glm::scale(local, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

        resource_meshes[0]->draw(uniformDiffuse);
      }
      if (data.resourceType == 2) // Water tower
      {
        glm::mat4 local = glm::rotate(MVP, -1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
        local = glm::scale(local, glm::vec3(2.f, 2.f, 2.f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

        resource_meshes[1]->draw(uniformDiffuse);
      }
      if (data.resourceType == 3) //
      {
        glm::mat4 local = glm::rotate(MVP, -1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
        local = glm::scale(local, glm::vec3(1.15f, 1.15f, 1.15f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

        resource_meshes[2]->draw(uniformDiffuse);
      }
      if (data.resourceType == 4) //
      {
        glm::mat4 local = glm::rotate(MVP, -1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
        local = glm::scale(local, glm::vec3(2.f, 2.f, 2.f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

        resource_meshes[3]->draw(uniformDiffuse);
      }
      if (data.resourceType == 5) //
      {
        glm::mat4 local = glm::rotate(MVP, -1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
        local = glm::scale(local, glm::vec3(2.f, 2.f, 2.f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

        resource_meshes[4]->draw(uniformDiffuse);
      }

    }

    if (tile.tileType == TILE_ROAD)
    {
      RoadData& data = (RoadData&)tile.data;
      glm::mat4 local = glm::rotate(MVP, (float)data.roadAng * 1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
      road_meshes[data.roadNum]->draw(uniformDiffuse);
    }
  }


  // Sky
  if (m_menu->getEarthLayer() == 0)
  {
    float colour0[] = { 200.f / 255.f, 224.f / 255.f,  224.f / 255.f }; // Skylight
    float colour1[] = { 205.f / 255.f, 178.f / 255.f,  165.f / 255.f }; // Dusty Rose
    float colourB[] = { 0.f, 0.f, 0.f };

    float a = (float)tile.airPollution / 255.f;
    colourB[0] = (1.f - a) * colour0[0] + a * colour1[0];
    colourB[1] = (1.f - a) * colour0[1] + a * colour1[1];
    colourB[2] = (1.f - a) * colour0[2] + a * colour1[2];

    glUniform3fv(uniformDiffuse, 1, colourB);

    drawObjModel_square();
  }

  // Underground
  if (m_menu->getEarthLayer() == 2)
  {
    // Show Utility Radius
    if (m_menu->getHover() >= 0 && m_menu->getHover() <= 2)
    {
      int res = m_menu->getHover() + 1;
      if (tile.surfaceUtilityFlag & (1<<res))
      {
        local = glm::translate(MVP, glm::vec3(0.f, 0.f, 1.0));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

        std::array<float, 3> colour0 = { 255.f / 255.f, 245.f / 255.f,  13.f / 255.f }; // Yellow
        glUniform3fv(uniformDiffuse, 1, colour0.data());

        drawObjModel_square();
      }
    }

    // Render underground scene
    if (tile.foundational)
    {
      float s = 0.8f;

      // std::array<float, 3> colour0 = { 217.f / 255.f, 214.f / 255.f,  255.f / 255.f }; // 0
      // std::array<float, 3> colour1 = { 217.f / 255.f, 214.f / 255.f,  195.f / 255.f }; // 1
      // std::array<float, 3> colour2 = { 217.f / 255.f, 214.f / 255.f,  225.f / 255.f }; // 2
      // std::array<float, 3> colour0a = { 197.f / 255.f, 255.f / 255.f,  195.f / 255.f }; // 0
      // std::array<float, 3> colour1a = { 197.f / 255.f, 225.f / 255.f,  195.f / 255.f }; // 1
      // std::array<float, 3> colour2a = { 197.f / 255.f, 194.f / 255.f,  195.f / 255.f }; // 2
      // std::array<float, 3> colour0b = { 255.f / 255.f, 194.f / 255.f,  195.f / 255.f }; // 0
      // std::array<float, 3> colour1b = { 225.f / 255.f, 194.f / 255.f,  195.f / 255.f }; // 1
      // std::array<float, 3> colour2b = { 197.f / 255.f, 194.f / 255.f,  195.f / 255.f }; // 2

      std::array<float, 3> colour0a = { 255.f / 255.f, 31.f / 255.f,  0.f / 255.f }; // Red
      std::array<float, 3> colour1a = { 127.f / 255.f, 49.f / 255.f,  38.f / 255.f };
      std::array<float, 3> colour2a = { 204.f / 255.f, 25.f / 255.f,  0.f / 255.f };
      std::array<float, 3> colour0 = { 34.f / 255.f, 255.f / 255.f,  13.f / 255.f }; // Green
      std::array<float, 3> colour1 = { 52.f / 255.f, 127.f / 255.f,  45.f / 255.f };
      std::array<float, 3> colour2 = { 27.f / 255.f, 204.f / 255.f,  10.f / 255.f };
      std::array<float, 3> colour0b = { 18.f / 255.f, 62.f / 255.f,  255.f / 255.f }; // Blue
      std::array<float, 3> colour1b = { 47.f / 255.f, 62.f / 255.f,  127.f / 255.f };
      std::array<float, 3> colour2b = { 15.f / 255.f, 50.f / 255.f,  204.f / 255.f };

      std::array<float, 3> colour0adull = { 0.5 * 255.f / 255.f, 0.5 *  31.f / 255.f, 0.5 *  0.f / 255.f }; // Red
      std::array<float, 3> colour1adull = { 0.5 * 127.f / 255.f, 0.5 *  49.f / 255.f, 0.5 *  38.f / 255.f };
      std::array<float, 3> colour2adull = { 0.5 * 204.f / 255.f, 0.5 *  25.f / 255.f, 0.5 *  0.f / 255.f };
      std::array<float, 3> colour0dull = { 0.5 * 34.f / 255.f,  0.5 * 255.f / 255.f, 0.5 *  13.f / 255.f }; // Green
      std::array<float, 3> colour1dull = { 0.5 * 52.f / 255.f,  0.5 * 127.f / 255.f, 0.5 *  45.f / 255.f };
      std::array<float, 3> colour2dull = { 0.5 * 27.f / 255.f,  0.5 * 204.f / 255.f, 0.5 *  10.f / 255.f };
      std::array<float, 3> colour0bdull = { 0.5 * 18.f / 255.f,  0.5 * 62.f / 255.f,  0.5 * 255.f / 255.f }; // Blue
      std::array<float, 3> colour1bdull = { 0.5 * 47.f / 255.f,  0.5 * 62.f / 255.f,  0.5 * 127.f / 255.f };
      std::array<float, 3> colour2bdull = { 0.5 * 15.f / 255.f,  0.5 * 50.f / 255.f,  0.5 * 204.f / 255.f };

      // Resource Box
      {
        BuildData& data = (BuildData&)tile.data;
        if (data.resourceType == 1) // power sub
        {
           s = 0.4f;
           if (tile.utilityFlag & (1<<1))
           {
             colour0 = colour0a;
             colour1 = colour1a;
             colour2 = colour2a;
           }
           else
           {
             colour0 = colour0adull;
             colour1 = colour1adull;
             colour2 = colour2adull;
           }
        }
        if (data.resourceType == 2) // water
        {
          colour0 = colour0b;
          colour1 = colour1b;
          colour2 = colour2b;
        }
        if (data.resourceType == 3) // data node
        {
           s = 0.4f;
           if (tile.utilityFlag & (1<<3))
           {
             colour0 = colour0;
             colour1 = colour1;
             colour2 = colour2;
           }
           else
           {
             colour0 = colour0dull;
             colour1 = colour1dull;
             colour2 = colour2dull;
           }
        }
        if (data.resourceType == 4) // power gen
        {
           colour0 = colour0a;
           colour1 = colour1a;
           colour2 = colour2a;
        }
        if (data.resourceType == 5) // data sat
        {
           colour0 = colour0;
           colour1 = colour1;
           colour2 = colour2;
        }
      }

      glm::mat4 local = glm::translate(MVP, glm::vec3(0.f, 0.f, 1.0 + 0.5f * s));
      local = glm::scale(local, glm::vec3(s, s, s));
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
      glUniform3fv(uniformDiffuse, 1, colour0.data());
      drawObjModel_square();

      local = glm::translate(MVP, glm::vec3(0.f, 0.f, 1.0));
      local = glm::scale(local, glm::vec3(s, s, s));
      local = glm::rotate(local, (float)1.57079632679f, glm::vec3(0.f, 1.f, 0.f));
      local = glm::translate(local, glm::vec3(0.f, 0.f, 0.5f));
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

      glUniform3fv(uniformDiffuse, 1, colour1.data());
      drawObjModel_square();

      local = glm::translate(MVP, glm::vec3(0.f, 0.f, 1.0));
      local = glm::scale(local, glm::vec3(s, s, s));
      local = glm::rotate(local, (float)-1.57079632679f, glm::vec3(1.f, 0.f, 0.f));
      local = glm::translate(local, glm::vec3(0.f, 0.f, 0.5f));
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
      glUniform3fv(uniformDiffuse, 1, colour2.data());
      drawObjModel_square();
    }

    if (tile.pipeType)
    {
      glm::mat4 local = glm::rotate(MVP, (float)tile.pipeAng * 1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
      local = glm::translate(local, glm::vec3(0.f, 0.f, 0.125f));
      //local = glm::scale(local, glm::vec3(2.f, 2.f, 2.f));
      local = glm::scale(local, glm::vec3(1.5f, 1.5f, 1.5f));
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

      pipe_meshes[tile.pipeRend]->draw(uniformDiffuse);
      if (tile.utilityFlag & (1<<tile.pipeType))
        pipe_meshes[tile.pipeRend]->drawWireframe(uniformDiffuse, tile.pipeType);
      else
        pipe_meshes[tile.pipeRend]->drawWireframe(uniformDiffuse, 10 + tile.pipeType);

      // ground under pipe
      local = MVP;
      //local = glm::scale(MVP, glm::vec3(0.95f, 0.95f, 0.95f));
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
      float colour0[] = { 97.f / 255.f, 94.f / 255.f,  95.f / 255.f }; // Granite
      glUniform3fv(uniformDiffuse, 1, colour0);
      drawObjModel_square();

      return;
    }
    else
    {
      glm::mat4 local = glm::scale(MVP, glm::vec3(0.5f, 0.5f, 0.5f));
      glm::mat4 sublocal = local;

      float h = 0.733f;

      sublocal = glm::translate(local, glm::vec3(0.5f, -0.5f, 0.0f));
      if (tile.un1)
      {
        drawCliffSide(sublocal, tile.un1, false);
      }
      else
      {
        sublocal = glm::translate(sublocal, glm::vec3(0.0f, 0.0f, h));
        //sublocal = glm::scale(sublocal, glm::vec3(0.95f, 0.95f, 0.95f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal[0][0]);
        float colour0[] = { 196.f / 255.f, 138.f / 255.f,  105.f / 255.f }; // Sandstone
        glUniform3fv(uniformDiffuse, 1, colour0);
        drawObjModel_square();
      }

      sublocal = glm::translate(local, glm::vec3(0.5f, 0.5f, 0.0f));
      if (tile.un2)
      {
        sublocal = glm::rotate(sublocal, (float)1.57079632679f, glm::vec3(0.f, 0.f, 1.0f));
        drawCliffSide(sublocal, tile.un2, false);
      }
      else
      {
        sublocal = glm::translate(sublocal, glm::vec3(0.0f, 0.0f, h));
        //sublocal = glm::scale(sublocal, glm::vec3(0.95f, 0.95f, 0.95f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal[0][0]);
        float colour0[] = { 196.f / 255.f, 138.f / 255.f,  105.f / 255.f }; // Sandstone
        glUniform3fv(uniformDiffuse, 1, colour0);
        drawObjModel_square();
      }

      sublocal = glm::translate(local, glm::vec3(-0.5f, 0.5f, 0.0f));
      if (tile.un4)
      {
        sublocal = glm::rotate(sublocal, (float)2.f * 1.57079632679f, glm::vec3(0.f, 0.f, 1.0f));

        drawCliffSide(sublocal, tile.un4, false);
      }
      else
      {
        sublocal = glm::translate(sublocal, glm::vec3(0.0f, 0.0f, h));
        //sublocal = glm::scale(sublocal, glm::vec3(0.95f, 0.95f, 0.95f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal[0][0]);
        float colour0[] = { 196.f / 255.f, 138.f / 255.f,  105.f / 255.f }; // Sandstone
        glUniform3fv(uniformDiffuse, 1, colour0);
        drawObjModel_square();
      }

      sublocal = glm::translate(local, glm::vec3(-0.5f, -0.5f, 0.0f));
      if (tile.un3)
      {
        sublocal = glm::rotate(sublocal, (float)3.f * 1.57079632679f, glm::vec3(0.f, 0.f, 1.0f));

        drawCliffSide(sublocal, tile.un3, false);
      }
      else
      {
        sublocal = glm::translate(sublocal, glm::vec3(0.0f, 0.0f, h));
        //sublocal = glm::scale(sublocal, glm::vec3(0.95f, 0.95f, 0.95f));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal[0][0]);
        float colour0[] = { 196.f / 255.f, 138.f / 255.f,  105.f / 255.f }; // Sandstone
        glUniform3fv(uniformDiffuse, 1, colour0);
        drawObjModel_square();
      }

      return;
    }

    // local = glm::translate(local, glm::vec3(0.0f, 0.0f, 1.0f));
    // local = glm::scale(local, glm::vec3(0.95f, 0.95f, 0.95f));
    // glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
    // drawObjModel_square();
  }
}


void RenderSystem::drawCliffSide(glm::mat4 MVP, char i, bool h)
{
  glm::mat4 local = MVP;
  //local = glm::scale(local, glm::vec3(0.95f, 0.95f, 0.95f));
  glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
  float colour0[] = { 97.f / 255.f, 94.f / 255.f,  95.f / 255.f }; // Granite
  glUniform3fv(uniformDiffuse, 1, colour0);
  drawObjModel_square();

  if (h) return;
  //local = MVP;
  //MVP = glm::translate(MVP, glm::vec3(-0.5f, 0.0f, 0.0f));
  //glm::mat4 local = glm::translate(MVP, glm::vec3(0.0f, -0.5f, 0.0f));

  if (i == 1) // corner
  {
    local = glm::rotate(MVP, (float)1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    local = glm::translate(local, glm::vec3(0.5f, 0.5f, 0.0f));
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
    cliff_meshes[1]->draw(uniformDiffuse);
  }
  if (i == 2)
  {
    local = glm::rotate(MVP, (float)1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    local = glm::translate(local, glm::vec3(0.5f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
    cliff_meshes[0]->draw(uniformDiffuse);
  }
  if (i == 3)
  {
    local = glm::rotate(MVP, (float)2.f * 1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    local = glm::translate(local, glm::vec3(0.5f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
    cliff_meshes[0]->draw(uniformDiffuse);
  }
  if (i == 4) // inner corner
  {
    // local = MVP;
    local = glm::rotate(local, (float)2.f * 1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    local = glm::translate(local, glm::vec3(0.5f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
    cliff_meshes[0]->draw(uniformDiffuse);

    local = glm::rotate(MVP, (float)1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    local = glm::translate(local, glm::vec3(0.5f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
    cliff_meshes[0]->draw(uniformDiffuse);
  }
}


void RenderSystem::move(double x, double y)
{
   m_cameraX += 0.05 * -x / m_zoom + 0.05 * y / m_zoom;
   m_cameraY += 0.05 *  x / m_zoom + 0.05 * y / m_zoom;
}


void RenderSystem::zoom(int i)
{
  if (i > 0) m_zoomFactor += 1;
  if (i < 0) m_zoomFactor -= 1;
  if (m_zoomFactor < 2) m_zoomFactor = 2;
  if (m_zoomFactor > 6) m_zoomFactor = 6;

  m_zoom = 0.5 + 0.5 * (1<<m_zoomFactor);
}


void RenderSystem::cursor_pos(double x, double y)
{
  m_cursorX = x / (1280. * 0.5) - 1.0;
  m_cursorY = y / (720.0 * 0.5) - 1.0;
}


void RenderSystem::draw()
{
  // Draw the whole screen
  glDepthFunc(GL_LESS);

  // GLenum buffers3[] = {
  //       GL_COLOR_ATTACHMENT0,
  //       GL_COLOR_ATTACHMENT1,
  //       GL_COLOR_ATTACHMENT2,
  // };

  // 1. Render to g-buffer
  //glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
  //glDrawBuffers(2, buffers3); // number of buffers
  // glDepthFunc(GL_LESS);
  draw1Pass();

  // 2. Present g-buffer with lights
  //glBindFramebuffer(GL_FRAMEBUFFER, m_default_framebuffer);
  //glDrawBuffers(1, buffers3);
  //draw2Pass();

  // 3. Render the UI overlay
  glDepthFunc(GL_ALWAYS);
  drawUI();

  static int counter  = 0;
  counter++;
  if (counter >= 60)
  {
    m_cityMap->update();
    Simulation::doSkyStep(m_cityMap);
    counter = 0;
  }
}


static int ci = 0;
static int cj = 0;
bool RenderSystem::performTileAction(int i)
{
  if (m_cityMap == 0) return false;

  // Get the data
  CityTile& tile = m_cityMap->getTile(ci, cj);
  if (tile.isDefault) return false;

  //tile.object_type = (tile.object_type + 1) % 4;
  //tile.road = (tile.road + 1) % (road_meshes.size() + 1);

  // Surface layer tools
  if (m_menu->getEarthLayer() == 1)
  {

    // Road !
    if (i == ACTION_ROAD)
    {
      if (tile.tileType == TILE_TREE)
      {
        tile.tileType = TILE_VOID;
      }
      if (tile.tileType != TILE_VOID) return true;

      //if (tile.road) tile.road = 0;
      //else
      tile.tileType = TILE_ROAD;

      checkRoadTile(ci, cj);
      checkRoadTile(ci, cj+1);
      checkRoadTile(ci, cj-1);
      checkRoadTile(ci-1, cj);
      checkRoadTile(ci+1, cj);

      return true;
    }

    // Res zone
    if (i == ACTION_RESI)
    {
      if (tile.tileType == TILE_TREE)
      {
        tile.tileType = TILE_VOID;
      }
      if (tile.tileType != TILE_VOID) return true;
      tile.tileType = TILE_ZONE;
      ZoneData& data = (ZoneData&)tile.data;
      data.object_type = 2;
      data.buildAng1 = 0;
      data.buildAng2 = 0;
      data.zone = 1;

      return true;
    }

    if (i == ACTION_COMM)
    {
      if (tile.tileType == TILE_TREE)
      {
        tile.tileType = TILE_VOID;
      }
      if (tile.tileType != TILE_VOID) return true;
      tile.tileType = TILE_ZONE;
      ZoneData& data = (ZoneData&)tile.data;
      data.object_type = 5;
      data.buildAng1 = 0;
      data.buildAng2 = 0;
      data.zone = 2;

      return true;
    }

    // Power Zone
    if (i == ACTION_POWER)
    {
      if (tile.pipeType && tile.pipeType != 1) return false;

      if (tile.tileType == TILE_TREE)
      {
        tile.tileType = TILE_VOID;
      }
      if (tile.tileType != TILE_VOID) return false;

      // ??? tile.object_type = 3;
      tile.tileType = TILE_BUILD;
      BuildData& data = (BuildData&)tile.data;
      data.resourceType = 1;
      data.resourceValue = 250000;

      tile.foundational = 1;
      tile.pipeType = 1;
      tile.pipeRend = 3;
      checkPipeTiles(ci, cj);
      Simulation::doConnectivity(m_cityMap, 1);
    }

    // Water Zone
    if (i == ACTION_WATER)
    {
      if (tile.pipeType && tile.pipeType != 2) return false;

      if (tile.tileType == TILE_TREE)
      {
        tile.tileType = TILE_VOID;
      }
      if (tile.tileType != TILE_VOID) return false;

      // ??? tile.object_type = 3;
      tile.tileType = TILE_BUILD;
      BuildData& data = (BuildData&)tile.data;
      data.resourceType = 2;
      data.resourceValue = 270000;

      tile.foundational = 1;
      tile.pipeType = 2;
      tile.pipeRend = 3;
      checkPipeTiles(ci, cj);
      tile.utilityFlag = (1<<2);
      Simulation::doConnectivity(m_cityMap, 2);
    }

    // Data Zone (small)
    if (i == ACTION_DATA)
    {
      if (tile.pipeType && tile.pipeType != 3) return false;

      if (tile.tileType == TILE_TREE)
      {
        tile.tileType = TILE_VOID;
      }
      if (tile.tileType != TILE_VOID) return false;

      // ??? tile.object_type = 3;
      tile.tileType = TILE_BUILD;
      BuildData& data = (BuildData&)tile.data;
      data.resourceType = 3;
      data.resourceValue = 150000;

      tile.foundational = 1;
      tile.pipeType = 3;
      tile.pipeRend = 3;
      checkPipeTiles(ci, cj);
      Simulation::doConnectivity(m_cityMap, 3);
    }

    // Power Zone (Generator)
    if (i == ACTION_POWER2)
    {
      if (tile.pipeType && tile.pipeType != 1) return false;

      if (tile.tileType == TILE_TREE)
      {
        tile.tileType = TILE_VOID;
      }
      if (tile.tileType != TILE_VOID) return false;

      // ??? tile.object_type = 3;
      tile.tileType = TILE_BUILD;
      BuildData& data = (BuildData&)tile.data;
      data.resourceType = 4;
      data.resourceValue = 1250000;

      tile.foundational = 1;
      tile.pipeType = 1;
      tile.pipeRend = 3;
      checkPipeTiles(ci, cj);
      tile.utilityFlag = (1<<1);
      Simulation::doConnectivity(m_cityMap, 1);
    }

    // Data Zone (Large)
    if (i == ACTION_DATA2)
    {
      if (tile.pipeType && tile.pipeType != 3) return false;

      if (tile.tileType == TILE_TREE)
      {
        tile.tileType = TILE_VOID;
      }
      if (tile.tileType != TILE_VOID) return false;

      // ??? tile.object_type = 3;
      tile.tileType = TILE_BUILD;
      BuildData& data = (BuildData&)tile.data;
      data.resourceType = 5;
      data.resourceValue = 640000;

      tile.foundational = 1;
      tile.pipeType = 3;
      tile.pipeRend = 3;
      checkPipeTiles(ci, cj);
      tile.utilityFlag = (1<<3);
      Simulation::doConnectivity(m_cityMap, 3);
    }

    // Park Zone
    if (i == ACTION_PARK)
    {
      if (tile.tileType != TILE_VOID) return false;

      tile.tileType = TILE_TREE;

      TreeData& data = (TreeData&)tile.data;
      data.treeAng1 = 1;
      data.treeAng2 = 1;
      data.treeAng3 = 1;
      data.treeAng4 = 1;
      data.park = 1;
    }

    // Bulldoze
    if (i == ACTION_BULLDOZE)
    {
      tile.tileType = TILE_VOID;

      checkRoadTile(ci, cj+1);
      checkRoadTile(ci, cj-1);
      checkRoadTile(ci-1, cj);
      checkRoadTile(ci+1, cj);

      if (tile.foundational)
      {
        int res = tile.pipeType;
        tile.foundational = 0;
        tile.pipeType = 0;
        checkPipeTiles(ci, cj);
        Simulation::doConnectivity(m_cityMap, res);
      }
    }
  }

  // Pipe layer
  if (m_menu->getEarthLayer() == 2)
  {
    if (i == 0)
    {
      if (tile.pipeType == 0)
      {
        tile.pipeType = 1;
        checkPipeTiles(ci, cj);
        Simulation::doConnectivity(m_cityMap, 1);
      }
      return true;
    }
    if (i == 1)
    {
      if (tile.pipeType == 0)
      {
        tile.pipeType = 2;
        checkPipeTiles(ci, cj);
        Simulation::doConnectivity(m_cityMap, 2);
      }
      return true;
    }
    if (i == 2)
    {
      if (tile.pipeType == 0)
      {
        tile.pipeType = 3;
        checkPipeTiles(ci, cj);
        Simulation::doConnectivity(m_cityMap, 3);
      }
      return true;
    }
    if (i == 3)
    {
      if (tile.foundational) return false;

      if (tile.pipeType != 0)
      {
        int res = tile.pipeType;
        tile.pipeType = 0;
        checkPipeTiles(ci, cj);
        Simulation::doConnectivity(m_cityMap, res);
      }
      return false;
    }
  }

  return false;
}


void RenderSystem::checkUndergroundTile(int i, int j)
{
  CityTile& tile = m_cityMap->getTile(i, j);

  if (tile.pipeType)
  {
    return;
  }

  CityTile& t_n = m_cityMap->getTile(i, j - 1);
  CityTile& t_s = m_cityMap->getTile(i, j + 1);
  CityTile& t_e = m_cityMap->getTile(i + 1, j);
  CityTile& t_w = m_cityMap->getTile(i - 1, j);

  CityTile& t_ne = m_cityMap->getTile(i + 1, j - 1);
  CityTile& t_se = m_cityMap->getTile(i + 1, j + 1);
  CityTile& t_nw = m_cityMap->getTile(i - 1, j - 1);
  CityTile& t_sw = m_cityMap->getTile(i - 1, j + 1);

  char a1 = checkUndergroundTile(t_n, t_e, t_ne);
  char a2 = checkUndergroundTile(t_e, t_s, t_se);
  char a3 = checkUndergroundTile(t_w, t_n, t_nw);
  char a4 = checkUndergroundTile(t_s, t_w, t_sw);

  tile.un1 = a1;
  tile.un2 = a2;
  tile.un3 = a3;
  tile.un4 = a4;
}


int RenderSystem::checkUndergroundTile(const CityTile& a, const CityTile& b, const CityTile& c)
{
  if (a.pipeType && b.pipeType) return 1; // corner
  if (a.pipeType) return 2; // edge
  if (b.pipeType) return 3; // edge
  if (c.pipeType) return 4; // inner corner

  return 0; //  nothing
}


void RenderSystem::checkPipeTiles(int i, int j)
{
  checkPipeTile(i, j);
  checkPipeTile(i+1, j);
  checkPipeTile(i-1, j);
  checkPipeTile(i, j+1);
  checkPipeTile(i, j-1);

  checkUndergroundTile(i, j);
  checkUndergroundTile(i+1, j);
  checkUndergroundTile(i-1, j);
  checkUndergroundTile(i, j+1);
  checkUndergroundTile(i, j-1);

  checkUndergroundTile(i+1, j+1);
  checkUndergroundTile(i-1, j+1);
  checkUndergroundTile(i+1, j-1);
  checkUndergroundTile(i-1, j-1);
}


void RenderSystem::checkPipeTile(int i, int j)
{
  CityTile& tile = m_cityMap->getTile(i, j);

  if (tile.pipeType == 0) return;
  if (tile.foundational) return;

  CityTile& t_n = m_cityMap->getTile(i, j - 1);
  CityTile& t_s = m_cityMap->getTile(i, j + 1);
  CityTile& t_e = m_cityMap->getTile(i + 1, j);
  CityTile& t_w = m_cityMap->getTile(i - 1, j);

  int x = 0;
  if (t_n.pipeType == tile.pipeType) x += 1;
  if (t_s.pipeType == tile.pipeType) x += 2;
  if (t_e.pipeType == tile.pipeType) x += 4;
  if (t_w.pipeType == tile.pipeType) x += 8;

  // ..nsew
  if (x == 0b0000)
  {
    tile.pipeRend = 0;
    tile.pipeAng = 0;
  }
  else if (x == 0b0001)
  {
    tile.pipeRend = 0;
    tile.pipeAng = 1;
  }
  else if (x == 0b0010)
  {
    tile.pipeRend = 0;
    tile.pipeAng = 1;
  }
  else if (x == 0b0011)
  {
    tile.pipeRend = 0;
    tile.pipeAng = 1;
  }
  else if (x == 0b0100)
  {
    tile.pipeRend = 0;
    tile.pipeAng = 0;
  }
  else if (x == 0b0101)
  {
    tile.pipeRend = 1;
    tile.pipeAng = 0;
  }
  else if (x == 0b0110)
  {
    tile.pipeRend = 1;
    tile.pipeAng = 1;
  }
  else if (x == 0b0111)
  {
    tile.pipeRend = 2;
    tile.pipeAng = 0;
  }
  else if (x == 0b1000)
  {
    tile.pipeRend = 0;
    tile.pipeAng = 0;
  }
  else if (x == 0b1001)
  {
    tile.pipeRend = 1;
    tile.pipeAng = 3;
  }
  else if (x == 0b1010)
  {
    tile.pipeRend = 1;
    tile.pipeAng = 2;
  }
  else if (x == 0b1011)
  {
    tile.pipeRend = 2;
    tile.pipeAng = 2;
  }
  else if (x == 0b1100)
  {
    tile.pipeRend = 0;
    tile.pipeAng = 0;
  }
  else if (x == 0b1101)
  {
    tile.pipeRend = 2;
    tile.pipeAng = 3;
  }
  else if (x == 0b1110)
  {
    tile.pipeRend = 2;
    tile.pipeAng = 1;
  }
  else if (x == 0b1111)
  {
    tile.pipeRend = 3;
    tile.pipeAng = 0;
  }

}


void RenderSystem::checkRoadTile(int i, int j)
{
  CityTile& t = m_cityMap->getTile(i, j);

  if (t.tileType != TILE_ROAD) return;

  CityTile& t_n = m_cityMap->getTile(i, j - 1);
  CityTile& t_s = m_cityMap->getTile(i, j + 1);
  CityTile& t_e = m_cityMap->getTile(i + 1, j);
  CityTile& t_w = m_cityMap->getTile(i - 1, j);

  int num = 0;
  if (t_n.tileType == TILE_ROAD) num += 1;
  if (t_s.tileType == TILE_ROAD) num += 1;
  if (t_e.tileType == TILE_ROAD) num += 1;
  if (t_w.tileType == TILE_ROAD) num += 1;

  int x = 0;
  if (t_n.tileType == TILE_ROAD) x += 1;
  if (t_s.tileType == TILE_ROAD) x += 2;
  if (t_e.tileType == TILE_ROAD) x += 4;
  if (t_w.tileType == TILE_ROAD) x += 8;

  RoadData& data = (RoadData&)t.data;

  data.roadNum = num;


  // ..nsew
  if (x == 0b0000)
  {
    data.roadNum = 0;
    data.roadAng = 0;
  }
  else if (x == 0b0001)
  {
    data.roadNum = 0;
    data.roadAng = 1;
  }
  else if (x == 0b0010)
  {
    data.roadNum = 0;
    data.roadAng = 1;
  }
  else if (x == 0b0011)
  {
    data.roadNum = 0;
    data.roadAng = 1;
  }
  else if (x == 0b0100)
  {
    data.roadNum = 0;
    data.roadAng = 0;
  }
  else if (x == 0b0101)
  {
    data.roadNum = 1;
    data.roadAng = 0;
  }
  else if (x == 0b0110)
  {
    data.roadNum = 1;
    data.roadAng = 1;
  }
  else if (x == 0b0111)
  {
    data.roadNum = 2;
    data.roadAng = 0;
  }
  else if (x == 0b1000)
  {
    data.roadNum = 0;
    data.roadAng = 0;
  }
  else if (x == 0b1001)
  {
    data.roadNum = 1;
    data.roadAng = 3;
  }
  else if (x == 0b1010)
  {
    data.roadNum = 1;
    data.roadAng = 2;
  }
  else if (x == 0b1011)
  {
    data.roadNum = 2;
    data.roadAng = 2;
  }
  else if (x == 0b1100)
  {
    data.roadNum = 0;
    data.roadAng = 0;
  }
  else if (x == 0b1101)
  {
    data.roadNum = 2;
    data.roadAng = 3;
  }
  else if (x == 0b1110)
  {
    data.roadNum = 2;
    data.roadAng = 1;
  }
  else if (x == 0b1111)
  {
    data.roadNum = 3;
    data.roadAng = 0;
  }

}


void RenderSystem::draw1Pass()
{
  glClearColor(0.f, 92.f / 255.f, 159.f / 255.f, 1.f); // "Skydiver"
  glClear( GL_COLOR_BUFFER_BIT );

  // Enable our shader program
  glUseProgram(programObject);



  // Draw the tiled terrain

  // Camera matrix
  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  //glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  // Or, for an ortho camera :
  //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
  float v_side = 0.05f / m_zoom;
  glm::mat4 Projection = glm::ortho(-v_side*1280.f, v_side*1280.f, -v_side*720.f, v_side*720.f, 1.0f, 100.0f);

  // Camera matrix
  glm::mat4 View       = glm::lookAt(
                glm::vec3(m_cameraX+30,m_cameraY+30, 40), // Camera is at (4,3,3), in World Space
                glm::vec3(m_cameraX+0,m_cameraY+0, 0), // and looks at the origin
                glm::vec3(0,0,1)  // Head is up (set to 0,-1,0 to look upside-down)
               );

  glm::mat4 VP = Projection * View;

  for (int i = 0; i < m_cityMap->m_width; ++i)
  {
    for (int j = 0; j < m_cityMap->m_height; ++j)
    {
      // Model matrix : an identity matrix (model will be at the origin)
      //glm::mat4 Model      = glm::mat4(1.0f);
      glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0*i, 1.0*j, 0));
      // Our ModelViewProjection : multiplication of our 3 matrices
      glm::mat4 MVP = VP * Model;
      // Upload it
      //glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &MVP[0][0]);

      // if (onscreen)
      float dx = i - m_cameraX;
      float dy = j - m_cameraY;
      // if (dx + dy > -20 && dx + dy < 20 &&
      //     dx - dy > -19 && dx - dy < 24)

      if ((dx + dy + 1)*m_zoom > -99 && (dx + dy - 1)*m_zoom < 99 &&
          (dx - dy + 1)*m_zoom > -99 && (dx - dy - 1)*m_zoom < 99 )
      {
        drawTile(i, j, MVP);
      }
    }
  }
/*
  //float colour1[] = { 159.f / 255.f, 175.f / 255.f,  108.f / 255.f }; // Leaf Green
  float colour1[] = { 58.f / 255.f, 121.f / 255.f,  94.f / 255.f }; // Pine Green
  glUniform3fv(uniformDiffuse, 1, colour1);

  for (int i = 2; i < 16; ++i)
  {
    for (int j = i%4; j < 16; j+=i)
    {
      glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0*i, 1.0*j, 0));
      glm::mat4 MVP = VP * Model;
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &MVP[0][0]);

      //drawObjModel_obj();
      pMesh->draw();
    }
  }

  glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0*7, 1.0*8, 0));
  glm::mat4 MVP = VP * Model;
  glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &MVP[0][0]);
  float colour22[] = { 1.0, 1.0, 1.0 };
  glUniform3fv(uniformDiffuse, 1, colour22);
  pMesh2->draw();


  Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0*5, 1.0*5, 0));
  MVP = VP * Model;
  glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &MVP[0][0]);
  pMeshGas->draw();
*/
  // Cursor

  // Do ray intersect
  // these positions must be in range [-1, 1] (!!!), not [0, width] and [0, height]
  // float mouseX = getMousePositionX() / (getWindowWidth()  * 0.5f) - 1.0f;
  // float mouseY = getMousePositionY() / (getWindowHeight() * 0.5f) - 1.0f;

  glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0*7, 1.0*8, 0));
  glm::mat4 MVP = VP * Model;

  glm::mat4 invVP = glm::inverse(VP);
  glm::vec4 screenPos = glm::vec4(m_cursorX, -m_cursorY, 1.0f, 1.0f);
  glm::vec3 worldPos = glm::vec3(invVP * screenPos);

  glm::vec3 dir = glm::normalize(glm::vec3(-3.f, -3.f, -4.f));
  //glm::vec3 dir = glm::normalize(glm::vec3(worldPos));




  float intersectionDistance = 0.f;
  if (glm::intersectRayPlane(worldPos, dir, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), intersectionDistance))
  {
    glm::vec3 target = worldPos + intersectionDistance * dir;

    ci = (int)(target.x + 0.5);
    cj = (int)(target.y + 0.5);
  }

  // Draw cursor
  if (m_showToolCursor)
  {
    float colourC[] = { 1.f, 1.f, 0.2f };
    glUniform3fv(uniformDiffuse, 1, colourC);


    Model = glm::translate(glm::mat4(1.0f), glm::vec3((float)ci, (float)cj, 0.51));
    MVP = VP * Model;
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &MVP[0][0]);

    resource_meshes[0]->draw_bbox(); // TODO
  }
}


void RenderSystem::hideToolCursor() { m_showToolCursor = false; }
void RenderSystem::showToolCursor() { m_showToolCursor = true; }

void  RenderSystem::drawUI()
{
  //glm::mat4 Model = glm::mat4(1.0f);
  //glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0*i, 1.0*j, 0));
  // int pixelWidth = 90;
  // int pixelHeight = 90;
  //
  // for (int i = 0; i < 10; ++i)
  // {
  //   float x = (i % 2) ? 238.5 : 110.5;
  //   float y = (i - (i % 2)) * 64.0 + 110.5;
  //
  //   glm::mat4 Model =
  //     glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 1.0, 0)) *
  //     glm::scale(glm::mat4(1.0f), glm::vec3(1.0 / 1280,  1.0 / 720, 1.0)) *
  //     glm::translate(glm::mat4(1.0f), glm::vec3(x, -y, 0)) *
  //     glm::scale(glm::mat4(1.0f), glm::vec3(pixelWidth, pixelHeight, 1.0));
  //
  //   glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &Model[0][0]);
  //
  //   drawObjModel_square();
  // }
  m_menu->draw(uniformDiffuse, uniformMVP);
}


void  RenderSystem::draw2Pass()
{
}


void  RenderSystem::RenderQuad()
{
	GLfloat vVertices2[] = {
    1.f,  1.f, 0.0f, 1.f, 1.0f,
    -1.f, -1.f, 0.0f, 0.f, 0.0f,
    1.f, -1.f, 0.0f, 1.f, 0.0f,
    1.f,  1.f, 0.0f, 1.f, 1.0f,
 	  -1.f,  1.f, 0.0f, 0.f, 1.0f,
    -1.f, -1.f, 0.0f, 0.f, 0.0f,
	};

	//enable our shader program
	//glUseProgram(m_programObject2);

	// This will identify our vertex buffer
	static GLuint vertexbuffer = 0;
	if (vertexbuffer == 0)
	{
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexbuffer);
		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices2), vVertices2, GL_STATIC_DRAW);

    std::cout << "making quad vbuffer" << std::endl;
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	float* ptr = 0;
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   5*sizeof(GLfloat),                  // stride
	   (void*)(ptr+0)            // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		 1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		 2,                  // size
		 GL_FLOAT,           // type
		 GL_FALSE,           // normalized?
		 5*sizeof(GLfloat),                  // stride
		 (void*)(ptr+3)            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}


void  RenderSystem::drawObjModel_square()
{

  // pos - uv
	// GLfloat vVertices2[] = {
  //    0.5f,  0.5f, 0.0f, 1.f, 1.0f,
  //   -0.5f, -0.5f, 0.0f, 0.f, 0.0f,
  //    0.5f, -0.5f, 0.0f, 1.f, 0.0f,
  //    0.5f,  0.5f, 0.0f, 1.f, 1.0f,
 	//   -0.5f,  0.5f, 0.0f, 0.f, 1.0f,
  //   -0.5f, -0.5f, 0.0f, 0.f, 0.0f,
	// };

  // pos - nor
  GLfloat vVertices2[] = {
     0.5f,  0.5f, 0.0f, 0.f, 0.0f, 1.f,
    -0.5f, -0.5f, 0.0f, 0.f, 0.0f, 1.f,
     0.5f, -0.5f, 0.0f, 0.f, 0.0f, 1.f,
     0.5f,  0.5f, 0.0f, 0.f, 0.0f, 1.f,
    -0.5f,  0.5f, 0.0f, 0.f, 0.0f, 1.f,
    -0.5f, -0.5f, 0.0f, 0.f, 0.0f, 1.f,
  };

	//enable our shader program
	//glUseProgram(m_programObject2);

	// This will identify our vertex buffer
	static GLuint vertexbuffer = 0;
	if (vertexbuffer == 0)
	{
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexbuffer);
		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices2), vVertices2, GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	float* ptr = 0;
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   6*sizeof(GLfloat),                  // stride
	   (void*)(ptr+0)            // array buffer offset
	);
	//glEnableVertexAttribArray(1);
	// glVertexAttribPointer(
	// 	 1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	// 	 2,                  // size
	// 	 GL_FLOAT,           // type
	// 	 GL_FALSE,           // normalized?
	// 	 5*sizeof(GLfloat),                  // stride
	// 	 (void*)(ptr+3)            // array buffer offset
	// );

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
		 1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
		 3,                  // size
		 GL_FLOAT,           // type
		 GL_FALSE,           // normalized?
		 6*sizeof(GLfloat),        // stride
		 (void*)(ptr+3)            // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

	//draw the triangle
	//glDrawArrays(GL_TRIANGLES, 0, 6);

}
