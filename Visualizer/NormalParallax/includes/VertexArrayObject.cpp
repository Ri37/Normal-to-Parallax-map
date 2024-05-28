#include "VertexArrayObject.h"
#include <cassert>


void AttributeData::Apply()
{
	glEnableVertexAttribArray(index);
	switch (type)
	{
		case GL_INT:
		case GL_UNSIGNED_INT:
			glVertexAttribIPointer(index, size, type, stride, ptr);
			break;
		case GL_FLOAT:
		case GL_DOUBLE:
			glVertexAttribPointer(index, size, type, normalized, stride, ptr);
			break;
		default:
			assert(false);// this wrapper does not support this data type for vertex attributes
			break;
	}
}


VertexArrayObject::VertexArrayObject() :
	m_id(0)
{
	glGenVertexArrays(1, &m_id);
}

VertexArrayObject::~VertexArrayObject() noexcept
{
	Clean();
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& rhs) noexcept :
	m_id(rhs.m_id)
{
	rhs.m_id = 0;
}

VertexArrayObject & VertexArrayObject::operator=(VertexArrayObject&& rhs) noexcept
{
	if (&rhs != this)
	{
		Clean();
		m_id = rhs.m_id;
		rhs.m_id = 0;
	}
	return *this;
}


void VertexArrayObject::Clean()
{
	if (m_id != 0)
	{
		glDeleteVertexArrays(1, &m_id);
		m_id = 0;
	}
}

void VertexArrayObject::Bind() const
{
	glBindVertexArray(m_id);
}

void VertexArrayObject::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArrayObject::SetIndices(const IndexBuffer& pIndexBuffer) const
{
	Bind();
	pIndexBuffer.Bind();
}

void VertexArrayObject::Init(std::initializer_list<std::pair<AttributeData, const ArrayBuffer&>> pDataBuffers) const
{
	Bind();
	for (auto val : pDataBuffers)
	{
		val.second.Bind();
		val.first.Apply();
	}
}

void VertexArrayObject::Init(std::initializer_list<std::pair<AttributeData, const ArrayBuffer&>> pDataBuffers, const IndexBuffer& pIndexBuffer) const
{
	Init(pDataBuffers);
	pIndexBuffer.Bind();
	Unbind();
}