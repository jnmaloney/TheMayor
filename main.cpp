#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#endif
#include "graphics.h"


#include "InputSystem.h"
#include "RenderSystem.h"
#include "CityMap.h"
#include "Mesh.h"
#include "RenderQueue.h"
#include "MeshManager.h"
#include "WindowManager.h"
#include "MenuManager.h"
#include "MenuBuild.h"
#include "CityMapRenderer.h"
#include "imgui.h"
#include "ActionManager.h"
#include "Simulation.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


RenderSystem* g_rs = 0;
InputSystem* g_is = 0;
CityMap g_map;
ActionManager g_actionManager(g_map);

WindowManager g_windowManager;
MenuManager g_menuManager;
MenuBuild g_menuBuild;
CityMapRenderer g_mapRenderer;
RenderQueue g_actionRenderQueue;
MeshManager g_meshManager;

//
// Drawing
//
void draw()
{
  if (g_map.ready)
  {
    g_rs->start();

    if (g_menuBuild.m_showOverlay == -1)
      g_mapRenderer.generateQueue(g_menuBuild.m_groundState);
    else
      g_mapRenderer.generateQueue(2 + g_menuBuild.m_showOverlay);

    g_mapRenderer.tick();
    g_mapRenderer.m_renderQueue.draw(g_rs->uniformML, g_rs->uniformDiffuse, g_rs);
    if (g_actionManager.active())
    {
      g_mapRenderer.m_renderQueueAdd.draw(g_rs->uniformML, g_rs->uniformDiffuse, g_rs);
      g_actionRenderQueue.draw(g_rs->uniformML, g_rs->uniformDiffuse, g_rs);
    }
    g_rs->end();
  }
  else
  {
    //glViewport(0, 0, g_windowManager.width, g_windowManager.height);
    glClearColor(238.f / 255.f, 238.f / 255.f, 238.f / 255.f, 1.f); // "eeeeee"
    glClear( GL_COLOR_BUFFER_BIT );

    if (g_menuBuild.m_startGame)
    {
      if (g_menuBuild.m_loadMap)
      {
        g_map.loadFile(g_menuBuild.m_cityName);
      }
      else
      {
        g_map.generate();
        g_map.processTrees();
      }
    }
  }

  // Loadimg Menu
  std::vector< std::string > files;
  //static std::vector< std::string > files2;
  //files.clear();
  //files2.clear();
  if (g_menuBuild.menu_load || !g_map.ready) // Checking for save files.
  {
    CityMap::availableLoadFiles(files);
  }

  // Get ready to load a new map
  if (g_menuBuild.m_clearMap)
  {
    g_menuBuild.m_clearMap = 0;
    g_map.ready = false;
  }


  // ImGui
  g_menuManager.predraw();
  g_menuBuild.draw(files);
  g_menuManager.postdraw(g_windowManager);

  //if (g_menuBuild.menu_load)
  // {
  //   g_menuBuild.menu_load = false;
  //   //g_map.loadFile();
  // }

  if (g_menuBuild.menu_save)
  {
    g_menuBuild.menu_save = false;
    g_map.saveFile(g_menuBuild.m_cityName);
  }

  // Process - HACK
  if (g_menuBuild.menu_process)
  {
    g_menuBuild.menu_process = false;
    g_map.processTrees();
  }

  g_actionManager.setNextAction(g_menuBuild.getNextAction());
  g_actionManager.setCursorPos(
    (int)(g_rs->m_cursorX + 0.5f),
    (int)(g_rs->m_cursorY + 0.5f)
  );

  // Hmmm..
  {
    g_actionRenderQueue.clear();
    int x1, y1, x2, y2;
    if (g_actionManager.m_lineTool.getLine(x1, y1, x2, y2))
    {
      // Render the line-build preview

      if (x1 > x2) std::swap(x1, x2);
      if (y1 > y2) std::swap(y1, y2);

      g_actionRenderQueue.setProgram(g_rs->programObject2);
      g_actionRenderQueue.setMesh(0);

      for (int i = x1; i <= x2; ++i)
      {
        for (int j = y1; j <= y2; ++j)
        {
          g_actionRenderQueue.setMVP(glm::translate(glm::mat4(1.0f), glm::vec3(1.f * i, 1.f * j, 0.2f)));
        }
      }
    }
  }
}


