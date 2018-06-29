#include "InputSystem.h"



InputSystem::InputSystem(RenderSystem* rs) : m_rs(rs)
{
  m_menu = rs->getMenu();
}


InputSystem::~InputSystem()
{


}


void InputSystem::cursor_pos_callback(double xpos, double ypos)
{
  m_cursor_x = xpos;
  m_cursor_y = ypos;


  // Check UI
  // Menu
  if (m_menu->mouseMove(xpos, ypos))
  {
    m_rs->hideToolCursor();
    return;
  }
  else
  {
    if (m_menu->getSelected() != -1)
    {
      m_rs->showToolCursor();
    }
    else
    {
      m_rs->hideToolCursor();
    }
  }

  //

  if (m_clickflags & (1<<GLFW_MOUSE_BUTTON_RIGHT))
  {
    double dx = m_clickstart_x - m_cursor_x;
    double dy = m_clickstart_y - m_cursor_y;
    m_clickstart_x = m_cursor_x;
    m_clickstart_y = m_cursor_y;
    m_rs->move(dx, dy);
  }

  else m_rs->cursor_pos(xpos, ypos);
}


void InputSystem::mouse_button_callback(int button, int action, int mods)
{
  // Menu
  if (m_menu->mouseButton(button, action, mods))
  {
    return;
  }
  else
  {

  }

    //if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
      if (action == GLFW_PRESS && m_clickflags == 0)
      {
          m_clickflags |= (1<<button);
          m_clickstart_x = m_cursor_x;
          m_clickstart_y = m_cursor_y;
      }
      else
      {
          m_clickflags &= ~(1<<button);
      }
    }




    // Tool ?
    if (action == GLFW_PRESS && m_clickflags == (1<<GLFW_MOUSE_BUTTON_LEFT) && m_menu->getSelected() != -1)
    {
      m_rs->performTileAction(m_menu->getSelected()); // J+HACK
    }

}
