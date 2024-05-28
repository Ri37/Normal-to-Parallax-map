template<BufferType type, BufferUsage usage>
inline void VertexArrayObject::AddAttribute(AttributeData& pAttrib, BufferObject<type, usage>& pBuffer) const
{
	pBuffer.Bind();
	pAttrib.Apply();
	return *this;
}
