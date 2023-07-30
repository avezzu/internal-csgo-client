#pragma once
#include <Windows.h>
#include <d3d9.h>

namespace vis {
	extern bool antiflash;
	void AntiFlash();

	extern bool enableRadar;
	void EnableRadar();

	extern bool enableGlowESP;
	void GlowESP();

	extern bool enableTraceLine;
	void TraceLine();

	extern bool enableskinChanger;
	void skinChanger();

	extern bool enableBoxESP;
	void BoxESP();

	extern bool enableSkeletonESP;
	void SkeletonESP();

	extern bool enableChams;

	extern int FOV;
	extern int oldFOV;
	void changeFOV(int FOV);

	

	extern int screen_width;
	extern int screen_hight;
		 
}