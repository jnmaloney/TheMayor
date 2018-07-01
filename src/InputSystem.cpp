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

  // Action
  if (m_allowDrag)
  {
    m_allowDrag = m_rs->performTileAction(m_menu->getSelected());
  }
}


void InputSystem::mouse_button_callback(int button, int action, int mods)
{
  // Button released
  if (action == GLFW_RELEASE) m_allowDrag = false;

  if (m_allowDrag) return;

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
      m_allowDrag = m_rs->performTileAction(m_menu->getSelected()); // J+HACK
    }

}


void InputSystem::frame()
{
  m_menu->setPop(m_cityMap->getPop());

  if (m_menu->m_message == 1 && m_cityMap->nHouseRoads > 1) { m_menu->advance(); }
  if (m_menu->m_message == 2 && m_cityMap->nHouseWatered > 1) { m_menu->advance(); }
  if (m_menu->m_message == 3 && m_cityMap->nHousePowered > 1) { m_menu->advance(); }
  if (m_menu->m_message == 4 && m_cityMap->nHasJobs == 1) { m_menu->advance(); }
  if (m_menu->m_message == 5 && m_cityMap->nHouseData > 1) { m_menu->advance(); }
  if (m_menu->m_message == 6 && m_cityMap->getPop() > 1e5) { m_menu->advance(); }
}
