#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>


template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


//GLuint Mesh::attribute_v_normal = -1;
GLuint Mesh::attribute_v_coord = -1;
GLuint Mesh::attribute_v_uv = -1;

Mesh::Mesh()
{

}


Mesh::~Mesh()
{

}


void Mesh::load_obj(const char* filename)
{
  using namespace std;

  ifstream in(filename, ios::in);
  if (!in)
  {
      cout << "Cannot open " << filename << endl;
      cerr << "Cannot open " << filename << endl; exit(1);
  }

  cout << "opened " << filename << endl;

  std::vector<glm::vec3> load_normals;
  std::vector<glm::vec2> load_uvs;

  string line;
  while (getline(in, line))
  {
      //cout << line << endl;
      if (line.substr(0,2) == "v ")
      {
          istringstream s(line.substr(2));
          glm::vec4 v;
          s >> v.x;
          s >> v.z;
          s >> v.y;
          v.w = 1.0f;
          v.x = v.x / 3.0;  // Load-time scaling
          v.y = v.y / 3.0;
          v.z = v.z / 3.0;
          m_obj_vertices.push_back(0.02f * v);
      }
      else if (line.substr(0,2) == "vn") // normals
      {
        istringstream s(line.substr(2));
        glm::vec3 v;
        s >> v.x;
        s >> v.z;
        s >> v.y;
        load_normals.push_back(v);
      }
      else if (line.substr(0,2) == "vt") // uvs
      {
        istringstream s(line.substr(2));
        glm::vec2 v;
        s >> v.x;
        s >> v.y;
        load_uvs.push_back(v);
      }
      else if (line.substr(0,2) == "f ")
      {
          istringstream s(line.substr(2));

          // Faces with normals
          std::string but;
          for (int i = 0; i < 3; ++i) // HACK Force tri
          {
            s >> but;
            //cout << but << endl;
            std::vector<std::string> sp = split(but, '/');

            if (sp.size() >= 1)
            {
              GLushort a;
              a = std::stoi( sp[0] ) - 1;
              //m_obj_elements.push_back(a);
              m_matElements[m_active].push_back(a);
            }

            // a/B/c
            if (sp.size() >= 2)
            {
              if (m_obj_uvs.size() == 0) m_obj_uvs.resize(m_obj_vertices.size(), glm::vec3(0.0, 0.0, 0.0));

              unsigned int a = 0;
              unsigned int b = -1;

              if (sp[0].length()) a = std::stoi( sp[0] ) - 1;
              if (sp[1].length()) b = std::stoi( sp[1] ) - 1;

              if (b < (unsigned int)-1)
                m_obj_uvs[a] = load_uvs[b];
            }

            // a/b/C
            if (sp.size() >= 3)
            {
              if (m_obj_normals.size() == 0) m_obj_normals.resize(m_obj_vertices.size(), glm::vec3(0.0, 0.0, 0.0));

              unsigned int a = std::stoi( sp[0] ) - 1;
              unsigned int b = std::stoi( sp[2] ) - 1;
              m_obj_normals[a] = load_normals[b];
            }
          }
      }
      else if (line.substr(0,2) == "o ")
      {
        load_normals.resize(0);
        load_uvs.resize(0);
      }
      else if (line.substr(0,7) == "usemtl ")
      {
          istringstream s(line.substr(7));
          s >> m_active;
          cout << m_active << endl;
      }
      // else if (line.substr(0,2) == "s ")
      // {
      //     istringstream s(line.substr(2));
      //     s >> m_active;
      //     cout << m_active << endl;
      // }
      else if (line[0] == '#')
      {

      }
      else
      {
          /* ignoring this line */
      }
  }

  cout << "loaded " << m_obj_vertices.size() << " vertices." << endl;
  //cout << "loaded " << m_obj_elements.size() << " elements." << endl;

  // Generate normals
  // if (m_obj_normals.size() == 0)
  // {
  //   m_obj_normals.resize(m_obj_vertices.size(), glm::vec3(0.0, 0.0, 0.0));
  //   for (int i = 0; i < m_matElements[m_active].size(); i+=3)
  //   {
  //       GLushort ia = m_matElements[m_active][i];
  //       GLushort ib = m_matElements[m_active][i+1];
  //       GLushort ic = m_matElements[m_active][i+2];
  //       //glm::vec3 normal(0.0f, 0.0f, 1.0f);
  //       glm::vec3 normal = glm::normalize(glm::cross(
  //         glm::vec3(m_obj_vertices[ib]) - glm::vec3(m_obj_vertices[ia]),
  //         glm::vec3(m_obj_vertices[ic]) - glm::vec3(m_obj_vertices[ia])));
  //       if (normal.length() < 1.0f) normal = glm::vec3(0.0f, 0.0f, 1.0f);
  //       m_obj_normals[ia] = m_obj_normals[ib] = m_obj_normals[ic] = normal;
  //   }
  //
  //   cout << "generated " << m_obj_normals.size() << " normals." << endl;
  // }
  // else cout << "using loaded normals." << endl;

  if (m_obj_uvs.size() == 0)
  {
    m_obj_uvs.resize(m_obj_vertices.size(), glm::vec3(0.0, 0.0, 0.0));
  }

}




