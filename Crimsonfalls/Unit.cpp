#include "stdafx.h"
#include "Unit.h"
#include "Renderer3D.h"

CUnit::CUnit(SMeshDataP3NT2 tesselator, int textureSlot, float radius, glm::vec2 pos)
	: m_mesh(MeshType::TriangleStrip)
	, m_radius(radius)
	, m_pos(pos)
	, m_textureSlot(textureSlot)
{
	m_mesh.Copy(tesselator);
}


CUnit::~CUnit()
{
}

glm::vec2 CUnit::GetPosition()
{
	return m_pos;
}

void CUnit::Draw(IRenderer3D &renderer) const
{
	m_mesh.Draw(renderer);
}

float CUnit::GetRadius()
{
	return m_radius;
}

int CUnit::GetTextureSlot()
{
	return m_textureSlot;
}