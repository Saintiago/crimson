#pragma once
#include "libchapter3.h"
#include "IRenderer3D.h"

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

private:
	CProgramContext &m_context;
    CVertexAttribute m_vertexAttr;
    CVertexAttribute m_normalAttr;
    CVertexAttribute m_texCoordAttr;
};