void Mesh::upload_obj()
{

  //load_obj("data/lowpoly_tree/obj/untitled.obj");

  //
  // Upload
  //
  if (m_obj_vertices.size() > 0)
  {
    glGenBuffers(1, &m_vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, m_obj_vertices.size() * sizeof(m_obj_vertices[0]),
      m_obj_vertices.data(), GL_STATIC_DRAW);
  } else std::cout << "no upload vert" << std::endl;

  if (m_obj_uvs.size() > 0)
  {
    glGenBuffers(1, &m_vbo_uvs);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_uvs);
    glBufferData(GL_ARRAY_BUFFER, m_obj_uvs.size() * sizeof(m_obj_uvs[0]),
      m_obj_uvs.data(), GL_STATIC_DRAW);
  } else std::cout << "no upload uvs" << std::endl;

  // if (m_obj_normals.size() > 0)
  // {
  //   glGenBuffers(1, &m_vbo_normals);
  //   glBindBuffer(GL_ARRAY_BUFFER, m_vbo_normals);
  //   glBufferData(GL_ARRAY_BUFFER, m_obj_normals.size() * sizeof(m_obj_normals[0]),
  //     m_obj_normals.data(), GL_STATIC_DRAW);
  // } else std::cout << "no upload nor" << std::endl;

  for (auto i = m_matElements.begin(); i != m_matElements.end(); ++i)
  {
    if ((*i).second.size() > 0)
    {
      GLuint ibo_elements;
      glGenBuffers(1, &ibo_elements);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,(*i).second.size() * sizeof((*i).second[0]),
        (*i).second.data(), GL_STATIC_DRAW); // elements (faces)

      m_matIboElements[(*i).first] = ibo_elements;

    } // else std::cout << "no upload ele" << std::endl;
  }
}


