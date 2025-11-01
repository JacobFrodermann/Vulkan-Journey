#include "app.hpp"
#include "appPipeline.hpp"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace firstGame {

    app::app() {
        createPipelineLayout();
        createPipeline();
        createCommandBuffer();
    }

    app::~app() {
        vkDestroyPipelineLayout(device.device(), layout, nullptr);
    }

    void app::run() {
        while (!win.shouldClose()) {
            glfwPollEvents();
        }
    }

    void app::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &layout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create Pipeline Layout");
        }
    }

    void app::createPipeline() {
        PipelineConfigInfo pipelineConfig = appPipeline::defaultPipelineConfigInfo(swapChain.width(), swapChain.height());

        pipelineConfig.renderPass = swapChain.getRenderPass();
        pipelineConfig.pipelineLayout = layout;

        pipeline = std::make_unique<appPipeline>(
            device,
            pipelineConfig,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv"
        );
    }

    void app::createCommandBuffer() {

    }

    void app::drawFrame() {
        
    }
}