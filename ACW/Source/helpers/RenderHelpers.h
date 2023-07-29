#pragma once
#include "IRenderHelpers.h"

/*
OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based on the
OpenGL Shading Language (GLSL) specifications.
https://github.com/g-truc/glm
*/
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

class OpenGLRenderHelpers : public IRenderHelpers
{
private:
	unsigned int m_BufferIDs[3];
	unsigned int m_ShaderProgramID;

	unsigned int m_Width;
	unsigned int m_Height;

	int m_ColourLocation;
	int m_ModelLocation;
	int m_ViewLocation;
	int m_ProjectionLocation;
	int m_LightLocation;

	void SetupShader();

	void Init();
	void SetSize(unsigned int pWidth, unsigned int pHeight);

	void SetModel(glm::mat4) const;
	void SetView(glm::mat4) const;
	void SetProjection(glm::mat4) const;
	void SetColour(glm::vec3) const;
	void SetLightPosition(glm::vec3) const;

	void GLCheckError() const;

	void RenderCube(const glm::vec3& pColour, const glm::mat4 pModel) const;
	void RenderSphere(const glm::vec3& pColour, const glm::mat4 pModel) const;
	void RenderCylinder(const glm::vec3& pColour, const glm::mat4 pModel) const;
public:

	~OpenGLRenderHelpers();
	void Render(const GameEntity*) const;
	void Render(const Camera*) const;
	void Render(const Floor*) const;
	void Render(const Ball*) const;
	void Render(const Pipe*) const;
	void Render(const Agent*) const;
	void Render(const World*) const;
};