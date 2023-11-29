#define IMGUI_DEFINE_MATH_OPERATORS
#include "gui.h"
#include "../hook/hook.h"
#include "../../dx9/dx9.h"
#include "../../cheats/aim/aim.h"
#include "../../cheats/vis/vis.h"
#include "../../cheats/misc/misc.h"
#include "../../sdk/interfaces.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

void gui::shotdown()
{
    dx9::open = false;
}

void gui::render()
{
    if (GetAsyncKeyState(VK_INSERT) & 1)
    {
        if (!ImGui::GetCurrentContext())
        {
            return;
        }

        dx9::open = !dx9::open;

    }

    renderGUI();
}

void gui::renderGUI()
{

    ImGuiIO &IO = ImGui::GetIO();
    IO.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    if (!dx9::open)
    {
        return;
    }

    ImGui::SetNextWindowPos(IO.DisplaySize / 2.f, ImGuiCond_Once, {0.5f, 0.5f});
    ImGui::SetWindowSize({800, 600});
    ImGui::Begin("Menu", &dx9::open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::BeginTabBar("tabs");

	if (ImGui::BeginTabItem("Aim")) {
		ImGui::Checkbox("TriggerBot", &aim::enableTriggerBot);
		ImGui::Checkbox("AimBot", &aim::enableAimBot);
		ImGui::Checkbox("SilentAimBot", &aim::enableSilentAimBot);
		ImGui::Checkbox("RecoilControl", &aim::enableRC);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Visuals")) {
		ImGui::Checkbox("AntiFlash", &vis::antiflash);
		ImGui::Checkbox("GlowESP", &vis::enableGlowESP);
		ImGui::Checkbox("BoxESP", &vis::enableBoxESP);
		ImGui::Checkbox("SkeletonESP", &vis::enableSkeletonESP);
		ImGui::Checkbox("Chams", &vis::enableChams);
		ImGui::Checkbox("SnapLine", &vis::enableTraceLine);
		ImGui::SliderInt("FOV", &vis::FOV, -180, 180);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Misc")) {
        ImGui::Checkbox("Bhop", &misc::bhop);
		ImGui::Checkbox("Radar", &vis::enableRadar);
		ImGui::Checkbox("SkinChanger", &vis::enableskinChanger);

		if (ImGui::Button("Disable cheats"))
		{
			aim::enableTriggerBot = false;
			aim::enableAimBot = false;
			aim::enableSilentAimBot = false;
			aim::enableRC = false;
			misc::bhop = false;
			vis::antiflash = false;
			vis::enableGlowESP = false;
			vis::enableBoxESP = false;
			vis::enableSkeletonESP = false;
			vis::enableTraceLine = false;
			vis::FOV = 90;
			vis::enableRadar = false;
			vis::enableskinChanger = false;
		}

		ImGui::EndTabItem();
	}


	ImGui::EndTabBar();
    ImGui::End();
}

void gui::watermark()
{
    auto drawList = ImGui::GetBackgroundDrawList();
    char framerate[128];

    time_t rawtime;
    struct tm *timeinfo;

    char time_[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    std::time_t currentTime = std::time(nullptr);
    std::tm *timeInfo = std::localtime(&currentTime);

    std::ostringstream oss;
    oss << std::put_time(timeInfo, "%d %b %Y");

    std::string str_date = oss.str();
    strftime(time_, sizeof(time_), "%H:%M:%S", timeinfo);
    std::string str_time(time_);

    snprintf(framerate, IM_ARRAYSIZE(framerate), "CS:GO Client [%d]\nFPS: %d\n\n%s %s\n",
             interfaces::Get().engine->GetEngineBuildNumber(), static_cast<int>(ImGui::GetIO().Framerate), str_date.c_str(), str_time.c_str());

    drawList->AddText({17, 9}, IM_COL32(0, 0, 0, 255), framerate);
    drawList->AddText({16, 8}, IM_COL32(27, 227, 200, 255), framerate);
}