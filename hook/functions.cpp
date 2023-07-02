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

