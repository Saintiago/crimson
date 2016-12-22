#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "MeshP3NT2.h"
#include "Renderer3D.h"

class CUnit;

typedef std::shared_ptr<CUnit> UnitPtr;

class CUnit
{
public:
	CUnit(float radius = 0, glm::vec2 pos = { 0, 0 });
	~CUnit();

	void virtual Update(float dt) = 0;

	RenderDataVector GetRenderData();
	glm::vec2 GetPosition();
	float GetRadius();

	void SetRenderData(RenderDataVector renderData);

protected:

	float m_radius;
	RenderDataVector m_renderData;
	glm::vec2 m_pos;
};


