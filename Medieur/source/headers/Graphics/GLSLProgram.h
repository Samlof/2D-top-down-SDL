#pragma once

#include <string>
#include <GL\glew.h>

class GLSLProgram {
public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string & pVertexShaderFilePath, const std::string & pFragmentShaderFilePath);
	void linkShaders();
	void addAttribute(const std::string& pAttributeName);

	void use();
	void unuse();

private:

	void compileShader(const std::string & pFilePath, GLuint pShaderId);
	GLuint mProgramId;
	GLuint mVertexShaderId, mFragmentShaderId;
	int mNumAttributes;
};