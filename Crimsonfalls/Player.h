#pragma once

#include "Ray.h"
#include "Unit.h"

class CPlayer;

typedef std::shared_ptr<CPlayer> PlayerPtr;

class CPlayer : public CUnit
{
public:
	CPlayer(float radius);
	~CPlayer();

	void SetArenaSize(glm::vec2 size);
	void Update(float dt) override;
	
	bool OnKeyDown(const SDL_KeyboardEvent &event);
	bool OnKeyUp(const SDL_KeyboardEvent &event);
	void OnMouseMotion(glm::vec2 hitPoint);
	glm::vec2 GetTurretDirection();

private:

	void SetTurretAngle();
	void SetPosition(float dt);
	glm::mat4 GetModel();

	std::set<unsigned> m_keysPressed;
	glm::vec2 m_mousePos = { 0, 0 };
	glm::vec2 m_turretDirection = { 0, 1 };
	glm::vec2 m_arenaSize = { 10, 10 };
};


