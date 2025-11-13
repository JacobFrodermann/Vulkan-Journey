#include "appWin.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vulkan/vulkan_core.h>

namespace firstGame {
    appWin::appWin(uint32_t w, uint32_t h, std::string name) : width{w}, height{h}, name{name} {
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

    VkExtent2D appWin::getExtend() {
        return {width, height};
    }

    GLFWwindow* appWin::getWindow() {
        return window;
    }
}