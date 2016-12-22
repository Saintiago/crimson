#include "stdafx.h"
#include "Unit.h"
#include "Renderer3D.h"

CUnit::CUnit(float radius, glm::vec2 pos)
	: m_radius(radius)
	, m_pos(pos)
{
}

CUnit::~CUnit()
{
}

glm::vec2 CUnit::GetPosition()
{
	return m_pos;
}

float CUnit::GetRadius()
{
	return m_radius;
}

RenderDataVector CUnit::GetRenderData()
{
	return m_renderData;
}

void CUnit::SetRenderData(RenderDataVector renderData)
{
	m_renderData = renderData;
}