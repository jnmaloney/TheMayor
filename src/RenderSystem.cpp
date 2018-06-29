#include <RenderSystem.h>
#include "shaders.h"
#include "SolidSphere.h"
#include "Mesh.h"
#include <iostream>
#include "Menu.h"


// Uniforms OLD
GLint uniformDiffuse;
GLint uniformMVP;
GLint uniform_s_GBuffer;
GLint lightPos, lightCol;


// Mesh
std::shared_ptr<Mesh> pMesh(new Mesh());
std::shared_ptr<Mesh> pMesh2(new Mesh());
std::shared_ptr<Mesh> pMeshGas(new Mesh());


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

    //pMesh->load_obj("data/suburb_assets_pt1/models/house-country-edit.obj");
    pMesh->load_obj("data/lowpoly_tree/obj/untitled.obj");
    //pMesh->load_obj("data/lowpoly_tree/obj/tree_snow.obj");
    pMesh->upload_obj();
    pMesh->setColourSceme(2);

    pMesh2->load_obj("data/HousesPackLowPoly_house1.obj");
    //pMesh2->load_obj("data/suburb_assets_pt1/models/house-country-edit.obj");
    //pMesh2->load_obj("data/3D Road Pack/Models/roadTile_160-edit.obj");
    //pMesh2->load_obj("data/lowpoly_tree/obj/untitled.obj");
    pMesh2->upload_obj();
    pMesh2->setColourSceme(4);

    //pMeshGas->load_obj("data/suburb_assets_pt1/models/gas-station-edit.obj");
    //pMeshGas->load_obj("data/suburb_assets_pt1/models/gas-station.obj");
    //pMeshGas->load_obj("data/3D Road Pack/Models/roadTile_150-edit.obj");
    //pMeshGas->load_obj("data/suburb_assets_pt1/models/house-luxurious-edit.obj");
    pMeshGas->load_obj("data/EngineHouse.obj");
    pMeshGas->upload_obj();
    pMeshGas->setColourSceme(3);


    std::vector<const char*> house_files;
    house_files.push_back("data/HousesPackLowPoly_house1.obj");
    house_files.push_back("data/HousesPackLowPoly_house2.obj");
    house_files.push_back("data/HousesPackLowPoly_house3.obj");
    house_files.push_back("data/HousesPackLowPoly_house4.obj");
    house_files.push_back("data/HousesPackLowPoly_house5.obj");
    for (int i = 0; i < house_files.size(); ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(house_files[i]);
      pMeshLoad->upload_obj();
      pMeshLoad->setColourSceme(5 + i);
      house_meshes.push_back(pMeshLoad);
    }

    std::vector<const char*> road_files;

    //road_files.push_back("data/3D Road Pack/Models/roadTile_160-edit.obj");
    road_files.push_back("data/3D Road Pack/Models/roadTile_277-edit2.obj");
    road_files.push_back("data/3D Road Pack/Models/roadTile_299-edit2.obj");//corner
    //road_files.push_back("data/3D Road Pack/Models/roadTile_150-edit.obj");//.
    road_files.push_back("data/3D Road Pack/Models/roadTile_148-edit2.obj");//.
    road_files.push_back("data/3D Road Pack/Models/roadTile_260-edit2.obj");
    //road_files.push_back("data/3D Road Pack/Models/roadTile_260-edit.obj");//.
    //road_files.push_back("data/3D Road Pack/Models/roadTile_178-edit.obj");
    //road_files.push_back("data/3D Road Pack/Models/roadTile_190-edit.obj");
    //road_files.push_back("data/3D Road Pack/Models/roadTile_215-edit.obj");
    // 287 3-way
    for (int i = 0; i < road_files.size(); ++i)
    {
      Mesh* pMeshLoad = new Mesh();
      pMeshLoad->load_obj(road_files[i]);
      pMeshLoad->upload_obj();
      pMeshLoad->setColourSceme(1);
      road_meshes.push_back(pMeshLoad);
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
  float colour0[] = { 123.f / 255.f, 179.f / 255.f,  105.f / 255.f }; // Grass Green
  float colour1[] = { 188.f / 255.f, 188.f / 255.f,  100.f / 255.f }; // Yellow

  if (tile.object_type == 2 && tile.treeAng1 == 0) glUniform3fv(uniformDiffuse, 1, colour1);
  else glUniform3fv(uniformDiffuse, 1, colour0);

  drawObjModel_square();

  // Objects
  if (tile.object_type == 1)
  {
    glm::mat4 sublocal = glm::scale(local, glm::vec3(0.25f));

    glm::mat4 sublocal2 = glm::translate(sublocal, glm::vec3(1.0f, 1.0f, 0.0f));
    if (tile.treeAng1)
    {
      sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng1), glm::vec3(0.f, 0.f, 1.f));
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal2[0][0]);
      pMesh->draw(uniformDiffuse);
    }

    sublocal2 = glm::translate(sublocal, glm::vec3(1.0f, -1.0f, 0.0f));
    if (tile.treeAng2)
    {
      sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng2), glm::vec3(0.f, 0.f, 1.f));
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal2[0][0]);
      pMesh->draw(uniformDiffuse);
    }

    sublocal2 = glm::translate(sublocal, glm::vec3(-1.0f, 1.0f, 0.0f));
    if (tile.treeAng3)
    {
      sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng3), glm::vec3(0.f, 0.f, 1.f));
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal2[0][0]);
      pMesh->draw(uniformDiffuse);
    }

    sublocal2 = glm::translate(sublocal, glm::vec3(-1.0f, -1.0f, 0.0f));
    if (tile.treeAng4)
    {
      sublocal2 = glm::rotate(sublocal2, (float)(0.0015f * tile.treeAng4), glm::vec3(0.f, 0.f, 1.f));
      glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &sublocal2[0][0]);
      pMesh->draw(uniformDiffuse);
    }
  }
  if (tile.object_type == 2)
  {
    glm::mat4 local = glm::rotate(MVP, 0.f, glm::vec3(0.f, 0.f, 1.f));
    local = glm::scale(local, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

    int i = tile.treeAng1 % 6;
    if (i > 0)
    {
      house_meshes[i - 1]->draw(uniformDiffuse);
    }
  }
  if (tile.object_type == 3)
  {
    glm::mat4 local = glm::rotate(MVP, -1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);

    pMeshGas->draw(uniformDiffuse);
  }

  if (tile.road > 0)
  {
    glm::mat4 local = glm::rotate(MVP, (float)tile.roadAng * 1.57079632679f, glm::vec3(0.f, 0.f, 1.f));
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &local[0][0]);
    road_meshes[tile.roadNum]->draw(uniformDiffuse);
  }
}


