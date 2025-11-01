#pragma once

#include "appWin.hpp"
#include "appPipeline.hpp"
#include "my_engine_device.hpp"
#include "my_engine_swap_chain.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace firstGame {
    class app {

        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;
            void run();

            app();
            ~app();

            app(const app &) = delete;
            app &operator=(const app &) = delete;

        private:
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffer();
            void drawFrame();


            appWin win{WIDTH, HEIGHT,  "Hello vk"};
            MyEngineDevice device{win};
            MyEngineSwapChain swapChain{device, win.getExtend()};

            std::unique_ptr<appPipeline> pipeline;
            VkPipelineLayout layout;
            std::vector<VkCommandBuffer> commandBuffers;
    };
};