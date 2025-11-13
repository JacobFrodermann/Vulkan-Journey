#include "app_model.hpp"
#include "my_engine_device.hpp"

#include <cassert>
#include <cstddef>
#include <cstring>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace firstGame {
    appModel::appModel(MyEngineDevice &dev, const std::vector<Vertex>& vertices) : appDevice{dev} {
        createVertexBuffers(vertices);
    }

    appModel::~appModel(){
        vkDestroyBuffer(appDevice.device(), vertexBuffer,nullptr);
        vkFreeMemory(appDevice.device(), vertexBufferMemory, nullptr);
    }

    void appModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "vertex count must be >= 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        appDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_2_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vertexBuffer,
            vertexBufferMemory
        );

        void *data;
        vkMapMemory(appDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(appDevice.device(), vertexBufferMemory);
    }

    void appModel::updateVertices(const std::vector<Vertex>& vertices) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "vertex count must be >= 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        void *data;
        vkMapMemory(appDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(appDevice.device(), vertexBufferMemory);
    }

    void appModel::draw(VkCommandBuffer cmdBuf) {
        vkCmdDraw(cmdBuf, vertexCount, 1, 0 ,0);
    }

    void appModel::bind(VkCommandBuffer cmdBuf) {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(cmdBuf, 0, 2, buffers, offsets);
    }

    std::vector<VkVertexInputBindingDescription> appModel::Vertex::getBindingDescription() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> appModel::Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = 0;
        return attributeDescriptions;
    }
}