void RenderSystem::move(double x, double y)
{
   m_cameraX += 0.05 * -x / m_zoom + 0.05 * y / m_zoom;
   m_cameraY += 0.05 *  x / m_zoom + 0.05 * y / m_zoom;
}


void RenderSystem::zoom(int i)
{
  m_zoomFactor += i;
  if (m_zoomFactor < 0) m_zoomFactor = 0;
  if (m_zoomFactor > 7) m_zoomFactor = 7;

  m_zoom = (1<<m_zoomFactor);
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
    counter = 0;
  }
}


static int ci = 0;
static int cj = 0;
void RenderSystem::performTileAction(int i)
{
  if (m_cityMap == 0) return;

  // Get the data
  CityTile& tile = m_cityMap->getTile(ci, cj);
  if (tile.isDefault) return;

  //tile.object_type = (tile.object_type + 1) % 4;
  //tile.road = (tile.road + 1) % (road_meshes.size() + 1);

  // Road !
  if (i == 0)
  {
    if (tile.object_type == 1)
    {
      tile.object_type = 0;
    }
    if (tile.object_type != 0) return;

    //if (tile.road) tile.road = 0;
    //else
    tile.road = 1;

    checkRoadTile(ci, cj);
    checkRoadTile(ci, cj+1);
    checkRoadTile(ci, cj-1);
    checkRoadTile(ci-1, cj);
    checkRoadTile(ci+1, cj);
  }

  // Res zone
  if (i == 1)
  {
    if (tile.object_type == 1)
    {
      tile.object_type = 0;
    }
    if (tile.road || tile.object_type) return;
    tile.object_type = 2;
    tile.treeAng1 = 0;
    tile.treeAng2 = 0;
  }

  // Power Zone
  if (i == 2)
  {
    if (tile.object_type == 1)
    {
      tile.object_type = 0;
    }
    if (tile.road || tile.object_type) return;
    tile.object_type = 3;
  }

  // Park Zone
  if (i == 3)
  {
    if (tile.road || tile.object_type) return;
    tile.object_type = 1;
    tile.treeAng1 = 1;
    tile.treeAng2 = 1;
    tile.treeAng3 = 1;
    tile.treeAng4 = 1;
  }

  // Bulldoze
  if (i == 4)
  {
    tile.object_type = 0;
    tile.road = 0;

    checkRoadTile(ci, cj+1);
    checkRoadTile(ci, cj-1);
    checkRoadTile(ci-1, cj);
    checkRoadTile(ci+1, cj);
  }
}


