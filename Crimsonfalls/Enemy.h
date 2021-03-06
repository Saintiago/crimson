#pragma once

#include "Ray.h"
#include "Unit.h"

class CEnemy : public CUnit
{
public:
	CEnemy(float radius, glm::vec2 pos);
	~CEnemy();

	void Update(float dt) override;
	void UpdateDirection(glm::vec2 playerPos);

	bool GetAlive();
	void SetAlive(bool alive);

private:

	glm::mat4 GetModel();
	
	glm::vec2 m_direction;
	bool m_alive = true;
	void SetPosition(float st);
};


