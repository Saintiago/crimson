#include "stdafx.h"
#include "Arena.h"
#include <algorithm>

CArena::CArena(float size)
	: m_size(size)
{
}

void CArena::Update(float dt)
{
}

glm::vec2 CArena::GetSize()
{
	return m_size;
}

glm::vec2 CArena::GetEntryPoint()
{
	int edge = rand() % 4 + 1;
	float xCoordMax = (m_size.x / 2);
	float yCoordMax = (m_size.y / 2);
	float xCoordRand = ((rand() % (int)(m_size.x * 1000)) - ((m_size.x / 2) * 1000)) / 1000;
	float yCoordRand = ((rand() % (int)(m_size.y * 1000)) - ((m_size.y / 2) * 1000)) / 1000;

	if (edge == 1)
	{
		return{ -xCoordMax, yCoordRand };
	}
	if (edge == 2)
	{
		return{ xCoordRand, -yCoordMax };
	}
	if (edge == 3)
	{
		return{ xCoordMax, yCoordRand };
	}
	if (edge == 4)
	{
		return{ xCoordRand, yCoordMax };
	}
}