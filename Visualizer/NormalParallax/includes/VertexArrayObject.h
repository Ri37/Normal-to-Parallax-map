#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include "GLconversions.hpp"
#include "BufferObject.h"

#include <initializer_list>
#include <utility>

struct AttributeData
{
	AttributeData(GLuint pIndex, GLint pSize, GLenum pType, GLboolean pNormalized, GLsizei pStride, void* pPtr) :
		index(pIndex), size(pSize), type(pType), normalized(pNormalized), stride(pStride), ptr(pPtr) 
	{ }

	void Apply();

	GLuint		index{};
	GLint		size{};
	GLenum		type{};
	GLboolean	normalized{};
	GLsizei		stride{};
	void*		ptr{};
};

// using template variable for ease of notation on call side
template <GLuint index, typename T, size_t pOffsetInBytes = 0, GLsizei pStrideInBytes = sizeof(T)>
AttributeData CreateAttribute = AttributeData(index, sizeof(T) / sizeof(typename GLExtractPrimitiveType<T>::primitive_type), NativeTypeToOpenGLType<typename GLExtractPrimitiveType<T>::primitive_type>(), false, pStrideInBytes, (void*)pOffsetInBytes);
// otherwise, the following template function is more readable and serves as the function equivalent of the above template variable:
/*
template <GLuint index, typename T>
AttributeData CreateAttribute(size_t pOffsetInBytes = 0, GLsizei pStrideInBytes = sizeof(T))
{
	using PrimitiveType = typename GLExtractPrimitiveType<T>::primitive_type;
	return AttributeData(index, sizeof(T) / sizeof(PrimitiveType), NativeTypeToOpenGLType<PrimitiveType>(), false, pStrideInBytes, (void*)pOffsetInBytes);
}
*/


class VertexArrayObject final
{
public:
	VertexArrayObject();
	~VertexArrayObject() noexcept;

	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	VertexArrayObject(VertexArrayObject&& rhs) noexcept;
	VertexArrayObject& operator=(VertexArrayObject&& rhs) noexcept;

public:
	operator unsigned int() const { return m_id;};

	void Clean();

	void Init(std::initializer_list< std::pair<AttributeData, const ArrayBuffer&> > pDataBuffers) const;
	void Init(std::initializer_list< std::pair<AttributeData, const ArrayBuffer&> > pDataBuffers, const IndexBuffer& pIndexBuffer) const;

	void Bind() const;
	void Unbind() const;

	template <BufferType type, BufferUsage usage>
	void AddAttribute(AttributeData&, BufferObject<type, usage>&) const;

	void SetIndices(const IndexBuffer&) const;

private:
	GLuint m_id;
};

#include "VertexArrayObject.inl"