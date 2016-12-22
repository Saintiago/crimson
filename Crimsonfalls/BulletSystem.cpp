#include "stdafx.h"
#include "BulletSystem.h"

namespace
{
	const float RELOAD_TIME = 0.5f;
}

CBulletSystem::CBulletSystem()
{
}


CBulletSystem::~CBulletSystem()
{
}

void CBulletSystem::PullTrigger()
{
	m_triggerDown = true;
}

void CBulletSystem::ReleaseTrigger()
{
	m_triggerDown = false;
}

void CBulletSystem::SpawnBullet()
{
	BulletPtr bullet = std::make_shared<CBullet>(0.04f, m_playerPos, m_turretDirection);
	bullet->SetRenderData(m_renderData);
	m_bullets.push_back(bullet);
}

void CBulletSystem::SetPlayerPos(glm::vec2 playerPos)
{
	m_playerPos = playerPos;
}

void CBulletSystem::SetTurretDirection(glm::vec2 direction)
{
	m_turretDirection = direction;
}

void CBulletSystem::RemoveDead()
{
	for (unsigned i = 0; i < m_bullets.size(); ++i)
	{
		if (!m_bullets.at(i)->GetAlive())
		{
			m_bullets.erase(m_bullets.begin() + i);
		}
	}
}

void CBulletSystem::KillBulletsThatCrossedBorder()
{
	for (auto & bullet : m_bullets)
	{
		glm::vec2 bulletPos = bullet->GetPosition();
		if ((abs(bulletPos.x) >= (m_arenaSize.x / 2)) || (abs(bulletPos.y) >= (m_arenaSize.y / 2)))
		{
			bullet->SetAlive(false);
		}
	}
}

void CBulletSystem::Update(float deltaSeconds)
{
	KillBulletsThatCrossedBorder();
	RemoveDead();

	m_timeToReload -= deltaSeconds;
	if (m_triggerDown && m_timeToReload < 0)
	{
		SpawnBullet();
		m_timeToReload = RELOAD_TIME;
	}

	for (auto & bullet : m_bullets)
	{
		bullet->Update(deltaSeconds);
	}
}

void CBulletSystem::SetRenderData(RenderDataVector renderData)
{
	m_renderData = renderData;
}

RenderDataVector CBulletSystem::GetSceneObjects()const
{
	RenderDataVector sceneObjects;
	for (auto & bullet : m_bullets)
	{
		RenderDataVector bulletRenderData = bullet->GetRenderData();
		sceneObjects.insert(sceneObjects.end(), bulletRenderData.begin(), bulletRenderData.end());
	}
	return sceneObjects;
}

std::vector<BulletPtr> CBulletSystem::GetBullets()
{
	return m_bullets;
}

void CBulletSystem::SetArenaSize(glm::vec2 size)
{
	m_arenaSize = size;
}
