#include "stdafx.h"
#include "Player.h"
#include "Renderer3D.h"

namespace
{
	const float LINEAR_MOVE_SPEED = 2.0f;

	glm::vec3 GetNormal(const float x, const float y)
	{
		glm::vec3 v1 = { x, y, 0 };
		glm::vec3 v2 = { x + 0.1f, y, 0 };
		glm::vec3 v3 = { x, y + 0.1f, 0 };
		return glm::normalize(glm::cross(v2 - v1, v3 - v1));
	}

	struct CPlayerTesselator : SMeshDataP3NT2
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

	bool ShouldTrackKeyPressed(const SDL_Keysym &key)
	{
		switch (key.sym)
		{
		case SDLK_w:
		case SDLK_a:
		case SDLK_s:
		case SDLK_d:
			return true;
		}
		return false;
	}

	glm::vec2 GetMoveDirection(std::set<unsigned> & keysPressed)
	{
		glm::vec2 direction = { 0, 0 };
		if (keysPressed.count(SDLK_UP) || keysPressed.count(SDLK_w))
		{
			direction.y -= 1;
		}
		if (keysPressed.count(SDLK_DOWN) || keysPressed.count(SDLK_s))
		{
			direction.y += 1;;
		}
		if (keysPressed.count(SDLK_LEFT) || keysPressed.count(SDLK_a))
		{
			direction.x -= 1;;
		}
		if (keysPressed.count(SDLK_RIGHT) || keysPressed.count(SDLK_d))
		{
			direction.x += 1;;
		}
		return direction;
	}
}

CPlayer::CPlayer(int textureSlot, float radius)
	: CUnit(GetTesselator(), textureSlot, radius)
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::Update(float dt)
{
	SetPosition(dt);
}

glm::mat4 CPlayer::GetModel()
{
	const glm::vec3 eye = { 0, 0, 0 };
	const glm::vec3 center = { m_turretDirection.x, 0, m_turretDirection.y };
	const glm::vec3 up = { 0, -1, 0 };

	glm::mat4 transform = glm::translate(glm::mat4(), { m_pos.x, 0.f, m_pos.y });
	glm::mat4 directionMatrix = glm::lookAt(eye, center, up);
	return transform * directionMatrix;

}

SMeshDataP3NT2 CPlayer::GetTesselator()
{
	CPlayerTesselator tesselator;
	tesselator.Tesselate();
	return tesselator;
}

bool CPlayer::OnKeyDown(const SDL_KeyboardEvent &event)
{
	if (ShouldTrackKeyPressed(event.keysym))
	{
		m_keysPressed.insert(unsigned(event.keysym.sym));
		return true;
	}
	return false;
}

bool CPlayer::OnKeyUp(const SDL_KeyboardEvent &event)
{
	if (ShouldTrackKeyPressed(event.keysym))
	{
		m_keysPressed.erase(unsigned(event.keysym.sym));
		return true;
	}
	return false;
}

bool CPlayer::OnMouseMotion(const CRay &ray)
{
	//  все спрайты лежат в плоскости Oxz.
	CPlane plane({ 1, 0, 1 }, { 1, 0, 0 }, { 0, 0, 1 });
	SRayIntersection intersection;

	if (!plane.Hit(ray, intersection))
	{
		return false;
	}

	const glm::vec3 hitPoint3D = intersection.m_point;
	const glm::vec2 hitPoint(hitPoint3D.x, hitPoint3D.z);

	m_mousePos = hitPoint;
	
	SetTurretAngle();

	return true;
}

void CPlayer::SetTurretAngle()
{
	glm::vec2 turretDirection = m_mousePos - m_pos;
	
	if (glm::length(turretDirection) > 0)
	{
		m_turretDirection = glm::normalize(turretDirection);
	}
}

void CPlayer::SetPosition(float dt)
{
	glm::vec2 direction = GetMoveDirection(m_keysPressed);
	if (glm::length(direction) > 0)
	{
		m_pos += glm::normalize(direction) * LINEAR_MOVE_SPEED * dt;
		m_pos.x = glm::clamp(m_pos.x, -(m_arenaSize.x / 2), (m_arenaSize.x / 2));
		m_pos.y = glm::clamp(m_pos.y, -(m_arenaSize.y / 2), (m_arenaSize.y / 2));
	}
}

void CPlayer::SetArenaSize(glm::vec2 size)
{
	m_arenaSize = size;
}