void Mesh::draw(GLuint uniformDiffuse)
{
  if (m_matElements[m_active].size() == 0) return;

  //
  // Draw
  //


  glEnableVertexAttribArray(attribute_v_coord);
  // Describe our vertices array to OpenGL (it can't guess its format automatically)
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertices);
  glVertexAttribPointer(
    attribute_v_coord,  // attribute
    4,                  // number of elements per vertex, here (x,y,z,w)
    GL_FLOAT,           // the type of each element
    GL_FALSE,           // take our values as-is
    0,                  // no extra data between each position
    0                   // offset of first element
  );

  glEnableVertexAttribArray(attribute_v_uv);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_uvs);
  glVertexAttribPointer(
    attribute_v_uv, // attribute
    2,                  // number of elements per vertex, here (x,y,z)
    GL_FLOAT,           // the type of each element
    GL_FALSE,           // take our values as-is
    0,                  // no extra data between each position
    0                   // offset of first element
  );

  // glEnableVertexAttribArray(attribute_v_normal);
  // glBindBuffer(GL_ARRAY_BUFFER, m_vbo_normals);
  // glVertexAttribPointer(
  //   attribute_v_normal, // attribute
  //   3,                  // number of elements per vertex, here (x,y,z)
  //   GL_FLOAT,           // the type of each element
  //   GL_FALSE,           // take our values as-is
  //   0,                  // no extra data between each position
  //   0                   // offset of first element
  // );

  // if (m_ibo_elements == 0)
  // {
  //   glDrawArrays(GL_TRIANGLES, 0, m_obj_vertices.size());
  // }
  //else
  // {
  //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_elements);
  //  int size = m_matElements[m_active].size();
  //  //glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  //  //std::cout << size << std::endl;
  //  //glDrawElements(GL_POINTS, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
  //  glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, 0);
  //  //glDrawElements(GL_POINTS, size, GL_UNSIGNED_SHORT, 0);
  // }


  // //float colour0[] = { 58.f / 255.f, 121.f / 255.f,  94.f / 255.f }; // pine
  // float colour0[] = { 88.f / 255.f, 91.f / 255.f,  94.f / 255.f }; // paved
  // //float colour1[] = { 0.f / 255.f, 179.f / 255.f,  105.f / 255.f }; // Asph?
  // float colour1[] = { 160.f / 255.f, 169.f / 255.f,  155.f / 255.f }; // Asph?
  // float colour2[] = { 123.f / 255.f, 0.f / 255.f,  105.f / 255.f }; // Grass Green
  // float colour3[] = { 123.f / 255.f, 179.f / 255.f,  0.f / 255.f }; // Grass Green
  // float colour4[] = {1.f, 1.f, 1.f }; // Grass Green
  //
  int c = 0;
  for (auto i = m_matIboElements.begin(); i != m_matIboElements.end(); ++i)
  {
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*i).second);
     int size = m_matElements[(*i).first].size();
     //int size;
     //glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
     //std::cout << size << std::endl;
     //glDrawElements(GL_POINTS, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

          // if (c%5==0) glUniform3fv(uniformDiffuse, 1, colour0);
          // if (c%5==1) glUniform3fv(uniformDiffuse, 1, colour1);
          // if (c%5==2) glUniform3fv(uniformDiffuse, 1, colour2);
          // if (c%5==3) glUniform3fv(uniformDiffuse, 1, colour3);
          // if (c%5==4) glUniform3fv(uniformDiffuse, 1, colour4);

      setRenderColour(c++, uniformDiffuse);

     glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, 0);
     //glDrawElements(GL_POINTS, size, GL_UNSIGNED_SHORT, 0);

  }

  glDisableVertexAttribArray(attribute_v_coord);
  //glDisableVertexAttribArray(1);


  // glEnableVertexAttribArray(0);
  // glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertices);
  // GLushort* ptr = 0;
  // glVertexAttribPointer(
  //    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
  //    4,                  // size
  //    GL_FLOAT,           // type
  //    GL_FALSE,           // normalized?
  //    7*sizeof(GLushort),                  // stride
  //    (void*)(ptr+0)            // array buffer offset
  // );
  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(
  //    1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
  //    3,                  // size
  //    GL_FLOAT,           // type
  //    GL_FALSE,           // normalized?
  //    7*sizeof(GLushort),                  // stride
  //    (void*)(ptr+4)            // array buffer offset
  // );
  // // Draw the triangle !
  // glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
  // //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_elements);
  // //glDrawElements(GL_TRIANGLES, 696/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
  // glDisableVertexAttribArray(0);
  // glDisableVertexAttribArray(1);
}


static glm::vec3 desaturate(glm::vec3 input)
{
  glm::vec3 output = glm::vec3(1.0);
  output[0] = input[0] * 0.5 + 0.4;
  output[1] = input[1] * 0.5 + 0.4;
  output[2] = input[2] * 0.5 + 0.4;
  return output;
}

