#pragma once
#include <string>

class ShaderLoader
{
public:
	ShaderLoader(const char* vertPath, const char* fragPath);
	void Use();
	unsigned GetProgramID() const { return programID; }
private:
	std::string ReadShaderSourceFromFile(const char* path);
	unsigned programID = 0;
};

