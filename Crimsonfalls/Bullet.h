#pragma once
#include "Unit.h"

class CBullet : public CUnit
{
public:
	CBullet(float radius, glm::vec2 pos, glm::vec2 direction);
	~CBullet();

	void Update(float dt) override;

	bool GetAlive();
	void SetAlive(bool alive);

private:

	glm::mat4 GetModel();

	glm::vec2 m_direction;
	bool m_alive = true;
	void SetPosition(float st);
};



