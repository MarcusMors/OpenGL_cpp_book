#include "_cpp.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using namespace std;


void init(GLFWwindow *window) {}

// Function to render a frame
void display(GLFWwindow *window, double currentTime)
{
  glClearColor(1.0, 0.0, 0.0, 1.0);// Red background
  glClear(GL_COLOR_BUFFER_BIT);
}


int main()
{
  // a. initialize GLFW library,
  if (not glfwInit()) { exit(EXIT_FAILURE); }

  request_opengl_version();// 4.3 core profile

  // b. instantiates a GLFWwindow,
  GLFWwindow *window = glfwCreateWindow_cpp({ .width = 600, .height = 600, .title = "Chapter2 - program1" });
  if (not window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  // c. initialize GLEW library,
  if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }

  // d. calls once “init()”: put here app specific tasks.
  init(window);

  glfwSwapInterval(1);// Enable VSync

  // Main loop
  while (not glfwWindowShouldClose(window)) {
    // e. repeatedly calls “display()”:  draws code into GLFWwindow.
    display(window, glfwGetTime());
    glfwSwapBuffers(window);// paints the screen
    glfwPollEvents();// detects events, like keys, mouse, window resize, etc.
  }

  // Clean up
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
