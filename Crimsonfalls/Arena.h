#pragma once

#include <functional>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Unit.h"

class CArena : public CUnit
{
public:

	glm::vec2 GetEntryPoint();
	glm::vec2 GetSize();
	CArena(float size);
	void Update(float dt);

private:
	glm::vec2 m_size;
};
