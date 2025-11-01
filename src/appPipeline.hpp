#pragma  once

#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "my_engine_device.hpp"

namespace firstGame {

    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class appPipeline {
        public:
            appPipeline(
                MyEngineDevice& device,
                const PipelineConfigInfo& config, 
                const std::string& vertPath, 
                const std::string& fragPath
            );
            
            ~appPipeline();

            appPipeline(const appPipeline&) = delete;
            void operator=(const appPipeline&) = delete;


            void bind(VkCommandBuffer commandBuffer);

            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
        private:
            static std::vector<char> readFile(const std::string& filepath);

            void createGraphicsPipeline(const PipelineConfigInfo& config, const std::string& vertPath, const std::string& fragPath);

            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

            MyEngineDevice &appDevice;
            VkPipeline graphicsPipeline;
            VkShaderModule vertShaderModule, fragShaderModule;


            
    };
};