#include "ShaderLoader.h"

#include <fstream>
#include <iostream>
#include <glad/glad.h>

ShaderLoader::ShaderLoader(const char* vertPath, const char* fragPath)
{
	std::string vertSrc = ReadShaderSourceFromFile(vertPath);
	std::string fragSrc = ReadShaderSourceFromFile(fragPath);
	const char* vertCSrc = vertSrc.c_str();
	const char* fragCSrc = fragSrc.c_str();
	programID = glCreateProgram();
	int success;
	char infoLog[512];
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs, 1, &vertCSrc, nullptr);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glShaderSource(fs, 1, &fragCSrc, nullptr);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glAttachShader(programID, vs);
	glAttachShader(programID, fs);
	glLinkProgram(programID);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void ShaderLoader::Use()
{
	glUseProgram(programID);
}

std::string ShaderLoader::ReadShaderSourceFromFile(const char* path)
{
	std::ifstream InFile(path);
	std::string FinalString;
	std::string currentLine;
	if(!InFile.is_open())
	{
		std::cout << "Shader not found at: " << path << '\n';
		return {};
	}
	while(std::getline(InFile,currentLine))
	{
		FinalString.append(currentLine + '\n');
	}
	return FinalString;
}
