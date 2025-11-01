#include "app.hpp"
#include "appPipeline.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <system_error>
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
            drawFrame();
        }

        vkDeviceWaitIdle(device.device());
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
        commandBuffers.resize(swapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.getCommandPool();
        allocInfo.commandBufferCount = commandBuffers.size();

        if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw "failed to create command buffers";
        }

        for (int i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = swapChain.getRenderPass();
            renderPassInfo.framebuffer = swapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0,0};
            renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();


            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1, 0.1, 0.1};
            clearValues[1].depthStencil = {1,0};

            renderPassInfo.clearValueCount = clearValues.size();
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            pipeline->bind(commandBuffers[i]);

            vkCmdDraw(commandBuffers[i], 6, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);

            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to end command Buffer");
            }
        }
    }

    void app::drawFrame() {
        uint32_t imageIndex;
        VkResult result = swapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain");
        }

        result = swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to submit command buffer");
        }
    }
}