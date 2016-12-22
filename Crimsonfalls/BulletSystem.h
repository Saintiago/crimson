#pragma once

#include <vector>
#include "Bullet.h"
#include <glm/vec2.hpp>
#include <memory>

typedef std::shared_ptr<CBullet> BulletPtr;

class CBulletSystem
{
public:
	CBulletSystem();
	~CBulletSystem();

	void PullTrigger();
	void ReleaseTrigger();

	void SpawnBullet();
	void SetPlayerPos(glm::vec2 playerPos);
	void SetTurretDirection(glm::vec2 playerPos);
	void SetArenaSize(glm::vec2 size);
	void RemoveDead();
	void KillBulletsThatCrossedBorder();

	void Update(float dt);
	std::vector<BulletPtr> GetBullets();
	void SetRenderData(RenderDataVector renderData);
	RenderDataVector GetSceneObjects()const;

private:

	std::vector<BulletPtr> m_bullets;
	glm::vec2 m_playerPos;
	glm::vec2 m_turretDirection;
	glm::vec2 m_arenaSize;
	bool m_triggerDown = false;
	float m_timeToReload = 0;
	RenderDataVector m_renderData;
};