void Mesh::setColourSceme(int i)
{
  if (i == 1)
  {
    m_colours.resize(0);
    m_colours.push_back(glm::vec3(88.f / 255.f, 91.f / 255.f,  94.f / 255.f));
    m_colours.push_back(glm::vec3(160.f / 255.f, 169.f / 255.f,  155.f / 255.f));
  }
  if (i == 2)
  {
    m_colours.resize(0);
    m_colours.push_back(glm::vec3(58.f / 255.f, 121.f / 255.f,  94.f / 255.f));
  }
  if (i == 3)
  {
    // Redbrick Station
    m_colours.resize(0);
    m_colours.push_back(glm::vec3(140.f/255.f, 94.f/255.f, 76.f/255.f));
    m_colours.push_back(glm::vec3(216.f/255.f, 209.f/255.f, 157.f/255.f));
    m_colours.push_back(glm::vec3(124.f/255.f, 75.f/255.f, 61.f/255.f));
    m_colours.push_back(glm::vec3(216.f/255.f, 209.f/255.f, 157.f/255.f));
    // m_colours.push_back(glm::vec3(139.f/255.f, 180.f/255.f, 74.f/255.f));
    // m_colours.push_back(glm::vec3(205.f/255.f, 202.f/255.f, 193.f/255.f));
  }
  if (i == 4)
  {
    m_colours.resize(0);
    m_colours.push_back(glm::vec3(111.f/255.f, 192.f/255.f, 211.f/255.f));
    m_colours.push_back(glm::vec3(158.f/255.f, 119.f/255.f, 88.f/255.f));
    m_colours.push_back(glm::vec3(216.f/255.f, 209.f/255.f, 157.f/255.f));
    m_colours.push_back(glm::vec3(106.f/255.f, 106.f/255.f, 106.f/255.f));
    m_colours.push_back(glm::vec3(139.f/255.f, 180.f/255.f, 74.f/255.f));
    m_colours.push_back(glm::vec3(205.f/255.f, 202.f/255.f, 193.f/255.f));
  }
  if (i == 5)
  {
    // house 1
    m_colours.resize(0);

    m_colours.push_back(desaturate(glm::vec3(1.f, 0.557f, 0.369f)));
    m_colours.push_back(desaturate(glm::vec3(0.027f, 0.027f, 0.027f)));
    m_colours.push_back(desaturate(glm::vec3(0.723f, 0.723f, 0.723f)));
    m_colours.push_back(desaturate(glm::vec3(0.319f, 0.181f, 0.122f)));
    m_colours.push_back(desaturate(glm::vec3(0.059f, 0.607f, 0.800f))); // glass
  }
  if (i == 6)
  {
    // house 2
    m_colours.resize(0);

    m_colours.push_back(desaturate(glm::vec3(0.800f, 0.644f, 0.369f)));
    m_colours.push_back(desaturate(glm::vec3(0.723f, 0.723f, 0.723f))); // glass
    m_colours.push_back(desaturate(glm::vec3(0.027f, 0.027f, 0.027f)));
    m_colours.push_back(desaturate(glm::vec3(0.560f, 0.226f, 0.027f))); // door
  }
  if (i == 7)
  {
    // house 3
    m_colours.resize(0);

    m_colours.push_back(desaturate(glm::vec3(1.f, 0.862f, 0.575f)));
    m_colours.push_back(desaturate(glm::vec3(1.f, 0.932f, 0.817f)));
    m_colours.push_back(desaturate(glm::vec3(0.027f, 0.027f, 0.027f)));
    m_colours.push_back(desaturate(glm::vec3(0.195f, 0.757f, 1.f))); // glass
    m_colours.push_back(desaturate(glm::vec3(1.0f, 0.194f, 0.108f)));
  }
  if (i == 8)
  {
    // house 4
    m_colours.resize(0);

    m_colours.push_back(desaturate(glm::vec3(1.f, 0.376f, 0.161f)));
    m_colours.push_back(desaturate(glm::vec3(0.800f, 0.800f, 0.800f)));
    m_colours.push_back(desaturate(glm::vec3(0.240f, 0.393f, 0.062f)));
    m_colours.push_back(desaturate(glm::vec3(0.376f, 0.100f, 0.013f)));
    m_colours.push_back(desaturate(glm::vec3(0.050f, 0.426f, 1.f))); // glass
  }
  if (i == 9)
  {
    // house 5
    m_colours.resize(0);

    m_colours.push_back(desaturate(glm::vec3(0.376f, 0.100f, 0.013f)));
    m_colours.push_back(desaturate(glm::vec3(1.0f, 0.301f, 0.071f)));
    m_colours.push_back(desaturate(glm::vec3(0.059f, 0.607f, 0.800f))); // glass
    m_colours.push_back(desaturate(glm::vec3(1.0f, 0.480f, 0.037f)));
    m_colours.push_back(desaturate(glm::vec3(0.800f, 0.070f, 0.037f)));
  }
}


