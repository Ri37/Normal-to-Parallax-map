#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include <utility>

#include "GLconversions.hpp"

class ShaderObject final
{
public:
	ShaderObject(GLenum pType);
	ShaderObject(GLenum pType, const std::string&);
	ShaderObject(const TypeSourcePair& pInfo) : ShaderObject(pInfo.first, pInfo.second) {}

	~ShaderObject() noexcept;

	ShaderObject(const ShaderObject&) = delete;
	ShaderObject& operator=(const ShaderObject&) = delete;

	ShaderObject(ShaderObject&& rhs) noexcept;
	ShaderObject& operator=(ShaderObject&& rhs) noexcept;

public:
	operator unsigned int() const { return m_id; }

	void Clean();

	bool FromFile(GLenum _shaderType, const char* _filename) const;
	bool FromMemory(GLenum _shaderType, const std::string& _source) const;

private:
	static GLuint CompileShaderFromMemory(const GLuint _shaderObject, const std::string& _source);

	GLuint	m_id;
};