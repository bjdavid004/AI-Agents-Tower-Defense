#include "GuiHelpers.h"

/*
Dear ImGui is a bloat-free graphical user interface library for C++.
https://github.com/ocornut/imgui
*/
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../GameEntity.h"
#include "../Camera.h"

#include "../Pipe.h"
#include "../Ball.h"
#include "../World.h"
#include "../Agent.h"

void ImGuiHelpers::RenderGui(string pLabel, glm::vec2 pVec2) const
{
	pLabel = pLabel + "[" + std::to_string(pVec2.x) + "," + std::to_string(pVec2.y) + "]";
	ImGui::Text(pLabel.c_str());
}

void ImGuiHelpers::RenderGui(string pLabel, glm::vec2 * pVec2) const
{
	pLabel = pLabel + "[" + std::to_string(pVec2->x) + "," + std::to_string(pVec2->y) + "]";
	ImGui::Text(pLabel.c_str());
}

void ImGuiHelpers::RenderGui(string pLabel, glm::vec3 pVec3) const
{
	pLabel = pLabel + "[" + std::to_string(pVec3.x) + "," + std::to_string(pVec3.y) + "," + std::to_string(pVec3.z) + "]";
	ImGui::Text(pLabel.c_str());
}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void ImGuiHelpers::RenderGuiAsColour(string pLabel, glm::vec3* pColour) const
{
	ImGui::ColorEdit3(pLabel.c_str(), &(*pColour)[0]);
	ImGui::SameLine(); 
	HelpMarker(
		"Click on the color square to open a color picker.\n"
		"Click and hold to use drag and drop.\n"
		"Right-click on the color square to show options.\n"
		"CTRL+click on individual component to input value.\n");
}

void ImGuiHelpers::RenderGui(string pLabel, glm::vec3* pVec3) const
{
	ImGui::InputFloat3(pLabel.c_str(), &(*pVec3)[0]);
}

void ImGuiHelpers::RenderGui(GameEntity* pEntity) const
{
	std::string label = "Entity : " + std::to_string(pEntity->ID());
	ImGui::Text(label.c_str());
}

void ImGuiHelpers::RenderGui(Pipe* pPipe) const
{
	std::string label = "Pipe : " + std::to_string(pPipe->ID());
	ImGui::Text(label.c_str());
}

void ImGuiHelpers::RenderGui(Ball* pBall) const
{
	std::string label = "Ball : " + std::to_string(pBall->ID());
	ImGui::Text(label.c_str());
	RenderGui("Position " + std::to_string(pBall->ID()) + ":", pBall->GetPosition());
	RenderGui("Velocity " + std::to_string(pBall->ID()) + ":", pBall->GetVelocity());
}

void ImGuiHelpers::RenderGui(Camera* pCamera) const
{
	std::string label = "Camera : " + std::to_string(pCamera->ID());
	ImGui::Text(label.c_str());
	RenderGui("pos1:", pCamera->Position);
	RenderGui("pos2:", &pCamera->Position);
}

void ImGuiHelpers::RenderGui(Agent* pAgent) const
{
	std::string label = "Agent " + std::to_string(pAgent->ID());
	ImGui::Text(label.c_str());
	RenderGuiAsColour(label + " colour :", &pAgent->GetColour());
	std::string label2 = "Action " + std::to_string(pAgent->ID()) + ":" + pAgent->GetAgentActions();
	ImGui::Text(label2.c_str());
}

void ImGuiHelpers::RenderGui(World* pWorld) const
{
	std::string label = "World " + std::to_string(pWorld->ID());
	ImGui::Text(label.c_str());
}