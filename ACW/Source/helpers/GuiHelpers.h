#pragma once
#include "IGuiHelpers.h"

#include "../glm/glm.hpp"
#include <string>

using std::string;

class ImGuiHelpers : public IGuiHelpers
{
	void RenderGui(string pLabel, glm::vec2) const;
	void RenderGui(string pLabel, glm::vec2*) const;
	void RenderGui(string pLabel, glm::vec3) const;
	void RenderGui(string pLabel, glm::vec3*) const;
	void RenderGuiAsColour(string pLabel, glm::vec3*) const;
	void RenderGui(GameEntity*) const;
	void RenderGui(Pipe*) const;
	void RenderGui(Ball*) const;
	void RenderGui(Camera*) const;
	void RenderGui(Agent*) const;
	void RenderGui(World*) const;
};