void Mesh::setRenderColour(int i, GLuint uniformDiffuse)
{
  float colour[] = {1.f, 1.f, 1.f };

  if (m_colours.size())
  {
    glm::vec3 col = m_colours[i % m_colours.size()];
    colour[0] = col[0];
    colour[1] = col[1];
    colour[2] = col[2];
  }

  glUniform3fv(uniformDiffuse, 1, colour);
}


void Mesh::draw_bbox()
{
  if (m_obj_vertices.size() == 0)
  {
    std::cout << "No size" << std::endl;
    return;
  }

  // Cube 1x1x1, centered on origin
  GLfloat vertices[] = {
    -0.5, -0.5, -0.5, 1.0,
     0.5, -0.5, -0.5, 1.0,
     0.5,  0.5, -0.5, 1.0,
    -0.5,  0.5, -0.5, 1.0,
    -0.5, -0.5,  0.5, 1.0,
     0.5, -0.5,  0.5, 1.0,
     0.5,  0.5,  0.5, 1.0,
    -0.5,  0.5,  0.5, 1.0,
  };
  GLuint vbo_vertices;
  glGenBuffers(1, &vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLushort elements[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    0, 4, 1, 5, 2, 6, 3, 7
  };
  GLuint ibo_elements;
  glGenBuffers(1, &ibo_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLfloat
    min_x, max_x,
    min_y, max_y,
    min_z, max_z;
  min_x = max_x = m_obj_vertices[0].x + 2500;
  min_y = max_y = m_obj_vertices[0].y;
  min_z = max_z = m_obj_vertices[0].z;
  for (unsigned int i = 0; i < m_obj_vertices.size(); i++) {
    if (m_obj_vertices[i].x < min_x) min_x = m_obj_vertices[i].x;
    if (m_obj_vertices[i].x > max_x) max_x = m_obj_vertices[i].x;
    if (m_obj_vertices[i].y < min_y) min_y = m_obj_vertices[i].y;
    if (m_obj_vertices[i].y > max_y) max_y = m_obj_vertices[i].y;
    if (m_obj_vertices[i].z < min_z) min_z = m_obj_vertices[i].z;
    if (m_obj_vertices[i].z > max_z) max_z = m_obj_vertices[i].z;
  }
  glm::vec3 size = glm::vec3(max_x-min_x, max_y-min_y, max_z-min_z);
  glm::vec3 center = glm::vec3((min_x+max_x)/2, (min_y+max_y)/2, (min_z+max_z)/2);
  glm::mat4 transform = glm::scale(glm::mat4(1), size) * glm::translate(glm::mat4(1), center);

  /* Apply object's transformation matrix */
  // glm::mat4 m = glm::mat4(1.0f); // this->object2world * transform;
  // glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(m));

  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glEnableVertexAttribArray(attribute_v_coord);
  glVertexAttribPointer(
    attribute_v_coord,  // attribute
    4,                  // number of elements per vertex, here (x,y,z,w)
    GL_FLOAT,           // the type of each element
    GL_FALSE,           // take our values as-is
    0,                  // no extra data between each position
    0                   // offset of first element
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
  glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisableVertexAttribArray(attribute_v_coord);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDeleteBuffers(1, &vbo_vertices);
  glDeleteBuffers(1, &ibo_elements);
}
