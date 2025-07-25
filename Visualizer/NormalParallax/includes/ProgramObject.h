#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include "ShaderObject.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <array>

#include <initializer_list>
#include <utility>

class ProgramObject final
{
public:
	using Binding = std::pair<int, const char*>;

	ProgramObject();
	~ProgramObject() noexcept;

	ProgramObject(std::initializer_list<ShaderObject>, std::initializer_list< Binding > = {}, std::initializer_list< Binding > = {});

	ProgramObject(const ProgramObject&) = delete;
	ProgramObject& operator=(const ProgramObject&) = delete;

	ProgramObject(ProgramObject&& rhs) noexcept;
	ProgramObject& operator=(ProgramObject&& rhs) noexcept;

public:
	operator unsigned int() const { return m_id; }

	bool Init(std::initializer_list<ShaderObject>, std::initializer_list< Binding > = {}, std::initializer_list< Binding > = {});
	void Clean() const;

	ProgramObject& AttachShader(const ShaderObject&);
	ProgramObject& AttachShaders(std::initializer_list<ShaderObject>);

	void BindAttribLocation(int, const char*) const;
	void BindAttribLocations( std::initializer_list< Binding > ) const;

	void BindFragDataLocation(int, const char*) const;
	void BindFragDataLocations(std::initializer_list< Binding >) const;

	bool LinkProgram() const;

	void SetTexture(const char* _uniform, int _sampler, GLuint _textureID);
	void SetCubeTexture(const char* _uniform, int _sampler, GLuint _textureID);

	template<typename U, typename T>
	void SetUniform(U _uniform, const T& pArr);

	GLint GetLocation(const char* _uniform);

	void Use() const;
	void Unuse() const;

private:
	GLint GLResolveUniformLocation(GLint _uniform) const;
	GLint GLResolveUniformLocation(const char* _uniform);

private:
	GLuint m_id;

	std::unordered_map< std::string, GLint >	m_map_uniform_locations;
	std::vector< GLuint >						m_list_shaders_attached;
};

#include "ProgramObject.inl"