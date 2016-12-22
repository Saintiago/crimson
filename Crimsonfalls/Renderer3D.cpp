#include "stdafx.h"
#include "Renderer3D.h"
#include "ProgramContext.h"

namespace
{
	GLenum GetPrimitiveType(MeshType type)
	{
		switch (type)
		{
		case MeshType::Triangles:
			return GL_TRIANGLES;
		case MeshType::TriangleFan:
			return GL_TRIANGLE_FAN;
		case MeshType::TriangleStrip:
			return GL_TRIANGLE_STRIP;
		default:
			throw std::logic_error("Unhandled mesh type");
		}
	}
}

CRenderer3D::CRenderer3D(CProgramContext &context)
    : m_context(context)
    , m_vertexAttr(m_context.GetPositionAttr())
    , m_normalAttr(m_context.GetNormalAttr())
    , m_texCoordAttr(m_context.GetTexCoordAttr())
{
    m_context.Use();
    m_vertexAttr.EnablePointer();
    m_normalAttr.EnablePointer();
    m_texCoordAttr.EnablePointer();
}

CRenderer3D::~CRenderer3D()
{
    m_vertexAttr.DisablePointer();
    m_normalAttr.DisablePointer();
    m_texCoordAttr.DisablePointer();
}

void CRenderer3D::SetTexCoord2DOffset(size_t offset, size_t stride)
{
    m_texCoordAttr.SetVec2Offset(offset, stride);
}

void CRenderer3D::SetPosition3DOffset(size_t offset, size_t stride)
{
    m_vertexAttr.SetVec3Offset(offset, stride, false);
}

void CRenderer3D::SetNormalOffset(size_t offset, size_t stride)
{
    m_normalAttr.SetVec3Offset(offset, stride, false);
}

void CRenderer3D::SetModel(glm::mat4 model)
{
	m_context.SetModel(model);
}

void CRenderer3D::SetTextureSlot(int textureSlot)
{
	m_context.SwitchShaderTextureSlot(textureSlot);
}

void CRenderer3D::Draw(RenderDataPtr sceneObject)
{
	// Выполняем привязку vertex array, normal array, tex coord 2d array
	const size_t stride = sizeof(SVertexP3NT2);
	const size_t positionOffset = size_t(offsetof(SVertexP3NT2, position));
	const size_t normalOffset = size_t(offsetof(SVertexP3NT2, normal));
	const size_t texCoordOffset = size_t(offsetof(SVertexP3NT2, texCoord));

	CBufferObject attributesBuffer(BufferType::Attributes);
	CBufferObject indexesBuffer(BufferType::Indicies);

	attributesBuffer.Copy(sceneObject->mesh.vertices);
	indexesBuffer.Copy(sceneObject->mesh.indicies);

	SetModel(sceneObject->model);
	SetTextureSlot(sceneObject->textureSlot);
	SetPosition3DOffset(positionOffset, stride);
	SetNormalOffset(normalOffset, stride);
	SetTexCoord2DOffset(texCoordOffset, stride);

	const GLenum primitive = GetPrimitiveType(MeshType::TriangleStrip);
	const GLvoid *indexOffset = reinterpret_cast<const GLvoid *>(0);
	const GLuint minIndex = 0;
	const GLuint maxIndex = GLuint(sceneObject->mesh.vertices.size());
	const GLsizei size = GLsizei(sceneObject->mesh.indicies.size());

	glDrawRangeElements(primitive, minIndex, maxIndex, size,
		GL_UNSIGNED_INT, indexOffset);
}