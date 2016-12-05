#pragma once

#include <vector>
#include "Enemy.h"
#include <glm/vec2.hpp>
#include <memory>

typedef std::shared_ptr<CEnemy> EnemyPtr;

class CEnemySystem
{
public:
	CEnemySystem(int textureSlot);
	~CEnemySystem();

	void SpawnEnemy(const glm::vec2 pos);
	void Populate(int particlesCount);
	void SetPlayerPos(glm::vec2 playerPos);

	void Update(float dt);
	std::vector<EnemyPtr> GetEnemies();
	void Draw(IRenderer3D &renderer)const;

private:

	std::vector<EnemyPtr> m_enemies;
	glm::vec2 m_playerPos;
	int m_textureSlot;
};

