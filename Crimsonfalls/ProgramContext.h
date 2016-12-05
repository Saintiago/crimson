#pragma once
#include "libchapter3.h"
#include <string>

class CProgramContext
{
public:
    struct SLightSource
    {
        glm::vec4 position;
        glm::vec4 diffuse;
        glm::vec4 specular;
    };

	CProgramContext();

    void Use();
    CVertexAttribute GetPositionAttr()const;
    CVertexAttribute GetNormalAttr()const;
    CVertexAttribute GetTexCoordAttr()const;

    const glm::mat4 &GetModel()const;
    const glm::mat4 &GetView()const;
    const glm::mat4 &GetProjection()const;
    const SLightSource &GetLight0()const;

    void SetModel(const glm::mat4 &value);
    void SetView(const glm::mat4 &value);
    void SetProjection(const glm::mat4 &value);
    void SetLight0(const SLightSource &source);
	int LoadTexture(const std::string texturePath);
	void SwitchShaderTextureSlot(int slot);

private:
    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::mat4 m_normalModelView;
    SLightSource m_light0;

    CShaderProgram m_program;
	CTexture2DLoader m_loader;
	int m_currentTextureSlot = 1;
	std::vector<CTexture2DUniquePtr> m_textures;

	void SwitchGlTextureSlot(int slot);
};
