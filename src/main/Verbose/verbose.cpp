#include "verbose.h"

void PrintGLVerbose() {
  // Prints out the OpenGL version
  std::cout << "" << std::endl;
  std::cout << ""
            << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << ""
            << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << ""
            << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << ""
            << "OpenGL Shading Language Version: "
            << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "" << std::endl;
}