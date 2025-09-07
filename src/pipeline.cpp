#include "pipeline.hpp"

namespace lve {

LvePipeline::LvePipeline(const std::string vertFilePath, std::string fragFilePath) {
    createGraphicsPipeline(vertFilePath, fragFilePath);
}

// This reads the file and puts it in a vector
std::vector<char> LvePipeline::readFile(const std::string& filepath) {
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};  // When the files open we seek the end of the file immediately and binary to avoid unwanted text transformations from occuring
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file " + filepath);
    }
    // Tells how many bytes because std::ios::ate already told file to go at the end and tellg() tells
    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    // Put cursor at the beginning
    file.seekg(0);
    // Read the file and put it in buffer.
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

void LvePipeline::createGraphicsPipeline(
    const std::string& vertFilepath, std::string& fragFilepath) {
    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);

    std::cout << "Vertex Shader Code Size : " << vertCode.size() << "\n";
    std::cout << "Fragment Shader Code Size : " << vertCode.size() << "\n";
}

}  // namespace lve
