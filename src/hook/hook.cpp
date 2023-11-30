#include "hook.h"
#include "../gui/gui.h"
#include "../../cheats/aim/aim.h"
#include "../../cheats/vis/vis.h"
#include "../../cheats/misc/misc.h"
#include "../../dx9/minhook/include/MinHook.h"
#include <intrin.h>
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_impl_dx9.h"

void hook::Init()
{
	dx9::Setup();
	if (MH_Initialize())
	{
		throw std::runtime_error("Unable to initialize minhook");
	}

	if (MH_CreateHook(VirtualFunction(dx9::device, 42), &EndScene, (void **)(&EndSceneOriginal)))
	{
		throw std::runtime_error("Unable to hook EndScene()");
	}

	if (MH_CreateHook(VirtualFunction(dx9::device, 16), &Reset, (void **)(&ResetOriginal)))
	{
		throw std::runtime_error("Unable to hook Reset()");
	}

	if (MH_CreateHook(VirtualFunction(interfaces::Get().g_ClientMode, 24), &hook::CreateMove, (void **)(&hook::OriginalCreateMove)))
	{
		throw std::runtime_error("Unable to hook CreateMove()");
	}

	if (MH_CreateHook(VirtualFunction(interfaces::Get().g_ClientMode, 44), &hook::DoPostScreenSpaceEffects, (void **)(&hook::DoPostScreenSpaceEffectsOriginal)))
	{
		throw std::runtime_error("Unable to hook DoPostScreenSpaceEffects()");
	}

	if (MH_CreateHook(VirtualFunction(interfaces::Get().g_Client, 37), &hook::FrameStageNotify, (void **)(&hook::FrameStageNotifyOriginal)))
	{
		throw std::runtime_error("Unable to hook FrameStageNotify()");
	}

	if (MH_CreateHook(VirtualFunction(interfaces::Get().panel, 41), &hook::PaintTraverse, (void **)(&hook::PaintTraverseOriginal)))
	{
		throw std::runtime_error("Unable to hook PaintTraverse()");
	}

	if (MH_CreateHook(VirtualFunction(interfaces::Get().studioRender, 29), &hook::DrawModel, (void **)(&hook::DrawModelOriginal)))
	{
		throw std::runtime_error("Unable to hook DrawModel()");
	}

	if (MH_EnableHook(MH_ALL_HOOKS))
	{
		throw std::runtime_error("Unable to enable hooks");
	}
}

void hook::Shutdown()
{
	gui::shotdown();
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
	dx9::Destroy();
}

long __stdcall hook::EndScene(IDirect3DDevice9 *device)
{
	static const auto address = _ReturnAddress();
	auto result = EndSceneOriginal(device, device);

	if (_ReturnAddress() == address)
	{
		return result;
	}

	if (!dx9::setup)
	{
		dx9::SetupMenu(device);
	}


	gui::render();
	
	return result;
}

HRESULT __stdcall hook::Reset(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *params)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
	return 0;
}

bool __stdcall hook::CreateMove(float frameTime, UserCmd *cmd)
{
	bool result = OriginalCreateMove(interfaces::Get().g_ClientMode, frameTime, cmd);

	if (!cmd || !cmd->commandNumber)
	{
		return result;
	}

	if (result)
		interfaces::Get().engine->SetViewAngles(cmd->viewAngles);

	if (misc::bhop)
		misc::enableBhop(cmd);
	if (aim::enableTriggerBot)
		aim::triggerBot(cmd);
	if (aim::enableSilentAimBot)
		aim::SilentAimBot(cmd);
	if (aim::enableAimBot)
		aim::AimBot();

	return false;
}

void __stdcall hook::DoPostScreenSpaceEffects(const void *viewSetup)
{
	DoPostScreenSpaceEffectsOriginal(interfaces::Get().g_ClientMode, viewSetup);
	if (vis::enableGlowESP)
		vis::GlowESP();
}

void __stdcall hook::FrameStageNotify(ClientFrameStage_t curStage)
{
	FrameStageNotifyOriginal(interfaces::Get().g_Client, curStage);
	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		if (vis::enableskinChanger)
			vis::skinChanger();
	}
}

void __stdcall hook::PaintTraverse(uintptr_t panel, bool forceRepaint, bool allowForce)
{
	if (panel == interfaces::Get().engineVGui->GetPanel(PANEL_TOOLS))
	{
		if (vis::enableTraceLine)
			vis::TraceLine();
		if (vis::enableBoxESP)
			vis::BoxESP();
		if (vis::enableSkeletonESP)
			vis::SkeletonESP();
	}

	PaintTraverseOriginal(interfaces::Get().panel, panel, forceRepaint, allowForce);
}

void __stdcall hook::DrawModel(void *result, const CDrawModelInfo &info, matrix3x4_t *bones, float *flexWeights, float *flexDelayedWeights, const Vector &modelOrigin, const std::int32_t flags)
{

	if (!interfaces::Get().engine->IsInGame())
	{
		DrawModelOriginal(interfaces::Get().studioRender, result, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
		return;
	}

	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = (interfaces::Get().EntityList->GetClientEntity(index));
	if (localPlayer && info.renderable && vis::enableChams)
	{
		auto entity = info.renderable->GetIClientUnknown()->GetBaseEntity();
		if (entity && entity->IsPlayer() && entity->Team() != localPlayer->Team())
		{
			static IMaterial *meterial = interfaces::Get().materialSystem->FindMaterial("debug/debugambientcube");

			float hidden[3] = {0.0117f, 0.f, 0.3882f};
			float visible[3] = {0.4588f, 0.96078f, 0.258823f};

			interfaces::Get().studioRender->SetAlphaModulation(1.f);
			meterial->SetMaterialVarFlag(IMaterial::IGNOREZ, true);
			interfaces::Get().studioRender->SetColorModulation(visible);
			interfaces::Get().studioRender->ForcedMaterialOverride(meterial);
			DrawModelOriginal(interfaces::Get().studioRender, result, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

			meterial->SetMaterialVarFlag(IMaterial::IGNOREZ, false);
			interfaces::Get().studioRender->SetColorModulation(hidden);
			interfaces::Get().studioRender->ForcedMaterialOverride(meterial);
			DrawModelOriginal(interfaces::Get().studioRender, result, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

			return interfaces::Get().studioRender->ForcedMaterialOverride(nullptr);
		}
	}

	DrawModelOriginal(interfaces::Get().studioRender, result, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
}
