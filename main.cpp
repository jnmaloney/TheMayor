#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "graphics.h"


#include "InputSystem.h"
#include "RenderSystem.h"
#include "CityMap.h"


GLFWwindow* g_window;
RenderSystem* g_rs = 0;
InputSystem* g_is = 0;
CityMap g_map;


void draw()
{
  //
  // Drawing
  //
  g_rs->draw();
}


void input()
{
  //
  // Input
  //
  g_is->frame();
}


void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
  g_is->cursor_pos_callback(xpos, ypos);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    g_is->mouse_button_callback(button, action, mods);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if (yoffset > 0.0) g_rs->zoom(-1);
  if (yoffset < 0.0) g_rs->zoom( 1);
}


void loop()
{
  input();
  draw();
}


int init()
{
  if( !glfwInit() )
  {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      return 1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  // Open a window and create its OpenGL context
  g_window = glfwCreateWindow( 1280, 720, "The Mayor", NULL, NULL);
  if( g_window == NULL )
  {
      fprintf( stderr, "Failed to open GLFW window.\n" );
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(g_window); // Initialize GLEW

  // Create game objects
  RenderSystem* rs = new RenderSystem();
  g_rs = rs;
  g_rs->setCityMap(&g_map);

  g_is = new InputSystem(g_rs);
  g_is->setCityMap(&g_map);

  g_map.generate();

  // Cursor callbacks
  glfwSetCursorPosCallback(g_window, cursor_pos_callback);
  glfwSetMouseButtonCallback(g_window, mouse_button_callback);
  glfwSetScrollCallback(g_window, scroll_callback);

  return 0;
}


void quit()
{
  glfwTerminate();
}


extern "C" int main(int argc, char** argv)
{
  if (init() != 0) return 1;

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
  #endif

  quit();

  return 0;
}
