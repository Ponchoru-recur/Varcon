#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "device.hpp"

// std -
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace lve {

// This tells how we want to configure out Pipeline
struct PipeLineConfigureInfo {};

class LvePipeline {
   public:
    LvePipeline(
        const LveDevice& device,
        const std::string vertFilePath,
        const std::string fragFilePath,
        const PipeLineConfigureInfo& configInfo);
    ~LvePipeline() {};

    // I have no fucking idea what this does.
    LvePipeline(const LvePipeline&) = delete;
    void operator=(const LvePipeline&) = delete;

   private:
    static std::vector<char> readFile(const std::string& file);
    void createGraphicsPipeline(
        const std::string& vertFilePath,
        const std::string& fragFilePath,
        const PipeLineConfigureInfo& configInfo);

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    LveDevice& lveDevice;
    VkPipeline graphicsPipeLine;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};
}  // namespace lve

#endif  // PIPELINE
