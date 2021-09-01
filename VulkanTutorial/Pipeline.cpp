#include "Pipeline.h"

#include <fstream>
#include <stdexcept>

std::vector<char> Pipeline::readShaderFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Failed to open shader file!");

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

void Pipeline::createGraphicPipeline(const std::string& vertFilePath, const std::string& fragFilePath)
{
	auto vertCode = readShaderFile("shaders/vert.spv");
	auto fragCode = readShaderFile("shaders/frag.spv");
}
