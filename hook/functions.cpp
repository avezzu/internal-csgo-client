#include "../move/move.h"
#include "../aim/aim.h"
#include "../vis/vis.h"
#include "../Interface/interfaces.h"
#include "fucntions.h"


bool __stdcall FN::CreateMove(float frameTime, UserCmd* cmd)
{
	bool result = OriginalCreateMove(interfaces::g_ClientMode, frameTime, cmd);

	if (!cmd || !cmd->commandNumber) {
		return result;
	}

	if (result)
		interfaces::engine->SetViewAngles(cmd->viewAngles);
	
	if (move::bhop)move::enableBhop(cmd);
	if (aim::enableTriggerBot)aim::triggerBot(cmd);
	if (aim::enableSilentAimBot)aim::SilentAimBot(cmd);
	if (aim::enableAimBot)aim::AimBot();

	return false;
}

void __stdcall FN::DoPostScreenSpaceEffects(const void* viewSetup)
{
	DoPostScreenSpaceEffectsOriginal(interfaces::g_ClientMode, viewSetup);
	if (vis::enableGlowESP)vis::GlowESP();
}


void __stdcall FN::FrameStageNotify(ClientFrameStage_t curStage)
{
	FrameStageNotifyOriginal(interfaces::g_Client, curStage);
	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		if (vis::enableskinChanger)vis::skinChanger();
	}
}

void __stdcall FN::PaintTraverse(uintptr_t panel, bool forceRepaint, bool allowForce)
{	
	if (panel == interfaces::engineVGui->GetPanel(PANEL_TOOLS))
	{
		if (vis::enableTraceLine)vis::TraceLine();
		if (vis::enableBoxESP)vis::BoxESP();
		if (vis::enableSkeletonESP)vis::SkeletonESP();
	}
	
	PaintTraverseOriginal(interfaces::panel, panel, forceRepaint, allowForce);
}


void __stdcall FN::DrawModel(void* result, const CDrawModelInfo& info, CMatrix3x4* bones, float* flexWeights, float* flexDelayedWeights, const CVector& modelOrigin, const std::int32_t flags)
{
	
	if (!interfaces::engine->IsInGame())
	{
		DrawModelOriginal(interfaces::studioRender, result, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
		return;
	}
	
	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = (interfaces::EntityList->GetClientEntity(index));
	if (localPlayer && info.renderable && vis::enableChams)
	{
		auto entity = info.renderable->GetIClientUnknown()->GetBaseEntity();
		if (entity && entity->IsPlayer() && entity->Team() != localPlayer->Team())
		{
			static IMaterial* meterial = interfaces::materialSystem->FindMaterial("debug/debugambientcube");

			float hidden[3] = { 0.0117f, 0.f, 0.3882f };
			float visible[3] = { 0.4588f, 0.96078f, 0.258823f };

			interfaces::studioRender->SetAlphaModulation(1.f);
			meterial->SetMaterialVarFlag(IMaterial::IGNOREZ, true);
			interfaces::studioRender->SetColorModulation(visible);
			interfaces::studioRender->ForcedMaterialOverride(meterial);
			DrawModelOriginal(interfaces::studioRender, result, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

			meterial->SetMaterialVarFlag(IMaterial::IGNOREZ, false);
			interfaces::studioRender->SetColorModulation(hidden);
			interfaces::studioRender->ForcedMaterialOverride(meterial);
			DrawModelOriginal(interfaces::studioRender, result, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

			return interfaces::studioRender->ForcedMaterialOverride(nullptr);
		}
	}

	DrawModelOriginal(interfaces::studioRender, result, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
}


