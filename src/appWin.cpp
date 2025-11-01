#include "appWin.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <string>

namespace firstGame {
    appWin::appWin(int w, int h, std::string name) : width{w}, height{h}, name{name} {
        init();
    }

    void appWin::init() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(window);
        glfwShowWindow(window);


        int h,w;

        glfwGetFramebufferSize(window, &w, &h);

        std::cout << h << " " << w << std::endl;
    }

    appWin::~appWin() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool appWin::shouldClose() {
        return glfwWindowShouldClose(window);
    }
    
    void appWin::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface)) {
            throw std::runtime_error("failed to create Windows surface");
        }
    }
}