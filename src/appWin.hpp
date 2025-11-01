#pragma once

#include <cstdint>
#include <string>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace firstGame {
    class appWin {
        private:
            GLFWwindow *window;

            void init();

            const uint32_t width;
            const uint32_t height;

            appWin(const appWin &) = delete;
            appWin &operator=(const appWin &) = delete;

            const std::string name;
        public:
            appWin(uint32_t w, uint32_t hm, std::string name);
            ~appWin();

            bool shouldClose();
            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

            VkExtent2D getExtend();
        };
}