#ifndef ___CPP_H__
#define ___CPP_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// This macro creates a static, compile-time character array,
// fills it with the file's bytes via #embed, and returns a lightweight string_view.
// #define GL_EMBED_SHADER(filepath)                              \
//   ([]() -> std::string_view {                                  \
//     static constexpr char shader_data[] = { #embed filepath }; \
//     return std::string_view(shader_data, sizeof(shader_data)); \
//   }())

using namespace std;

template<size_t N> constexpr string_view embed_to_string(const unsigned char (&data)[N])
{
  // Cast the unsigned char array to const char* for string_view
  return string_view(reinterpret_cast<const char *>(data), N);
}

void request_opengl_version()
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

struct P_glfwCreateWindow_cpp
{// window settings
  int width{};
  int height{};
  string title{ "Awesome App Title" };
  GLFWmonitor *fullscreen_mode{};// nullptr for windowed mode
  GLFWwindow *resource_sharing{};// nullptr if no sharing
};

GLFWwindow *glfwCreateWindow(P_glfwCreateWindow_cpp window_settings)
{
  auto ws = window_settings;
  GLFWwindow *window = glfwCreateWindow(ws.width, ws.height, ws.title.c_str(), nullptr, nullptr);
  return window;
}

class Shader
{
public:
  GLuint id{};

  // The constructor takes the shader type and the embedded source code
  Shader(GLenum t_type, string_view t_source) : id{ glCreateShader(t_type) }
  {
    const GLchar *src = t_source.data();
    GLint length = static_cast<GLint>(t_source.size());

    glShaderSource(id, 1, &src, &length);// &length -> nullptr if length is unknown and src is null-terminated
    glCompileShader(id);

    check_compile_errors();
  }

  ~Shader() { glDeleteShader(id); }

  // Delete copy constructors to prevent accidental double-deletion of OpenGL IDs
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

private:
  void check_compile_errors()
  {
    GLint success{};
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (not success) {
      GLint logLength;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
      vector<char> infoLog(logLength);
      glGetShaderInfoLog(id, logLength, nullptr, infoLog.data());
      cerr << "SHADER_COMPILATION_ERROR:\n" << infoLog.data() << "\n";
    }
  }
};

class Program
{
public:
  GLuint id{ glCreateProgram() };

  void validate_id()
  {
    if (id == 0) { throw std::runtime_error("Failed to create OpenGL program object (no context?)"); }
  }
  Program() { validate_id(); }
  Program(const Shader &vertexShader, const Shader &fragmentShader)
  {
    validate_id();
    glAttachShader(id, vertexShader.id);
    glAttachShader(id, fragmentShader.id);
    glLinkProgram(id);

    check_link_errors();
  }

  void attach(Shader t_shader)
  {
    glAttachShader(id, t_shader.id);
    glLinkProgram(id);
    check_link_errors();
  }

  void use() const { glUseProgram(id); }
  ~Program()
  {
    if (id != 0) { glDeleteProgram(id); }
  }

private:
  void check_link_errors()
  {
    GLint success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
      GLchar infoLog[512];
      glGetProgramInfoLog(id, sizeof(infoLog), nullptr, infoLog);
      cerr << "ERROR: Program linking failed:\n" << infoLog << endl;
    }
  }
};

#endif// ___CPP_H__
