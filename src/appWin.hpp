#pragma once

#include <string>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace firstGame {
    class appWin {
        private:
            GLFWwindow *window;

            void init();

            const int width;
            const int height;

            appWin(const appWin &) = delete;
            appWin &operator=(const appWin &) = delete;

            const std::string name;
        public:
            appWin(int w, int hm, std::string name);
            ~appWin();

            bool shouldClose();
            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
        };
}