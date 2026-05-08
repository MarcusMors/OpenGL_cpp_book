#include "_cpp.hpp"
#include <stdexcept>

using namespace std;

void glDrawArrays_cpp(P_glDrawArrays P) { glDrawArrays(static_cast<GLenum>(P.mode), P.start_index, P.count); }

void request_opengl_version()
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
GLFWwindow *glfwCreateWindow(P_glfwCreateWindow window_settings)
{
  auto ws = window_settings;
  GLFWwindow *window = glfwCreateWindow(ws.width, ws.height, ws.title.c_str(), nullptr, nullptr);
  return window;
}

void Program::validate_id()
{
  if (id == 0) { throw std::runtime_error("Failed to create OpenGL program object (no context?)"); }
}


void Program::attach(const Shader_Pipeline &pipeline)
{
  // Validate tessellation pair
  const bool has_tcs = !pipeline.tessellation_control.empty();
  const bool has_tes = !pipeline.tessellation_evaluation.empty();
  if (has_tcs != has_tes) {
    throw std::runtime_error("Tessellation control and evaluation must both be present or both absent");
  }

  // Compile each stage that has source
  vector<GLuint> shaders;

  auto compile = [&](GLenum type, string_view source, string name) {
    if (!source.empty()) {
      GLuint shader = compile_shader(type, source);
      glAttachShader(id, shader);
      shaders.push_back(shader);
    }
  };

  compile(GL_VERTEX_SHADER, pipeline.vertex.view(), "vertex");
  compile(GL_TESS_CONTROL_SHADER, pipeline.tessellation_control.view(), "tessellation control");
  compile(GL_TESS_EVALUATION_SHADER, pipeline.tessellation_evaluation.view(), "tessellation evaluation");
  compile(GL_GEOMETRY_SHADER, pipeline.geometry.view(), "geometry");
  compile(GL_FRAGMENT_SHADER, pipeline.fragment.view(), "fragment");

  // Link the program
  glLinkProgram(id);
  check_link_errors();

  // Shader objects are no longer needed after linking
  for (GLuint shader : shaders) glDeleteShader(shader);
}

GLuint Program::compile_shader(GLenum type, string_view source)
{
  GLuint shader = glCreateShader(type);
  const char *src = source.data();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (not success) {
    char log[512];
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    throw std::runtime_error("Shader compilation failed: " + string{ log });
  }
  return shader;
}

void Program::check_link_errors()
{
  GLint success;
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar infoLog[512];
    glGetProgramInfoLog(id, sizeof(infoLog), nullptr, infoLog);
    cerr << "ERROR: Program linking failed:\n" << infoLog << endl;
  }
}

// void attach(Shader_Pipeline pipeline)
// {
//   if (pipeline.tessellation_control.empty() != pipeline.tessellation_evaluation.empty()) {
//     throw std::runtime_error("Tessellation control and evaluation must both be present or both absent");
//   }

//   // Compile each stage that has source
//   auto create_and_attach = [&](GLenum type, Shader_Source source) {
//     if (!source.empty()) {
//       Shader shader(type, source);
//       // shaders.push_back({ type, source });
//       // shaders.push_back(std::move(Shader{ type, source }));
//       // shaders.emplace_back(type, source);
//       // glAttachShader(id, shaders.back().id);
//       glAttachShader(id, shader.id);
//     }
//   };

//   // for (auto s : shaders) {
//   // }


//   create_and_attach(GL_VERTEX_SHADER, pipeline.vertex);
//   create_and_attach(GL_TESS_CONTROL_SHADER, pipeline.tessellation_control);
//   create_and_attach(GL_TESS_EVALUATION_SHADER, pipeline.tessellation_evaluation);
//   create_and_attach(GL_GEOMETRY_SHADER, pipeline.geometry);
//   create_and_attach(GL_FRAGMENT_SHADER, pipeline.fragment);

//   // Link the program
//   glLinkProgram(id);
//   check_link_errors();
// }
