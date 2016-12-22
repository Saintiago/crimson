#include "stdafx.h"
#include "WindowClient.h"
#include "Renderer3D.h"
#include <windows.h>

using glm::mat4;
using glm::vec3;

namespace
{

const float CAMERA_INITIAL_ROTATION = 3.f;
const float CAMERA_INITIAL_DISTANCE = 4;
const float ENEMY_SPAWN_TIME = 2;
const float ARENA_SIZE = 8.0f;
const float TILE_SIZE = 1.f;

glm::vec3 GetNormal(const float x, const float y)
{
	glm::vec3 v1 = { x, y, 0 };
	glm::vec3 v2 = { x + 0.1f, y, 0 };
	glm::vec3 v3 = { x, y + 0.1f, 0 };
	return glm::normalize(glm::cross(v2 - v1, v3 - v1));
}

void SetupOpenGLState()
{
    // включаем механизмы трёхмерного мира.
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}

glm::mat4 GetProjectionMatrix(const glm::ivec2 &size)
{
	// Матрица перспективного преобразования вычисляется функцией
	// glm::perspective, принимающей угол обзора, соотношение ширины
	// и высоты окна, расстояния до ближней и дальней плоскостей отсечения.
	const float fieldOfView = glm::radians(70.f);
	const float aspect = float(size.x) / float(size.y);
	const float zNear = 0.01f;
	const float zFar = 100.f;

	return glm::perspective(fieldOfView, aspect, zNear, zFar);
}

glm::vec3 TransformPoint(const glm::vec3 point, const glm::mat4 &transform)
{
	const glm::vec4 original(point, 1);
	glm::vec4 transformed = transform * original;
	transformed /= transformed.w;

	return glm::vec3(transformed);
}

bool CheckIfTwoUnitsCollided(UnitPtr u1, UnitPtr u2)
{
	float collisionDistance = u1->GetRadius() + u2->GetRadius();
	return glm::length(u1->GetPosition() - u2->GetPosition()) <= collisionDistance;
}

std::vector<SVertexP3NT2> MakeStubVertexAttributes(float coef)
{
	std::vector<SVertexP3NT2> vertices;

	SVertexP3NT2 vertex1;
	SVertexP3NT2 vertex2;
	SVertexP3NT2 vertex3;
	SVertexP3NT2 vertex4;

	glm::vec3 v1 = { 0, 0, -2.0f };
	glm::vec3 v2 = { 1.0f, 0, 1.0f };
	glm::vec3 v3 = { -1.0f, 0, 1.0f };
	glm::vec3 v4 = { 0, -3.0f, 1.0f };
	glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

	vertex1.position = v1 * coef;
	vertex2.position = v2 * coef;
	vertex3.position = v3 * coef;
	vertex4.position = v4 * coef;

	vertex1.normal = normal;
	vertex2.normal = normal;
	vertex3.normal = normal;
	vertex4.normal = normal;

	vertex1.texCoord = { 1.f - v1.x, v1.y };
	vertex2.texCoord = { 1.f - v2.x, v2.y };
	vertex3.texCoord = { 1.f - v3.x, v3.y };
	vertex4.texCoord = { 1.f - v4.x, v4.y };

	vertices.clear();
	vertices.push_back(vertex1);
	vertices.push_back(vertex2);
	vertices.push_back(vertex3);
	vertices.push_back(vertex4);

	return vertices;
}

std::vector<uint32_t> MakeStubTriangleStripIndicies()
{
	std::vector<uint32_t> indicies;
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

	return indicies;
}

std::vector<SVertexP3NT2> MakeArenaVertexAttributes()
{
	std::vector<SVertexP3NT2> vertices;

	// вычисляем позиции вершин.
	for (float ci = -(ARENA_SIZE / 2); ci <= (ARENA_SIZE / 2); ++ci)
	{
		const float x = TILE_SIZE * float(ci);
		for (float ri = -(ARENA_SIZE / 2); ri <= (ARENA_SIZE / 2); ++ri)
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
	return vertices;
}

std::vector<uint32_t> MakeArenaTriangleStripIndicies()
{
	std::vector<uint32_t> indicies;
	indicies.clear();
	indicies.reserve(ARENA_SIZE * ARENA_SIZE * 2);
	// вычисляем индексы вершин.
	for (unsigned ci = 0; ci <= ARENA_SIZE; ++ci)
	{
		if (ci % 2 == 0)
		{
			for (unsigned ri = 0; ri <= ARENA_SIZE; ++ri)
			{
				unsigned index = ci * ARENA_SIZE + ri;
				indicies.push_back(index + ARENA_SIZE);
				indicies.push_back(index);
			}
		}
		else
		{
			for (unsigned ri = ARENA_SIZE; ri <= ARENA_SIZE; --ri)
			{
				unsigned index = ci * ARENA_SIZE + ri;
				indicies.push_back(index);
				indicies.push_back(index + ARENA_SIZE);
			}
		}
	}
	return indicies;
}

}

CWindowClient::CWindowClient(CWindow &window)
    : CAbstractWindowClient(window)
    , m_defaultVAO(CArrayObject::do_bind_tag())
	, m_programContext()
    , m_arena(ARENA_SIZE)
    , m_camera(CAMERA_INITIAL_ROTATION, CAMERA_INITIAL_DISTANCE)
    , m_sunlight(GL_LIGHT0)
{

	SMeshDataP3NT2 enemyMesh = { MakeStubVertexAttributes(0.2f), MakeStubTriangleStripIndicies() };
	SMeshDataP3NT2 bulletMesh = { MakeStubVertexAttributes(0.08f), MakeStubTriangleStripIndicies() };
	SMeshDataP3NT2 arenaMesh = { MakeArenaVertexAttributes(), MakeArenaTriangleStripIndicies() };
	SMeshDataP3NT2 tankBaseMesh = { MakeStubVertexAttributes(0.3f), MakeStubTriangleStripIndicies() };
	SMeshDataP3NT2 tankTurretMesh = { MakeStubVertexAttributes(0.2f), MakeStubTriangleStripIndicies() };

	m_player = std::make_shared<CPlayer>(0.3f);
	m_player->SetArenaSize({ ARENA_SIZE, ARENA_SIZE });
	m_bullets.SetArenaSize({ ARENA_SIZE, ARENA_SIZE });

	RenderDataPtr arenaRender = std::make_shared<RenderData>(arenaMesh, glm::mat4(1), m_programContext.LoadTexture("res/img/dirt.jpg"));
	m_arena.SetRenderData({ arenaRender });
	RenderDataPtr enemyRender = std::make_shared<RenderData>(enemyMesh, glm::mat4(), m_programContext.LoadTexture("res/img/flesh.jpg"));
	m_enemies.SetRenderData({ enemyRender });
	RenderDataPtr tankBaseRender = std::make_shared<RenderData>(tankBaseMesh, glm::mat4(), m_programContext.LoadTexture("res/img/steel.jpg"));
	RenderDataPtr tankTurretRender = std::make_shared<RenderData>(tankTurretMesh, glm::mat4(), m_programContext.LoadTexture("res/img/steel.jpg"));
	m_player->SetRenderData({ tankBaseRender,  tankTurretRender });
	RenderDataPtr bulletRender = std::make_shared<RenderData>(bulletMesh, glm::mat4(), m_programContext.LoadTexture("res/img/brass.jpg"));
	m_bullets.SetRenderData({ bulletRender });

    const glm::vec3 SUNLIGHT_DIRECTION = {-1.f, 0.2f, 0.7f};
    const glm::vec4 WHITE_RGBA = {1, 1, 1, 1};
    const glm::vec4 BLACK_RGBA = {0, 0, 0, 1};

    window.SetBackgroundColor(BLACK_RGBA);
    SetupOpenGLState();

    m_sunlight.SetDirection(SUNLIGHT_DIRECTION);
    m_sunlight.SetDiffuse(WHITE_RGBA);
    m_sunlight.SetSpecular(WHITE_RGBA);
}

void CWindowClient::OnUpdateWindow(float deltaSeconds)
{
    m_camera.Update(deltaSeconds);
	m_player->Update(deltaSeconds);

	m_enemies.SetPlayerPos(m_player->GetPosition());
	m_enemies.Update(deltaSeconds);

	m_bullets.SetPlayerPos(m_player->GetPosition());
	m_bullets.SetTurretDirection(m_player->GetTurretDirection());
	m_bullets.Update(deltaSeconds);

	if (CheckPlayerEnemiesCollisions()
		&& MessageBox(NULL, L"Congratulations, you have been killed", L"Game Over", MB_ICONINFORMATION || MB_OK) == (IDOK || IDCANCEL))
	{
		exit(0);
	}

	CheckBulletsEnemiesCollisions();

	m_timeToSpawnEnemy -= deltaSeconds;
	if (m_timeToSpawnEnemy < 0)
	{
		m_enemies.SpawnEnemy(m_arena.GetEntryPoint());
		m_difficulty++;
		m_timeToSpawnEnemy = glm::clamp(ENEMY_SPAWN_TIME - ((float)m_difficulty / 10), .2f, ENEMY_SPAWN_TIME);
	}

	std::cerr << m_difficulty << std::endl;
}

bool CWindowClient::CheckBulletsEnemiesCollisions()
{
	for (unsigned i = 0; i < m_enemies.GetEnemies().size(); ++i)
	{
		for (const auto & bullet : m_bullets.GetBullets())
		{
			if (CheckIfTwoUnitsCollided(m_enemies.GetEnemies().at(i), bullet))
			{
				m_enemies.GetEnemies().at(i)->SetAlive(false);
				bullet->SetAlive(false);
			}
		}
	}

	return false;
}

bool CWindowClient::CheckPlayerEnemiesCollisions()
{
	for (const auto & enemy : m_enemies.GetEnemies())
	{
		if (CheckIfTwoUnitsCollided(m_player, enemy))
		{
			return true;
		}
	}
	return false;
}

void CWindowClient::OnDrawWindow()
{
    SetupView(GetWindow().GetWindowSize());
    SetupLight0();

	m_renderList.clear();

	RenderDataVector arenaRenderData = m_arena.GetRenderData();
	m_renderList.insert(m_renderList.end(), arenaRenderData.begin(), arenaRenderData.end());

	RenderDataVector enemyRenderData = m_enemies.GetSceneObjects();
	m_renderList.insert(m_renderList.end(), enemyRenderData.begin(), enemyRenderData.end());

	RenderDataVector bulletRenderData = m_bullets.GetSceneObjects();
	m_renderList.insert(m_renderList.end(), bulletRenderData.begin(), bulletRenderData.end());

	RenderDataVector playerRenderData = m_player->GetRenderData();
	m_renderList.insert(m_renderList.end(), playerRenderData.begin(), playerRenderData.end());

	if (m_renderList.size() > 3)
	{
		m_renderList = m_renderList;
	}

	CRenderer3D renderer(m_programContext);
	for (auto & sceneObject : m_renderList)
	{
		renderer.Draw(sceneObject);
	}
}

void CWindowClient::OnKeyDown(const SDL_KeyboardEvent &event)
{
    m_camera.OnKeyDown(event);
	m_player->OnKeyDown(event);
}

void CWindowClient::OnKeyUp(const SDL_KeyboardEvent &event)
{
    m_camera.OnKeyUp(event);
	m_player->OnKeyUp(event);
}

void CWindowClient::SetupView(const glm::ivec2 &size)
{
	const mat4 view = m_camera.GetViewTransform(m_player->GetPosition());

    // Матрица перспективного преобразования вычисляется функцией
    // glm::perspective, принимающей угол обзора, соотношение ширины
    // и высоты окна, расстояния до ближней и дальней плоскостей отсечения.
    const float fieldOfView = glm::radians(70.f);
    const float aspect = float(size.x) / float(size.y);
    const float zNear = 0.01f;
    const float zFar = 100.f;
    const mat4 proj = glm::perspective(fieldOfView, aspect, zNear, zFar);

    glViewport(0, 0, size.x, size.y);

    m_programContext.SetView(view);
    m_programContext.SetProjection(proj);
}

void CWindowClient::SetupLight0()
{
    CProgramContext::SLightSource light0;
    light0.specular = m_sunlight.GetSpecular();
    light0.diffuse = m_sunlight.GetDiffuse();
    light0.position = m_sunlight.GetUniformPosition();
    m_programContext.SetLight0(light0);
}

void CWindowClient::OnMouseMotion(const glm::vec2 &pos)
{
	glm::vec2 clickCoord;
	if (!GetArenaCoord(pos, clickCoord))
	{
		return;
	}
	m_player->OnMouseMotion(clickCoord);
}

void CWindowClient::OnMouseDown(const SDL_MouseButtonEvent & mouseEvent)
{
	if (mouseEvent.button == SDL_BUTTON_LEFT)
	{
		m_bullets.PullTrigger();
	}
}

void CWindowClient::OnMouseUp(const SDL_MouseButtonEvent & mouseEvent)
{
	if (mouseEvent.button == SDL_BUTTON_LEFT)
	{
		m_bullets.ReleaseTrigger();
	}
}

bool CWindowClient::GetArenaCoord(glm::vec2 mousePos, glm::vec2 & clickCoord)
{
	// Вычисляем позицию точки в нормализованных координатах окна,
	//  то есть на диапазоне [-1; 1].
	// Также переворачиваем координату "y",
	//  т.к. OpenGL считает нулевым нижний левый угол окна,
	//  а все оконные системы - верхний левый угол.
	const glm::ivec2 winSize = GetWindow().GetWindowSize();
	const glm::vec2 halfWinSize = 0.5f * glm::vec2(winSize);
	const glm::vec2 invertedPos(mousePos.x, winSize.y - mousePos.y);
	const glm::vec2 normalizedPos = (invertedPos - halfWinSize) / halfWinSize;

	// Вычисляем матрицу обратного преобразования
	//  поскольку поле игры не имеет своей трансформации,
	//  мы берём матрицу камеры в качестве ModelView-матрицы
	const glm::mat4 mvMat = m_camera.GetViewTransform(m_player->GetPosition());
	const glm::mat4 projMat = GetProjectionMatrix(winSize);
	const glm::mat4 inverse = glm::inverse(projMat * mvMat);

	// В нормализованном пространстве глубина изменяется от -1 до +1.
	// Вычисляем начало и конец отрезка, проходящего через
	//  нормализованное пространство насквозь.
	const glm::vec3 start = TransformPoint(glm::vec3(normalizedPos, -1.f), inverse);
	const glm::vec3 end = TransformPoint(glm::vec3(normalizedPos, +1.f), inverse);

	CRay ray(start, end - start);
	
	//  все спрайты лежат в плоскости Oxz.
	CPlane plane({ 1, 0, 1 }, { 1, 0, 0 }, { 0, 0, 1 });
	SRayIntersection intersection;

	if (!plane.Hit(ray, intersection))
	{
		return false;
	}

	const glm::vec3 hitPoint3D = intersection.m_point;
	clickCoord = { hitPoint3D.x, hitPoint3D.z };

	return true;
}
