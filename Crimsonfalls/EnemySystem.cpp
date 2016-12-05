#include "stdafx.h"
#include "EnemySystem.h"

CEnemySystem::CEnemySystem(int textureSlot)
	: m_textureSlot(textureSlot)
{
}


CEnemySystem::~CEnemySystem()
{
}

void CEnemySystem::SpawnEnemy(const glm::vec2 pos = { 0,0 })
{
	EnemyPtr enemy = std::make_shared<CEnemy>(m_textureSlot, 0.2f, pos);
	m_enemies.push_back(enemy);
}

void CEnemySystem::Populate(int particlesCount)
{
	for (int i = 0; i < particlesCount; i++)
	{
		float coordX = (float)(rand() % 100) / 50;
		float coordY = (float)(rand() % 100) / 50;
		SpawnEnemy({ coordX, coordY });
	}
}

void CEnemySystem::SetPlayerPos(glm::vec2 playerPos)
{
	m_playerPos = playerPos;
}

void CEnemySystem::Update(float dt)
{
	for (auto & enemy : m_enemies)
	{
		enemy->UpdateDirection(m_playerPos);
		enemy->Update(dt);
	}
}

void CEnemySystem::Draw(IRenderer3D &renderer)const
{
	for (auto & enemy : m_enemies)
	{
		enemy->Draw(renderer);
	}
}

std::vector<EnemyPtr> CEnemySystem::GetEnemies()
{
	return m_enemies;
}
