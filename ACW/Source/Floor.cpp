#include "Floor.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"

Floor::Floor()
{	
	glm::vec3& colour = GetColour();
	colour.x = 0.6f;
	colour.y = 0.3f;
	colour.z = 0.0f;
}

void Floor::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Floor::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}
