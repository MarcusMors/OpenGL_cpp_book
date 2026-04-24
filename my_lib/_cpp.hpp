#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using namespace std;

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

GLFWwindow *glfwCreateWindow_cpp(P_glfwCreateWindow_cpp window_settings)
{
  auto ws = window_settings;
  GLFWwindow *window = glfwCreateWindow(ws.width, ws.height, ws.title.c_str(), nullptr, nullptr);
  return window;
}
