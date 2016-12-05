#include "stdafx.h"
#include "ProgramContext.h"

namespace
{
glm::mat4 GetNormalMatrix(const glm::mat4 &modelView)
{
    return glm::transpose(glm::inverse(modelView));
}
}

CProgramContext::CProgramContext()
{
    const auto vertShader = CFilesystemUtils::LoadFileAsString("res/cloud_earth_robust.vert");
    const auto fragShader = CFilesystemUtils::LoadFileAsString("res/cloud_earth_robust.frag");
    m_program.CompileShader(vertShader, ShaderType::Vertex);
    m_program.CompileShader(fragShader, ShaderType::Fragment);
    m_program.Link();
}

void CProgramContext::Use()
{
	m_program.Use();
    
	m_program.FindUniform("view") = m_view;
	m_program.FindUniform("projection") = m_projection;

	m_program.FindUniform("light0.position") = m_light0.position;
	m_program.FindUniform("light0.diffuse") = m_light0.diffuse;
	m_program.FindUniform("light0.specular") = m_light0.specular;
}

CVertexAttribute CProgramContext::GetPositionAttr() const
{
    return m_program.FindAttribute("vertex");
}

CVertexAttribute CProgramContext::GetNormalAttr() const
{
    return m_program.FindAttribute("normal");
}

CVertexAttribute CProgramContext::GetTexCoordAttr() const
{
    return m_program.FindAttribute("textureUV");
}

const glm::mat4 &CProgramContext::GetModel() const
{
    return m_model;
}

const glm::mat4 &CProgramContext::GetView() const
{
    return m_view;
}

const glm::mat4 &CProgramContext::GetProjection() const
{
    return m_projection;
}

const CProgramContext::SLightSource &CProgramContext::GetLight0() const
{
    return m_light0;
}

void CProgramContext::SetModel(const glm::mat4 &value)
{
    m_model = value;
	const glm::mat4 mv = m_view * m_model;
	m_program.FindUniform("modelView") = mv;
	m_program.FindUniform("normalModelView") = GetNormalMatrix(mv);
}

void CProgramContext::SetView(const glm::mat4 &value)
{
    m_view = value;
}

void CProgramContext::SetProjection(const glm::mat4 &value)
{
    m_projection = value;
}

void CProgramContext::SetLight0(const CProgramContext::SLightSource &source)
{
    m_light0 = source;
}

int CProgramContext::LoadTexture(const std::string texturePath)
{
	SwitchGlTextureSlot(m_currentTextureSlot);
	m_textures.push_back(m_loader.Load(texturePath));
	m_textures.at(m_currentTextureSlot)->Bind();
	return m_currentTextureSlot++;
}

void CProgramContext::SwitchGlTextureSlot(int slot)
{
	if (slot == 0)	glActiveTexture(GL_TEXTURE0);
	if (slot == 1)	glActiveTexture(GL_TEXTURE1);
	if (slot == 2)	glActiveTexture(GL_TEXTURE2);
	if (slot == 3)	glActiveTexture(GL_TEXTURE3);
	if (slot == 4)	glActiveTexture(GL_TEXTURE4);
	if (slot == 5)	glActiveTexture(GL_TEXTURE5);
	if (slot == 6)	glActiveTexture(GL_TEXTURE6);
	if (slot == 7)	glActiveTexture(GL_TEXTURE7);
}

void CProgramContext::SwitchShaderTextureSlot(int slot)
{
	m_program.FindUniform("colormap") = slot; // GL_TEXTURE<slot>
}