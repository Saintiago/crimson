#pragma once
#include "libchapter3.h"
#include "IRenderer3D.h"
#include "MeshP3NT2.h"

struct RenderData
{
	RenderData::RenderData(SMeshDataP3NT2 mesh, glm::mat4 model, int textureSlot)
		: mesh(mesh)
		, model(model)
		, textureSlot(textureSlot)
	{
	}

	SMeshDataP3NT2 mesh;
	glm::mat4 model;
	int textureSlot;
};

typedef std::shared_ptr<RenderData> RenderDataPtr;
typedef std::vector<RenderDataPtr> RenderDataVector;
typedef std::shared_ptr<CBufferObject> BufferPtr;

class CProgramContext;

class CRenderer3D : public IRenderer3D
{
public:
	CRenderer3D(CProgramContext &context);
    ~CRenderer3D();

    // IRenderer3D interface
    void SetTexCoord2DOffset(size_t offset, size_t stride) override;
    void SetPosition3DOffset(size_t offset, size_t stride) override;
    void SetNormalOffset(size_t offset, size_t stride) override;
	void SetModel(glm::mat4 model);
	void SetTextureSlot(int textureSlot);
	void Draw(RenderDataPtr sceneObject);

private:
	CProgramContext &m_context;
    CVertexAttribute m_vertexAttr;
    CVertexAttribute m_normalAttr;
    CVertexAttribute m_texCoordAttr;
};
