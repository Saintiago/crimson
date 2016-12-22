#include "stdafx.h"
#include "Bullet.h"
#include "Renderer3D.h"

namespace
{
	const float LINEAR_MOVE_SPEED = 7.5f;
}

CBullet::CBullet(float radius, glm::vec2 pos, glm::vec2 direction)
	: CUnit(radius, pos)
	, m_direction(direction)
{
}


CBullet::~CBullet()
{
}

void CBullet::Update(float dt)
{
	SetPosition(dt);
	m_renderData.at(0)->model = GetModel();
}

void CBullet::SetPosition(float dt)
{
	if (glm::length(m_direction) > 0)
	{
		m_pos += glm::normalize(m_direction) * LINEAR_MOVE_SPEED * dt;
	}
}

glm::mat4 CBullet::GetModel()
{
	const glm::vec3 eye = { 0, 0, 0 };
	const glm::vec3 center = { m_direction.x, 0, m_direction.y };
	const glm::vec3 up = { 0, -1, 0 };

	glm::mat4 transform = glm::translate(glm::mat4(), { m_pos.x, 0.5f, m_pos.y });
	glm::mat4 directionMatrix = glm::lookAt(eye, center, up);
	return transform * directionMatrix;

}

bool CBullet::GetAlive()
{
	return m_alive;
}

void CBullet::SetAlive(bool alive)
{
	m_alive = alive;
}