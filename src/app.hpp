#pragma once

#include "appWin.hpp"
#include "appPipeline.hpp"
#include "app_model.hpp"
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
    
            void handleKeyPress(int key, int scancode, int action, int mods);

        private:
            void loadModels();
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffer();
            void drawFrame();
            void genMoreTriangles();
            void reduceTriangles();


            appWin win{WIDTH, HEIGHT,  "Hello vk"};
            MyEngineDevice gameDevice{win};
            MyEngineSwapChain swapChain{gameDevice, win.getExtend()};

            std::unique_ptr<appPipeline> pipeline;
            VkPipelineLayout layout;
            std::vector<VkCommandBuffer> commandBuffers;
            std::unique_ptr<appModel> gameModel;    
        
            std::vector<appModel::Vertex> vertices;
            appModel::Vertex halfDistance(appModel::Vertex a, appModel::Vertex b);

            int level;
    };
};