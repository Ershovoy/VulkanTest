#pragma once

#include <string>
#include <vector>

class Pipeline
{
public:
	Pipeline(const std::string& vertFilePath, const std::string& fragFilePath);
	~Pipeline();
private:
	static std::vector<char> readShaderFile(const std::string& filename);

	void createGraphicPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
};

