#include "ShaderParser.h"


std::string parseShader(const std::string& path) {
	std::stringstream result;
	std::ifstream file(path);

	if (file.is_open()) {
		std::string line;
		while (getline(file, line))
		{
			result << line << '\n';
		}
		file.close();
	}
	else
		std::cout << "Unable to open file\n";

	return result.str();
}
