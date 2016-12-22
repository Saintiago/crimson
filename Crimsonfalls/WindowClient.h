#pragma once
#include "libchapter3.h"
#include "Arena.h"
#include "Player.h"
#include "EnemySystem.h"
#include "BulletSystem.h"
#include "ProgramContext.h"
#include "Renderer3D.h"
#include <vector>

class CWindowClient
        : public CAbstractWindowClient
{
public:
    CWindowClient(CWindow &window);

protected:
    // IWindowClient interface
    void OnUpdateWindow(float deltaSeconds) override;
    void OnDrawWindow() override;
    void OnKeyDown(const SDL_KeyboardEvent &) override;
    void OnKeyUp(const SDL_KeyboardEvent &) override;
	void OnMouseMotion(const glm::vec2 &pos) override;
	void OnMouseDown(const SDL_MouseButtonEvent &) override;
	void OnMouseUp(const SDL_MouseButtonEvent &) override;

private:
    void SetupView(const glm::ivec2 &size);
    void SetupLight0();
	bool CheckPlayerEnemiesCollisions();
	bool CheckBulletsEnemiesCollisions();
	bool GetArenaCoord(glm::vec2 mousePos, glm::vec2 & clickCoord);

	CProgramContext m_programContext;
    // Данный VAO будет объектом по-умолчанию.
    // Его привязка должна произойти до первой привязки VBO.
    //  http://stackoverflow.com/questions/13403807/
    CArrayObject m_defaultVAO;

    CArena m_arena;
	PlayerPtr m_player;
	CEnemySystem m_enemies;
	CBulletSystem m_bullets;
	int m_difficulty = 0;

    CCamera m_camera;
    CDirectedLightSource m_sunlight;
    
	float m_timeToSpawnEnemy = 0;

	RenderDataVector m_renderList;
};