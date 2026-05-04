#include "_cpp.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

// Helper function to check shader compilation status
// void checkShaderCompilation(GLuint shader, const char *shaderType)
// {
//   GLint success;
//   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//   if (!success) {
//     GLchar infoLog[512];
//     glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
//     cerr << "ERROR: " << shaderType << " shader compilation failed:\n" << infoLog << endl;
//   }
// }

// Helper function to check program linking status
// void checkProgramLinking(GLuint program)
// {
//   GLint success;
//   glGetProgramiv(program, GL_LINK_STATUS, &success);
//   if (!success) {
//     GLchar infoLog[512];
//     glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
//     cerr << "ERROR: Program linking failed:\n" << infoLog << endl;
//   }
// }

// GLuint createShaderProgram()
// {
//   static const char vshader_source[] = {
// #embed "vertex.glsl"
//     , '\0'
//   };
//   GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
//   const char *vSrc = vshader_source;
//   glShaderSource(vShader, 1, &vSrc, nullptr);
//   glCompileShader(vShader);
//   checkShaderCompilation(vShader, "vertex");

//   static const char fshader_source[] = {
// #embed "fragment.glsl"
//     , '\0'
//   };
//   GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
//   const char *fSrc = fshader_source;
//   glShaderSource(fShader, 1, &fSrc, nullptr);
//   glCompileShader(fShader);
//   checkShaderCompilation(fShader, "fragment");


//   GLuint program = glCreateProgram();
//   glAttachShader(program, vShader);
//   glAttachShader(program, fShader);
//   glLinkProgram(program);
//   checkProgramLinking(program);

//   // Shader objects are no longer needed after linking
//   glDeleteShader(vShader);
//   glDeleteShader(fShader);

//   return program;
// }

void init(GLFWwindow *window)
{
  static constexpr unsigned char vert_src[] = {
#embed "vertex.glsl"
  };

  static constexpr unsigned char frag_src[] = {
#embed "fragment.glsl"
  };

  Program program{};
  // Program program{ Shader(GL_VERTEX_SHADER, embed_to_string(vert_src)),
  //   Shader(GL_FRAGMENT_SHADER, embed_to_string(frag_src)) };
  renderingProgram = program.id;
  program.attach(Shader(GL_VERTEX_SHADER, embed_to_string(vert_src)));
  program.attach(Shader(GL_FRAGMENT_SHADER, embed_to_string(frag_src)));

  glGenVertexArrays(numVAOs, vao);
  glBindVertexArray(vao[0]);
  // No VBO needed because vertex shader does not read any attribute
}

void display(GLFWwindow *window, double currentTime)
{
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);// optional: set clear color once
  glClear(GL_COLOR_BUFFER_BIT);// clear the screen

  glUseProgram(renderingProgram);
  glDrawArrays(GL_POINTS, 0, 1);// draw a single point at (0,0)

  glfwSwapBuffers(window);// present the rendered frame
}

int main()
{
  // Initialize GLFW
  if (!glfwInit()) {
    cerr << "Failed to initialize GLFW" << endl;
    return -1;
  }

  request_opengl_version();// 4.3 core profile

  GLFWwindow *window = glfwCreateWindow({ .width = 600, .height = 600, .title = "Chapter2 - program1" });
  if (window == nullptr) {
    cerr << "Failed to create window" << endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // c. initialize GLEW library,
  // not now
  glewInit();// Initialize GLEW (must be done after a valid GL context)

  // d. calls once “init()”: put here app specific tasks.
  init(window);

  glfwSwapInterval(1);// enable vsync

  // Main loop
  while (not glfwWindowShouldClose(window)) {
    double currentTime = glfwGetTime();
    display(window, currentTime);
    glfwPollEvents();
  }

  // Optional cleanup
  // glDeleteProgram(renderingProgram);
  // glDeleteVertexArrays(numVAOs, vao);
  glfwDestroyWindow(window);
  glfwTerminate();

  exit(EXIT_SUCCESS);
  return 0;
}
