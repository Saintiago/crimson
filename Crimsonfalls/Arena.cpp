#include "stdafx.h"
#include "Arena.h"
#include <algorithm>

namespace
{
	const float TILE_SIZE = 1.f;

	glm::vec3 GetNormal(const float x, const float y)
	{
		glm::vec3 v1 = { x, y, 0 };
		glm::vec3 v2 = { x + 0.1f, y, 0 };
		glm::vec3 v3 = { x, y + 0.1f, 0 };
		return glm::normalize(glm::cross(v2 - v1, v3 - v1));
	}

struct CArenaTesselator : SMeshDataP3NT2
{
public:
    static const unsigned MIN_PRECISION = 4;

    void Tesselate(float xSize, float ySize)
    {
        MakeVertexAttributes(xSize, ySize);
        MakeTriangleStripIndicies(xSize, ySize);
    }

private:
    void MakeVertexAttributes(float xSize, float ySize)
    {
        // вычисляем позиции вершин.
		for (float ci = -(xSize / 2); ci <= (xSize / 2); ++ci)
		{
			const float x = TILE_SIZE * float(ci);
			for (float ri = -(ySize / 2); ri <= (ySize / 2); ++ri)
			{
				const float y = TILE_SIZE * float(ri);

				SVertexP3NT2 vertex;
				vertex.position = { x, 0, y };

				// Нормаль к сфере - это нормализованный вектор радиуса к данной точке
				// Поскольку координаты центра равны 0, координаты вектора радиуса
				// будут равны координатам вершины.
				// Благодаря радиусу, равному 1, нормализация не требуется.
				vertex.normal = GetNormal(x, y);

				// Обе текстурные координаты должны плавно изменяться от 0 до 1,
				// натягивая прямоугольную картинку на тело вращения.
				// При UV-параметризации текстурными координатами будут u и v.
				vertex.texCoord = { x / 4, y / 4 };

				vertices.push_back(vertex);
			}
		}
    }

    void MakeTriangleStripIndicies(float xSize, float ySize)
    {
        indicies.clear();
        indicies.reserve(xSize * ySize * 2);
        // вычисляем индексы вершин.
        for (unsigned ci = 0; ci <= xSize; ++ci)
        {
            if (ci % 2 == 0)
            {
                for (unsigned ri = 0; ri <= ySize; ++ri)
                {
                    unsigned index = ci * ySize + ri;
                    indicies.push_back(index + ySize);
                    indicies.push_back(index);
                }
            }
            else
            {
                for (unsigned ri = ySize; ri <= ySize; --ri)
                {
                    unsigned index = ci * ySize + ri;
                    indicies.push_back(index);
                    indicies.push_back(index + ySize);
                }
            }
        }
    }
};
}

CArena::CArena(int textureSlot)
	: CUnit(GetTesselator(), textureSlot)
{
}

void CArena::Update(float dt)
{

}

SMeshDataP3NT2 CArena::GetTesselator()
{
	CArenaTesselator tesselator;
	tesselator.Tesselate(ARENA_SIZE, ARENA_SIZE);
	return tesselator;
}

glm::mat4 CArena::GetModel()
{
	return glm::mat4(1);
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

glm::vec2 CArena::GetSize()
{
	return m_size;
}
