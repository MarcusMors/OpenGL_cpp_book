#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

// Function to render a frame
void display(GLFWwindow* window, double currentTime) {
    glClearColor(1.0, 0.0, 0.0, 1.0);  // Red background
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(void) {
    // Initialize GLFW
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Request OpenGL 4.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);  // Enable VSync

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}