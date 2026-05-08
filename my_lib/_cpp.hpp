#ifndef ___CPP_H__
#define ___CPP_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "Shader.hpp"

using namespace std;


struct Shader_Pipeline
{
  Shader_Source vertex{};
  Shader_Source tessellation_control{};
  Shader_Source tessellation_evaluation{};
  Shader_Source geometry{};
  Shader_Source fragment{};
};

enum GL_Draw : GLenum {
  // glDrawArrays works with vertex array objects (VAOs) and buffer objects (VBOs)—the vertex attributes must be
  // properly set up via glVertexAttribPointer and enabled.
  POINTS,
  //
  LINE_STRIP,
  LINE_LOOP,
  LINES,
  //
  TRIANGLE_STRIP,
  TRIANGLE_FAN,
  TRIANGLES,
  //
  LINES_ADJACENCY,
  LINE_STRIP_ADJACENCY,//(for geometry shaders)
  //
  TRIANGLES_ADJACENCY,
  TRIANGLE_STRIP_ADJACENCY
};

struct P_glDrawArrays
{
  GL_Draw mode{};
  GLint start_index{};
  GLsizei count{};
};

// for drawing with indeces use glDrawElements
void glDrawArrays_cpp(P_glDrawArrays P);


template<size_t N> constexpr string_view embed_to_string(const unsigned char (&data)[N])
{
  // Cast the unsigned char array to const char* for string_view
  return string_view(reinterpret_cast<const char *>(data), N);
}

void request_opengl_version();

struct P_glfwCreateWindow
{// window settings
  int width{};
  int height{};
  string title{ "Awesome App Title" };
  GLFWmonitor *fullscreen_mode{};// nullptr for windowed mode
  GLFWwindow *resource_sharing{};// nullptr if no sharing
};

GLFWwindow *glfwCreateWindow(P_glfwCreateWindow window_settings);


class Program
{
public:
  GLuint id{ glCreateProgram() };

  void validate_id();
  Program() { validate_id(); }
  void attach(const Shader_Pipeline &pipeline);

  void use() const { glUseProgram(id); }
  ~Program()
  {
    if (id != 0) { glDeleteProgram(id); }
  }

private:
  GLuint compile_shader(GLenum type, std::string_view source);
  void check_link_errors();
};

#endif// ___CPP_H__
