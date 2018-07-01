#pragma once

#include "graphics.h"
#include <string>
#include <vector>
#include <map>



class Mesh
{

public:

  static GLuint attribute_v_coord;
  static GLuint attribute_v_uv;

  Mesh();
  ~Mesh();

  void load_obj(const char* filename);
  void upload_obj();

  void draw(GLuint uniformDiffuse = 0);
  void drawWireframe(GLuint uniformDiffuse = 0, int tag = 0);
  void draw_bbox();

  // material mode
  void setColourSceme(int i);

protected:

  void setRenderColour(int i, GLuint uniformDiffuse = 0);


  std::vector<glm::vec4> m_obj_vertices;
  std::vector<glm::vec3> m_obj_normals;
  std::vector<glm::vec2> m_obj_uvs;
  //std::vector<GLushort> m_obj_elements;
  std::map< std::string, std::vector<GLushort> > m_matElements;

  std::string m_active;

  GLuint m_vbo_normals = 0;
  GLuint m_vbo_vertices = 0;
  GLuint m_vbo_uvs = 0;
  //GLuint m_ibo_elements = 0;
  std::map< std::string, GLuint > m_matIboElements;

  std::vector<glm::vec3> m_colours;
};
