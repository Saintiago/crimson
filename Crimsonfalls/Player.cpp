#include "stdafx.h"
#include "Player.h"
#include "Renderer3D.h"

namespace
{
	const float LINEAR_MOVE_SPEED = 2.0f;

	bool ShouldTrackKeyPressed(const SDL_Keysym &key)
	{
		switch (key.sym)
		{
		case SDLK_w:
		case SDLK_a:
		case SDLK_s:
		case SDLK_d:
			return true;
		}
		return false;
	}

	glm::vec2 GetMoveDirection(std::set<unsigned> & keysPressed)
	{
		glm::vec2 direction = { 0, 0 };
		if (keysPressed.count(SDLK_UP) || keysPressed.count(SDLK_w))
		{
			direction.y -= 1;
		}
		if (keysPressed.count(SDLK_DOWN) || keysPressed.count(SDLK_s))
		{
			direction.y += 1;;
		}
		if (keysPressed.count(SDLK_LEFT) || keysPressed.count(SDLK_a))
		{
			direction.x -= 1;;
		}
		if (keysPressed.count(SDLK_RIGHT) || keysPressed.count(SDLK_d))
		{
			direction.x += 1;;
		}
		return direction;
	}
}

CPlayer::CPlayer(float radius)
	: CUnit(radius)
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::Update(float dt)
{
	SetPosition(dt);
	m_renderData.at(0)->model = GetModel();
}

glm::mat4 CPlayer::GetModel()
{
	const glm::vec3 eye = { 0, 0, 0 };
	const glm::vec3 center = { m_turretDirection.x, 0, m_turretDirection.y };
	const glm::vec3 up = { 0, -1, 0 };

	glm::mat4 transform = glm::translate(glm::mat4(), { m_pos.x, 1.f, m_pos.y });
	glm::mat4 directionMatrix = glm::lookAt(eye, center, up);
	return transform * directionMatrix;

}

bool CPlayer::OnKeyDown(const SDL_KeyboardEvent &event)
{
	if (ShouldTrackKeyPressed(event.keysym))
	{
		m_keysPressed.insert(unsigned(event.keysym.sym));
		return true;
	}
	return false;
}

bool CPlayer::OnKeyUp(const SDL_KeyboardEvent &event)
{
	if (ShouldTrackKeyPressed(event.keysym))
	{
		m_keysPressed.erase(unsigned(event.keysym.sym));
		return true;
	}
	return false;
}

void CPlayer::OnMouseMotion(glm::vec2 hitPoint)
{
	m_mousePos = hitPoint;
	SetTurretAngle();
}

void CPlayer::SetTurretAngle()
{
	glm::vec2 turretDirection = m_mousePos - m_pos;
	
	if (glm::length(turretDirection) > 0)
	{
		m_turretDirection = glm::normalize(turretDirection);
	}
}

void CPlayer::SetPosition(float dt)
{
	glm::vec2 direction = GetMoveDirection(m_keysPressed);
	if (glm::length(direction) > 0)
	{
		m_pos += glm::normalize(direction) * LINEAR_MOVE_SPEED * dt;
		m_pos.x = glm::clamp(m_pos.x, -(m_arenaSize.x / 2), (m_arenaSize.x / 2));
		m_pos.y = glm::clamp(m_pos.y, -(m_arenaSize.y / 2), (m_arenaSize.y / 2));
	}
}

void CPlayer::SetArenaSize(glm::vec2 size)
{
	m_arenaSize = size;
}

glm::vec2 CPlayer::GetTurretDirection()
{
	return m_turretDirection;
}
