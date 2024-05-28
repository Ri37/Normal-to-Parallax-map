#include "ShaderObject.h"
#include <iostream>
#include <fstream>


ShaderObject::ShaderObject(GLenum pType) :
	m_id(glCreateShader(pType))
{

}

ShaderObject::~ShaderObject() noexcept
{
	Clean();
}

ShaderObject::ShaderObject(ShaderObject&& rhs) noexcept :
	m_id(rhs.m_id)
{
	rhs.m_id = 0;
}

ShaderObject& ShaderObject::operator=(ShaderObject&& rhs) noexcept
{
	if (&rhs != this)
	{
		Clean();

		m_id = rhs.m_id;

		rhs.m_id = 0;
	}
	return *this;
}

ShaderObject::ShaderObject(GLenum pType, const std::string &pFilenameOrSource) :
	ShaderObject(pType)
{
	if (!FromFile(pType, pFilenameOrSource.c_str()))
	{
		if ( !std::ifstream(pFilenameOrSource).good())
			FromMemory(pType, pFilenameOrSource);
	}
}


void ShaderObject::Clean()
{
	if (m_id != 0)
	{
		glDeleteShader(m_id);
		m_id = 0;
	}
}

bool ShaderObject::FromFile(GLenum _shaderType, const char* _filename) const
{
	// _fileName megnyitasa
	std::ifstream shaderStream(_filename);

	if (!shaderStream.is_open())
		return false;

	// shaderkod betoltese _fileName fajlbol
	std::string shaderCode = "";

	// file tartalmanak betoltese a shaderCode string-be
	std::string line = "";
	while (std::getline(shaderStream, line))
		shaderCode += line + "\n";

	shaderStream.close();

	// terjunk vissza a forditas eredmenyevel
	return CompileShaderFromMemory(m_id, shaderCode) != 0;
}

bool ShaderObject::FromMemory(GLenum _shaderType, const std::string& _source) const
{
	return CompileShaderFromMemory(m_id, _source) != 0;
}

GLuint ShaderObject::CompileShaderFromMemory(const GLuint _shaderObject, const std::string& _source)
{
	// betoltott kod hozzarendelese a shader-hez
	const char* sourcePointer = _source.c_str();
	glShaderSource(_shaderObject, 1, &sourcePointer, nullptr);

	// shader leforditasa
	glCompileShader(_shaderObject);

	// forditas statuszanak lekerdezese
	GLint result = GL_FALSE;
	glGetShaderiv(_shaderObject, GL_COMPILE_STATUS, &result);

	// ha volt hiba, irjuk ki az okat
	if (GL_FALSE == result)
	{
		int infoLogLength;
		glGetShaderiv(_shaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* error = new char[infoLogLength];
		glGetShaderInfoLog(_shaderObject, infoLogLength, nullptr, error);

		std::cerr << "Hiba: " << error << std::endl;

		delete[] error;
		return 0;
	}

	return _shaderObject;
}