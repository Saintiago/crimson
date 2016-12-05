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

}

CWindowClient::CWindowClient(CWindow &window)
    : CAbstractWindowClient(window)
    , m_defaultVAO(CArrayObject::do_bind_tag())
    , m_arena(m_programContext.LoadTexture("res/img/dirt.jpg"))
	, m_player(m_programContext.LoadTexture("res/img/steel.jpg"), 0.2f)
	, m_enemies(m_programContext.LoadTexture("res/img/flesh.jpg"))
    , m_camera(CAMERA_INITIAL_ROTATION, CAMERA_INITIAL_DISTANCE)
    , m_sunlight(GL_LIGHT0)
{
	m_player.SetArenaSize(m_arena.GetSize());

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
	m_player.Update(deltaSeconds);

	m_enemies.SetPlayerPos(m_player.GetPosition());
	m_enemies.Update(deltaSeconds);

	if (CheckPlayerEnemiesCollisions()
		&& MessageBox(NULL, L"Congratulations, you have been killed", L"Game Over", MB_ICONINFORMATION || MB_OK) == (IDOK || IDCANCEL))
	{
		exit(0);
	}

	m_timeToSpawnEnemy -= deltaSeconds;
	if (m_timeToSpawnEnemy < 0)
	{
		m_enemies.SpawnEnemy(m_arena.GetEntryPoint());
		size_t enemiesCount = m_enemies.GetEnemies().size();
		m_timeToSpawnEnemy = glm::clamp(ENEMY_SPAWN_TIME - ((float)enemiesCount / 10), .0f, ENEMY_SPAWN_TIME);
	}
}

bool CWindowClient::CheckPlayerEnemiesCollisions()
{
	glm::vec2 playerPos = m_player.GetPosition();
	float playerRadius = m_player.GetRadius();

	for (const auto & enemy : m_enemies.GetEnemies())
	{
		float collisionDistance = enemy->GetRadius() + playerRadius;
		
		if (glm::length(playerPos - enemy->GetPosition()) <= collisionDistance)
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

	CRenderer3D renderer(m_programContext);

	m_programContext.SetModel(m_arena.GetModel());
	int textureSlot = m_arena.GetTextureSlot();
	m_programContext.SwitchShaderTextureSlot(textureSlot);
	m_arena.Draw(renderer);

	m_programContext.SetModel(m_player.GetModel());
	textureSlot = m_player.GetTextureSlot();
	m_programContext.SwitchShaderTextureSlot(textureSlot);
	m_player.Draw(renderer);

	for (auto & enemy : m_enemies.GetEnemies())
	{
		m_programContext.SetModel(enemy->GetModel());
		textureSlot = enemy->GetTextureSlot();
		m_programContext.SwitchShaderTextureSlot(enemy->GetTextureSlot());
		enemy->Draw(renderer);
	}
}

void CWindowClient::OnKeyDown(const SDL_KeyboardEvent &event)
{
    m_camera.OnKeyDown(event);
	m_player.OnKeyDown(event);
}

void CWindowClient::OnKeyUp(const SDL_KeyboardEvent &event)
{
    m_camera.OnKeyUp(event);
	m_player.OnKeyUp(event);
}

void CWindowClient::SetupView(const glm::ivec2 &size)
{
	const mat4 view = m_camera.GetViewTransform(m_player.GetPosition());

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
	// Вычисляем позицию точки в нормализованных координатах окна,
	//  то есть на диапазоне [-1; 1].
	// Также переворачиваем координату "y",
	//  т.к. OpenGL считает нулевым нижний левый угол окна,
	//  а все оконные системы - верхний левый угол.
	const glm::ivec2 winSize = GetWindow().GetWindowSize();
	const glm::vec2 halfWinSize = 0.5f * glm::vec2(winSize);
	const glm::vec2 invertedPos(pos.x, winSize.y - pos.y);
	const glm::vec2 normalizedPos = (invertedPos - halfWinSize) / halfWinSize;

	// Вычисляем матрицу обратного преобразования
	//  поскольку поле игры не имеет своей трансформации,
	//  мы берём матрицу камеры в качестве ModelView-матрицы
	const glm::mat4 mvMat = m_camera.GetViewTransform(m_player.GetPosition());
	const glm::mat4 projMat = GetProjectionMatrix(winSize);
	const glm::mat4 inverse = glm::inverse(projMat * mvMat);

	// В нормализованном пространстве глубина изменяется от -1 до +1.
	// Вычисляем начало и конец отрезка, проходящего через
	//  нормализованное пространство насквозь.
	const glm::vec3 start = TransformPoint(glm::vec3(normalizedPos, -1.f), inverse);
	const glm::vec3 end = TransformPoint(glm::vec3(normalizedPos, +1.f), inverse);

	m_player.OnMouseMotion(CRay(start, end - start));
}
