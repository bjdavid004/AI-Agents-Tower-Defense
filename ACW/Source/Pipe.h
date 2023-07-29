#pragma once
#include "GameEntity.h"
class Pipe :
    public GameEntity
{
public:
    Pipe();
    float m_BaseRadius;
    float m_LipRadius;
    float m_BaseHeight;
    float m_LipHeight;

    void Render(const IRenderHelpers&) const;
    void RenderGui(const IGuiHelpers&);

    glm::vec3 GetPosition() const
    {
        glm::mat4 model = GetModel();
        glm::vec3 translation(model[3][0], model[3][1], model[3][2]);
        return translation;
    }

    void SetPosition(glm::vec3 pPosition)
    {
        glm::mat4& model = GetModel();
        model[3][0] = pPosition.x;
        model[3][1] = pPosition.y;
        model[3][2] = pPosition.z;
    }
};

