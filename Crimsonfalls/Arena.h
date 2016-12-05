#pragma once

#include <functional>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Unit.h"

const float ARENA_SIZE = 8.0f;

class CArena : public CUnit
{
public:

	glm::vec2 GetEntryPoint();
	CArena(int textureSlot);
	void Update(float dt) override;
	glm::mat4 GetModel() override;
	SMeshDataP3NT2 GetTesselator() override;
	glm::vec2 GetSize();

private:
	glm::vec2 m_size = { ARENA_SIZE, ARENA_SIZE };
};
