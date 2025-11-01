#pragma once

#include "appWin.hpp"
#include "appPipeline.hpp"
#include "my_engine_device.hpp"

namespace firstGame {
    class app {

        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;
            void run();

        private:
            appWin win{WIDTH, HEIGHT,  "Hello vk"};
            MyEngineDevice device{win};
            appPipeline pipeline{device, appPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT), "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};
    };
};