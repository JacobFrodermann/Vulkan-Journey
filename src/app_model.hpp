#pragma once

#include "my_engine_device.hpp"
#include <glm/ext/vector_float2.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

namespace firstGame {
    class appModel {
        public:

            struct Vertex {
                glm::vec2 pos;

                static std::vector<VkVertexInputBindingDescription> getBindingDescription();
                static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
            } typedef Vertex;

            appModel(MyEngineDevice& device, const std::vector<Vertex>& vertices);
            ~appModel();
            
            appModel(const appModel &) = delete;
            appModel &operator=(const appModel &) = delete;

            void bind(VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer);
    
            void updateVertices(const std::vector<Vertex>& vertices);

        private:
            MyEngineDevice& appDevice;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            
            uint32_t vertexCount;

            void createVertexBuffers(const std::vector<Vertex> &vertices);

    };                
}