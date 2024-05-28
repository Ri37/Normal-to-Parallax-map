#include "Mesh_OGL3.h"


Mesh::Mesh() :
	m_vao(0),
	m_vbo(0),
	m_ib(0),
	m_vertices(),
	m_indices()
{

}

Mesh::Mesh(Mesh&& rhs) noexcept :
	m_vao(rhs.m_vao),
	m_vbo(rhs.m_vbo),
	m_ib(rhs.m_ib),
	m_vertices(std::move(rhs.m_vertices)),
	m_indices(std::move(rhs.m_indices))
{
	rhs.m_vao = 0;
	rhs.m_vbo = 0;
	rhs.m_ib = 0;
}

Mesh& Mesh::operator=(Mesh&& rhs) noexcept
{
	if (&rhs != this)
	{
		Clean();

		m_vao = rhs.m_vao;
		m_vbo = rhs.m_vbo;
		m_ib = rhs.m_ib;
		m_vertices = std::move(rhs.m_vertices);
		m_indices = std::move(rhs.m_indices);

		rhs.m_vao = 0;
		rhs.m_vbo = 0;
		rhs.m_ib = 0;
	}
	return *this;
}

Mesh::~Mesh() noexcept
{
	Clean();
}


void Mesh::Clean()
{
	if (m_vao != 0)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	if (m_vbo != 0)
	{
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0;
	}
	if (m_ib != 0)
	{
		glDeleteBuffers(1, &m_ib);
		m_ib = 0;
	}
}

void Mesh::initBuffers()
{
	Clean();

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ib);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_vertices.size(), (void*)&m_vertices[0], GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*m_indices.size(), (void*)&m_indices[0], GL_STREAM_DRAW);

	glBindVertexArray(0);
}

void Mesh::draw() const
{
	assert(m_vao != 0);// Kell hogy legyen ervenyes VAO a kirajzolashoz!
	assert(m_vbo != 0);// Kell hogy legyen ervenyes VBO a kirajzolashoz!
	assert(m_ib != 0);// Kell hogy legyen ervenyes index buffer a kirajzolashoz!

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
