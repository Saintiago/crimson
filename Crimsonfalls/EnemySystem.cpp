#include "stdafx.h"
#include "EnemySystem.h"

CEnemySystem::CEnemySystem()
{
}


CEnemySystem::~CEnemySystem()
{
}

void CEnemySystem::SpawnEnemy(const glm::vec2 pos = { 0,0 })
{
	EnemyPtr enemy = std::make_shared<CEnemy>(0.2f, pos);

	RenderDataPtr enemyRender = std::make_shared<RenderData>(
		m_renderData.at(0)->mesh, 
		m_renderData.at(0)->model, 
		m_renderData.at(0)->textureSlot
	);

	enemy->SetRenderData({enemyRender});
	m_enemies.push_back(enemy);
}

void CEnemySystem::RemoveDead()
{
	for (unsigned i = 0; i < m_enemies.size(); ++i)
	{
		if (!m_enemies.at(i)->GetAlive())
		{
			m_enemies.erase(m_enemies.begin() + i);
		}
	}
}

void CEnemySystem::SetRenderData(RenderDataVector renderData)
{
	m_renderData = renderData;
}

void CEnemySystem::SetPlayerPos(glm::vec2 playerPos)
{
	m_playerPos = playerPos;
}

void CEnemySystem::Update(float dt)
{
	RemoveDead();

	for (auto & enemy : m_enemies)
	{
		enemy->UpdateDirection(m_playerPos);
		enemy->Update(dt);
	}
}

RenderDataVector CEnemySystem::GetSceneObjects()const
{
	RenderDataVector sceneObjects;
	for (auto & enemy : m_enemies)
	{
		RenderDataVector enemyRenderData = enemy->GetRenderData();
		sceneObjects.insert(sceneObjects.end(), enemyRenderData.begin(), enemyRenderData.end());
	}
	return sceneObjects;
}

std::vector<EnemyPtr> CEnemySystem::GetEnemies()
{
	return m_enemies;
}
