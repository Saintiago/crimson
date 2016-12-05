#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "MeshP3NT2.h"

class CUnit
{
public:
	CUnit(SMeshDataP3NT2 tesselator, int textureSlot, float radius = 0, glm::vec2 pos = { 0, 0 });
	~CUnit();

	void virtual Update(float dt) = 0;
	SMeshDataP3NT2 virtual GetTesselator() = 0;
	glm::mat4 virtual GetModel() = 0;
	void Draw(IRenderer3D &renderer)const;

	glm::vec2 GetPosition();
	float GetRadius();
	int GetTextureSlot();

protected:

	float m_radius;
	int m_textureSlot;
	glm::vec2 m_pos = { 0, 0 };

private:
	
	CMeshP3NT2 m_mesh;
};


