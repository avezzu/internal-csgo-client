#pragma once
#include "../../sdk/interfaces.h"


namespace aim {

	extern bool enableTriggerBot;
	void triggerBot(UserCmd* cmd);

	extern bool enableAimBot;
	void AimBot();

	extern bool enableSilentAimBot;
	void SilentAimBot(UserCmd* cmd);

	extern bool enableRC;
	extern Vector oldPunch;
	void recoilControl();
}