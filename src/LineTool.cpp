#include "LineTool.h"
#include <stdlib.h>


LineTool::LineTool()
{
  reset();
}


LineTool::~LineTool()
{

}

void LineTool::reset()
{
  m_startSet = false;
  m_endSet = false;
}


void LineTool::setPoint(int x, int y)
{
  if (m_startSet == false)
  {
    startX = x;
    startY = y;
    m_startSet = true;
  }
  //else
  {
    if (abs(x - startX) < abs(y - startY))
    {
      endX = startX;
      endY = y;
    }
    else
    {
      endX = x;
      endY = startY;
    }
    m_endSet = true;
  }
}


bool LineTool::getLine(int& x1, int& y1, int& x2, int& y2)
{
  if (m_endSet)
  {
    x1 = startX;
    y1 = startY;
    x2 = endX;
    y2 = endY;
  }

  else if (m_startSet)
  {
    x1 = startX;
    y1 = startY;
    x2 = startX;
    y2 = startY;
  }

  else return false;

  return true;
}
