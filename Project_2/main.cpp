#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    std::cout << "Project is compiling and linking successfully!" << std::endl;
    
    // Just a quick test to ensure GLFW is linked
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    glfwTerminate();
    return 0;
}