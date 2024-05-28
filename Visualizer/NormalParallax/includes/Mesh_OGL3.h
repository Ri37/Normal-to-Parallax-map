#pragma once

#include <GL/glew.h>

#include <vector>
#include <glm/glm.hpp>

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;
	};

public:
	Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& rhs) noexcept;
	Mesh& operator=(Mesh&& rhs) noexcept;

	~Mesh() noexcept;

public:
	void Clean();

	void initBuffers();
	void draw() const;

	void addVertex(const Vertex& vertex) { m_vertices.push_back(vertex); }
	void addIndex(unsigned int index) { m_indices.push_back(index); }

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ib;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
};
