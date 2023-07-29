#pragma once
#include "GameEntity.h"
class Floor :
    public GameEntity
{
    public:
        Floor();
        void Render(const IRenderHelpers&) const;
        void RenderGui(const IGuiHelpers&);
};

