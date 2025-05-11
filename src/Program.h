#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>

class Program
{
public:
	Program();

	GLuint GetID();

	void Activate();
	void Deactivate();

	GLuint GetUniformID(const std::string& uniformName);

	bool Load(std::string vertexShaderFilePath, std::string fragmentShaderShaderPath);

public:
	static void setProgramsDirectory(std::string programDirectory);
	static std::string getProgramsDirectory();

private:
	bool readFile(std::string filePath, std::string& content);
	bool compileShader(GLenum shaderType, GLuint* shader, std::string shaderSource);
	bool linkProgram(const GLuint vertexShaderID, const GLuint fragmentShaderID);
	void setupUniforms();

private:
	GLuint _programID;
	std::map<std::string, GLuint> _uniformsID;
	static std::string _programsDirectory;
};