//
// Input
//
void input()
{
  static int timer = 0;
  ++timer;
  if (timer < 60) return;
  timer = 0;

  if (g_map.ready)
  {
    g_map.update();
    g_menuBuild.m_pop = g_map.m_pop;
    if (g_menuBuild.m_tutorialState == 1 && g_map.m_connected > 1) g_menuBuild.m_tutorialState++;
    if (g_menuBuild.m_tutorialState == 2 && g_map.m_watered > 1) g_menuBuild.m_tutorialState++;
    if (g_menuBuild.m_tutorialState == 3 && g_map.m_powered > 1) g_menuBuild.m_tutorialState++;
    if (g_menuBuild.m_tutorialState == 4 && g_map.m_powered_surface) g_menuBuild.m_tutorialState++;
    if (g_menuBuild.m_tutorialState == 5 && g_map.m_comm) g_menuBuild.m_tutorialState++;
    if (g_menuBuild.m_tutorialState == 6 && g_map.m_data_connected) g_menuBuild.m_tutorialState++;
    if (g_menuBuild.m_tutorialState == 7 && g_map.m_pop >= 5000) g_menuBuild.m_tutorialState = 0;

    Simulation::doSkyStep(&g_map);
  }
}


void window_size_callback(GLFWwindow* window, int width, int height)
{
  g_windowManager.width = width;
  g_windowManager.height = height;

  //g_rs->setWindow(width, height);
}


bool m_mouseDrag = false;
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (ImGui::GetIO().WantCaptureMouse) return;

  g_is->cursor_pos_callback(xpos, ypos);
  g_rs->setCursor(xpos, ypos);
  //
  // if (m_mouseDrag)
  // {
  //   g_actionManager.performTileAction();
  // }

  g_actionManager.cursorPos((int)(0.5f + g_rs->m_cursorX), (int)(0.5f + g_rs->m_cursorY));
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (ImGui::GetIO().WantCaptureMouse) return;

  g_is->mouse_button_callback(button, action, mods);

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    //g_actionManager.performTileAction();
    //if (g_actionManager.continueAction()) m_mouseDrag = true;

    g_actionManager.beginActionCommand();
  }
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
  {
    //g_actionManager.performTileAction();
    //if (g_actionManager.continueAction()) m_mouseDrag = true;

    g_actionManager.releaseActionCommand();
  }
  //else m_mouseDrag = false;

  if (button == GLFW_MOUSE_BUTTON_RIGHT)
  {
    //g_actionManager.setNextAction(0);
    g_menuBuild.clearAction();

    g_actionManager.cancelActionCommand();
  }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if (yoffset > 0.0) g_rs->zoom(-1);
  if (yoffset < 0.0) g_rs->zoom( 1);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // if (key == GLFW_KEY_E && action == GLFW_PRESS)
    //     activate_airship();
}


void loop()
{
  // Check for a resize..
  // int h = 0;
  // int w = 0;
  // int f = 0;
  // emscripten_get_canvas_size(&w, &h, &f);
  // if (
  //   w != g_windowManager.width ||
  //   h != g_windowManager.height)
  //   {
  //     glfwSetWindowSize(g_windowManager.g_window, w, h);
  //     return;
  //   }

    //emscripten_set_canvas_size(200, 300);

  //
  // Normal operation:
  //
  //You can read the 'io.WantCaptureMouse', 'io.WantCaptureKeyboard' and 'io.WantTextInput' flags from the ImGuiIO structure (e.g. if (ImGui::GetIO().WantCaptureMouse) { ... } )
  input();
  draw();
}


