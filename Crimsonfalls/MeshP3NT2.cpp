#include "stdafx.h"
#include "MeshP3NT2.h"
#include "IRenderer3D.h"

CMeshP3NT2::CMeshP3NT2(MeshType meshType)
    : m_meshType(meshType)
{
}

MeshType CMeshP3NT2::GetMeshType()
{
	return m_meshType;
}

size_t CMeshP3NT2::GetIndiciesCount()
{
	return m_indiciesCount;
}

size_t CMeshP3NT2::GetVerticiesCount()
{
	return m_verticiesCount;
}
