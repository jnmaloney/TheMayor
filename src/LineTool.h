#pragma once


class LineTool
{
public:

  LineTool();
  ~LineTool();

  void reset();

  void setPoint(int x, int y);

  bool getLine(int& x1, int& y1, int& x2, int& y2);

protected:

  bool m_startSet;
  int startX;
  int startY;
  bool m_endSet;
  int endX;
  int endY;
};