int init()
{
  // g_windowManager.width = 800;
  // g_windowManager.height = 600;
  g_windowManager.width = 640;
  g_windowManager.height = 480;
  g_windowManager.init("Civille");
  g_menuManager.init(g_windowManager);

  g_rs = new RenderSystem();
  g_rs->m_cameraX = 128.0;
  g_rs->m_cameraY = 128.0;
  g_rs->init(g_windowManager);
  //g_rs->setCityMap(&g_map);

  g_is = new InputSystem(g_rs);
  //g_is->setCityMap(&g_map);

  g_mapRenderer.m_cityMap = &g_map;
  g_mapRenderer.m_rs = g_rs;
  // g_mapRenderer.loadMeshes();
  // g_mapRenderer.generateQueue();
  //g_mapRenderer.loadMeshes();
  g_meshManager.init();
  // g_mapRenderer.generateQueue();
  // g_mapRenderer.tick();

  // Window size callback
  glfwSetWindowSizeCallback(g_windowManager.g_window, window_size_callback);
  glfwSetFramebufferSizeCallback(g_windowManager.g_window, window_size_callback);

  // Cursor callbacks
  glfwSetCursorPosCallback(g_windowManager.g_window, cursor_pos_callback);
  glfwSetMouseButtonCallback(g_windowManager.g_window, mouse_button_callback);
  glfwSetScrollCallback(g_windowManager.g_window, scroll_callback);

  // Key callbacks
  //glfwSetKeyCallback(g_windowManager.g_window, key_callback);
  glfwSetKeyCallback(g_windowManager.g_window, ImGui_ImplGlfw_KeyCallback);
  glfwSetCharCallback(g_windowManager.g_window, ImGui_ImplGlfw_CharCallback);

  // TODO Touch callbacks


  // HACK Render fixed loading
  // g_meshGenerator.load_obj("data/Generator.obj");
  // g_meshGenerator.upload_obj();

  // g_meshTree.load_obj("data/lightmap/tree_lightmap.obj");
  // g_meshTree.upload_obj();
  //
  // g_meshHouse.load_obj("data/lightmap/house1_lightmap.obj");
  // g_meshHouse.upload_obj();
  //
  // g_renderQueue.setProgram(g_rs->programObject);
  //
  // // g_renderQueue.setMesh(&g_meshGenerator);
  // // g_renderQueue.setMVP(glm::mat4(1.0f));
  // // g_renderQueue.submit();
  // //
  // // g_renderQueue.setMVP(glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 1.f, 0.f)));
  // // g_renderQueue.submit();
  //
  // g_renderQueue.setMesh(&g_meshTree);
  // g_renderQueue.setMVP(glm::translate(glm::mat4(1.0f), glm::vec3(1.f, 0.f, 0.f)));
  //
  // g_renderQueue.setMesh(&g_meshHouse);
  // g_renderQueue.setMVP(glm::translate(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 0.f)));
  //
  //
  // static Texture g_texture;
  // static Texture g_texture2;
  //
  // g_texture.loadPng("data/lightmap/lightmap.png");
  // g_texture2.loadPng("data/lightmap/lightmap_house1.png");
  //
  // g_meshTree.texture = &g_texture;
  // g_meshHouse.texture = &g_texture2;


  return 0;
}


void quit()
{
  glfwTerminate();
}


extern "C" int main(int argc, char** argv)
{
  EM_ASM(
    FS.mkdir('/working1');
    FS.mount(IDBFS, {}, '/working1');

    // sync from persisted state into memory and then
    // run the 'test' function
    FS.syncfs(true, function (err) {
      //std::cout << "Sync Error : " << err << std::endl;
      //assert(!err);
      //ccall('test', 'v');
    });
  );

  if (init() != 0) return 1;

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
  #endif

  quit();

  return 0;
}
