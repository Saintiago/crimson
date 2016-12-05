#pragma once

#include "Ray.h"
#include "Unit.h"

class CPlayer : public CUnit
{
public:
	CPlayer(int textureSlot, float radius);
	~CPlayer();

	void SetArenaSize(glm::vec2 size);
	void Update(float dt) override;
	glm::mat4 GetModel() override;
	SMeshDataP3NT2 GetTesselator() override;

	bool OnKeyDown(const SDL_KeyboardEvent &event);
	bool OnKeyUp(const SDL_KeyboardEvent &event);
	bool OnMouseMotion(const CRay &ray);

private:

	void SetTurretAngle();
	void SetPosition(float dt);

	std::set<unsigned> m_keysPressed;
	glm::vec2 m_mousePos = { 0, 0 };
	glm::vec2 m_turretDirection = { 0, 1 };
	glm::vec2 m_arenaSize = { 10, 10 };
};