void RenderSystem::checkRoadTile(int i, int j)
{
  CityTile& t = m_cityMap->getTile(i, j);
  if (t.road == 0) return;

  CityTile& t_n = m_cityMap->getTile(i, j - 1);
  CityTile& t_s = m_cityMap->getTile(i, j + 1);
  CityTile& t_e = m_cityMap->getTile(i + 1, j);
  CityTile& t_w = m_cityMap->getTile(i - 1, j);

  int num = 0;
  if (t_n.road) num += 1;
  if (t_s.road) num += 1;
  if (t_e.road) num += 1;
  if (t_w.road) num += 1;

  int x = 0;
  if (t_n.road) x += 1;
  if (t_s.road) x += 2;
  if (t_e.road) x += 4;
  if (t_w.road) x += 8;


  t.roadNum = num;


  // ..nsew
  if (x == 0b0000)
  {
    t.roadNum = 0;
    t.roadAng = 0;
  }
  else if (x == 0b0001)
  {
    t.roadNum = 0;
    t.roadAng = 1;
  }
  else if (x == 0b0010)
  {
    t.roadNum = 0;
    t.roadAng = 1;
  }
  else if (x == 0b0011)
  {
    t.roadNum = 0;
    t.roadAng = 1;
  }
  else if (x == 0b0100)
  {
    t.roadNum = 0;
    t.roadAng = 0;
  }
  else if (x == 0b0101)
  {
    t.roadNum = 1;
    t.roadAng = 0;
  }
  else if (x == 0b0110)
  {
    t.roadNum = 1;
    t.roadAng = 1;
  }
  else if (x == 0b0111)
  {
    t.roadNum = 2;
    t.roadAng = 0;
  }
  else if (x == 0b1000)
  {
    t.roadNum = 0;
    t.roadAng = 0;
  }
  else if (x == 0b1001)
  {
    t.roadNum = 1;
    t.roadAng = 3;
  }
  else if (x == 0b1010)
  {
    t.roadNum = 1;
    t.roadAng = 2;
  }
  else if (x == 0b1011)
  {
    t.roadNum = 2;
    t.roadAng = 2;
  }
  else if (x == 0b1100)
  {
    t.roadNum = 0;
    t.roadAng = 0;
  }
  else if (x == 0b1101)
  {
    t.roadNum = 2;
    t.roadAng = 3;
  }
  else if (x == 0b1110)
  {
    t.roadNum = 2;
    t.roadAng = 1;
  }
  else if (x == 0b1111)
  {
    t.roadNum = 3;
    t.roadAng = 0;
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

      if ((dx + dy)*m_zoom > -88 && (dx + dy)*m_zoom < 88 &&
          (dx - dy)*m_zoom > -99 && (dx - dy)*m_zoom < 99 )
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

    pMesh->draw_bbox();
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
