#pragma once
#include "libchapter3.h"
#include "Arena.h"
#include "Player.h"
#include "EnemySystem.h"
#include "ProgramContext.h"
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

private:
    void SetupView(const glm::ivec2 &size);
    void SetupLight0();
	bool CheckPlayerEnemiesCollisions();

    // Данный VAO будет объектом по-умолчанию.
    // Его привязка должна произойти до первой привязки VBO.
    //  http://stackoverflow.com/questions/13403807/
    CArrayObject m_defaultVAO;

    CArena m_arena;
	CPlayer m_player;
	CEnemySystem m_enemies;
    CCamera m_camera;
    CDirectedLightSource m_sunlight;
    CProgramContext m_programContext;
	float m_timeToSpawnEnemy = 0;
};