#pragma once

#include "Ray.h"
#include "Unit.h"

class CEnemy : public CUnit
{
public:
	CEnemy(int textureSlot, float radius, glm::vec2 pos);
	~CEnemy();

	void Update(float dt) override;
	glm::mat4 GetModel();
	SMeshDataP3NT2 GetTesselator();
	void UpdateDirection(glm::vec2 playerPos);

private:
	
	glm::vec2 m_direction;

	void SetPosition(float st);
};


