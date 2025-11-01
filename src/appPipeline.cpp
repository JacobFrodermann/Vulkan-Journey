#include "appPipeline.hpp"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace firstGame {
    std::vector<char> appPipeline::readFile(const std::string& filepath) {

        std::ifstream file{filepath, std::ios::ate | std::ios::binary};

        if (!file.is_open()) {
            throw std::runtime_error("failed to open fle " + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());

        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(),fileSize);

        file.close();

        return buffer;
    }

    void appPipeline::createGraphicsPipeline(const PipelineConfigInfo& config, const std::string& vertPath, const std::string& fragPath){
        std::vector<char> vertCode = readFile(vertPath);
        std::vector<char> fragCode = readFile(fragPath);

        std::cout << vertCode.size() << std::endl;
        std::cout << fragCode.size() << std::endl;
    }

    appPipeline::appPipeline(MyEngineDevice& device, const PipelineConfigInfo& config, const std::string& vertPath, const std::string& fragPath) : appDevice(device) {
        createGraphicsPipeline(config, vertPath, fragPath);
    }

    
    void appPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(appDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw  std::runtime_error("failed to create shader module");
        }
    }
    
    PipelineConfigInfo appPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
        PipelineConfigInfo config{};
        return 
        config;
    }
}