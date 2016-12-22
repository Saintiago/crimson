#include "stdafx.h"
#include "Enemy.h"
#include "Renderer3D.h"

namespace
{
	const float LINEAR_MOVE_SPEED = 0.8f;
}

CEnemy::CEnemy(float radius, glm::vec2 pos)
	: CUnit(radius, pos)
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Update(float dt)
{
	SetPosition(dt);
	m_renderData.at(0)->model = GetModel();
}

void CEnemy::SetPosition(float dt)
{
	if (glm::length(m_direction) > 0)
	{
		m_pos += glm::normalize(m_direction) * LINEAR_MOVE_SPEED * dt;
	}
}

void CEnemy::UpdateDirection(glm::vec2 playerPos)
{
	m_direction = playerPos - m_pos;
}

glm::mat4 CEnemy::GetModel()
{
	const glm::vec3 eye = { 0, 0, 0 };
	const glm::vec3 center = { m_direction.x, 0, m_direction.y };
	const glm::vec3 up = { 0, -1, 0 };

	glm::mat4 transform = glm::translate(glm::mat4(), { m_pos.x, 0.f, m_pos.y });
	glm::mat4 directionMatrix = glm::lookAt(eye, center, up);
	return transform * directionMatrix;
}

bool CEnemy::GetAlive()
{
	return m_alive;
}

void CEnemy::SetAlive(bool alive)
{
	m_alive = alive;
}