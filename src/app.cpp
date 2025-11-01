#include "app.hpp"
#include <GLFW/glfw3.h>

namespace firstGame {
    void app::run() {
        while (!win.shouldClose()) {
            glfwPollEvents();
        }
    }
}