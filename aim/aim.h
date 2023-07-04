#pragma once
#include "../Interface/usercmd.h"


namespace aim {

	extern bool enableTriggerBot;
	void triggerBot(UserCmd* cmd);

	extern bool enableAimBot;
	void AimBot();

	extern bool enableSilentAimBot;
	void SilentAimBot(UserCmd* cmd);

	extern bool enableRC;
	extern CVector oldPunch;
	void recoilControl();
}
