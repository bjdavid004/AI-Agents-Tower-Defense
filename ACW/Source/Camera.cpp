#include "Camera.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"

void Camera::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Camera::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}
