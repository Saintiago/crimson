#pragma once

#include <vector>
#include "Enemy.h"
#include <glm/vec2.hpp>
#include <memory>

typedef std::shared_ptr<CEnemy> EnemyPtr;

class CEnemySystem
{
public:
	CEnemySystem();
	~CEnemySystem();

	void SpawnEnemy(const glm::vec2 pos);
	void SetPlayerPos(glm::vec2 playerPos);
	void RemoveDead();
	void SetRenderData(RenderDataVector renderData);

	void Update(float dt);
	std::vector<EnemyPtr> GetEnemies();
	RenderDataVector GetSceneObjects()const;

private:

	std::vector<EnemyPtr> m_enemies;
	glm::vec2 m_playerPos;
	RenderDataVector m_renderData;
};

