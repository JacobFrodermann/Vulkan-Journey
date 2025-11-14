#include "app.hpp"
#include "appPipeline.hpp"
#include "app_model.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <system_error>
#include <type_traits>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace firstGame {

    app::app() {
        level = 0;
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffer();
    }

    app::~app() {
        vkDestroyPipelineLayout(gameDevice.device(), layout, nullptr);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        app* ap = (app*) glfwGetWindowUserPointer(window);
        ap->handleKeyPress(key, scancode, action, mods);
    }


    void app::run() {
        glfwSetWindowUserPointer(win.getWindow(), this);
        glfwSetKeyCallback(win.getWindow(), key_callback);

        while (!win.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(gameDevice.device());
    }

    void app::loadModels() {
        vertices =  {
            {{0.f, -.5f}},
            {{0.5f, .5f}},
            {{-0.5f, .5f}}
        };

        for (int i = 0; i < 729*9; i++) {
            vertices.push_back({});
        }

        gameModel = std::make_unique<appModel>(gameDevice, vertices);
    }

    void app::handleKeyPress(int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            genMoreTriangles();
            gameModel->updateVertices(vertices);
        } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
            reduceTriangles();
            gameModel->updateVertices(vertices);
        }

    }

    void app::genMoreTriangles() {
        std::vector<appModel::Vertex> newVertices = {}; 

        for (int i = 0; i < vertices.size() && i < 729; i +=3) {
            if (vertices[i].pos.x == 0 && vertices[i].pos.y == 0) continue;
            
            newVertices.push_back(vertices[i]);
            newVertices.push_back(halfDistance(vertices[i], vertices[i+1]));
            newVertices.push_back(halfDistance(vertices[i], vertices[i+2]));
            
            newVertices.push_back(vertices[i+1]);
            newVertices.push_back(halfDistance(vertices[i+1], vertices[i]));
            newVertices.push_back(halfDistance(vertices[i+1], vertices[i+2]));
            
            newVertices.push_back(vertices[i+2]);
            newVertices.push_back(halfDistance(vertices[i+2], vertices[i]));
            newVertices.push_back(halfDistance(vertices[i+2], vertices[i+1]));
        }
        vertices = newVertices;
        std::cout << newVertices.size() << std::endl;
    }

    void app::reduceTriangles() {
        std::vector<appModel::Vertex> newVertices = {};
        if (vertices.size() == 3) return;
        
        for (int i = 0; i < vertices.size(); i += 3) {
            newVertices.push_back(vertices[i]);
        }

        vertices = newVertices;
        std::cout << newVertices.size() << std::endl;
    }

    appModel::Vertex app::halfDistance(appModel::Vertex a, appModel::Vertex b) {
        return appModel::Vertex{{
            a.pos.x+(b.pos.x-a.pos.x)/2,
            a.pos.y+(b.pos.y-a.pos.y)/2
        }};
    }

    void app::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(gameDevice.device(), &pipelineLayoutInfo, nullptr, &layout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create Pipeline Layout");
        }
    }

    void app::createPipeline() {
        PipelineConfigInfo pipelineConfig = appPipeline::defaultPipelineConfigInfo(swapChain.width(), swapChain.height());

        pipelineConfig.renderPass = swapChain.getRenderPass();
        pipelineConfig.pipelineLayout = layout;

        pipeline = std::make_unique<appPipeline>(
            gameDevice,
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
        allocInfo.commandPool = gameDevice.getCommandPool();
        allocInfo.commandBufferCount = commandBuffers.size();

        if (vkAllocateCommandBuffers(gameDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
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

            gameModel->bind(commandBuffers[i]);
            gameModel->draw(commandBuffers[i]);

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