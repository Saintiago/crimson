#include "stdafx.h"
#include "Enemy.h"
#include "Renderer3D.h"

namespace
{
	const float LINEAR_MOVE_SPEED = 0.5f;

	glm::vec3 GetNormal(const float x, const float y)
	{
		glm::vec3 v1 = { x, y, 0 };
		glm::vec3 v2 = { x + 0.1f, y, 0 };
		glm::vec3 v3 = { x, y + 0.1f, 0 };
		return glm::normalize(glm::cross(v2 - v1, v3 - v1));
	}

	struct CEnemyTesselator : SMeshDataP3NT2
	{
	public:
		static const unsigned MIN_PRECISION = 4;

		void Tesselate()
		{
			MakeVertexAttributes();
			MakeTriangleStripIndicies();
		}

	private:
		void MakeVertexAttributes()
		{
			SVertexP3NT2 vertex1;
			SVertexP3NT2 vertex2;
			SVertexP3NT2 vertex3;
			SVertexP3NT2 vertex4;

			glm::vec3 v1 = { 0, 0, -0.4f };
			glm::vec3 v2 = { 0.2f, 0, 0.2f };
			glm::vec3 v3 = { -0.2f, 0, 0.2f };
			glm::vec3 v4 = { 0, -0.6f, 0.2f };
			glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

			vertex1.position = v1;
			vertex2.position = v2;
			vertex3.position = v3;
			vertex4.position = v4;

			vertex1.normal = normal;
			vertex2.normal = normal;
			vertex3.normal = normal;
			vertex4.normal = normal;

			vertex1.texCoord = { 1.f - v1.x, v1.y };
			vertex2.texCoord = { 1.f - v2.x, v2.y };
			vertex3.texCoord = { 1.f - v3.x, v3.y };
			vertex4.texCoord = { 1.f - v4.x, v4.y };

			vertices.push_back(vertex1);
			vertices.push_back(vertex2);
			vertices.push_back(vertex3);
			vertices.push_back(vertex4);
		}

		void MakeTriangleStripIndicies()
		{
			indicies.clear();
			indicies.reserve(12);

			indicies.push_back(1);
			indicies.push_back(2);
			indicies.push_back(0);

			indicies.push_back(3);
			indicies.push_back(0);
			indicies.push_back(2);

			indicies.push_back(3);
			indicies.push_back(1);
			indicies.push_back(0);

			indicies.push_back(3);
			indicies.push_back(2);
			indicies.push_back(1);
		}
	};
}

CEnemy::CEnemy(int textureSlot, float radius, glm::vec2 pos)
	: CUnit(GetTesselator(), textureSlot, radius, pos)
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Update(float dt)
{
	SetPosition(dt);
}

SMeshDataP3NT2 CEnemy::GetTesselator()
{
	CEnemyTesselator tesselator;
	tesselator.Tesselate();
	return tesselator;
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