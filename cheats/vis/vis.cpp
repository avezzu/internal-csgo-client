#include "vis.h"
#include "../../src/math/types/vector.h"
#include "../../sdk/interfaces.h"
#include "../../src/math/types/vmatrix.h"
#include "../../src/math/types/matrix3x4.h"
#include "../../src/math/math.h"
#include "../../imgui/imgui.h"
#include <vector>
#include <string>
#include <iostream>

int vis::screen_width = 0;
int vis::screen_hight = 0;


bool vis::antiflash = false;
void vis::AntiFlash() 
{	
	if (!interfaces::Get().engine->IsInGame())
		return;

	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = (interfaces::Get().EntityList->GetClientEntity(index));
	if (localPlayer)
	{
		localPlayer->FlashDuration() = 0;
	}
}

bool vis::enableRadar = false;
void vis::EnableRadar()
{
	if (!interfaces::Get().engine->IsInGame())
		return;

	for (int i = 1; i <= 64; i++)
	{
		auto entity = interfaces::Get().EntityList->GetClientEntity(i);
		if (!entity)
			continue;

		entity->Spotted() = true;
	}
}


bool vis::enableGlowESP = false;
void vis::GlowESP()
{
	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::Get().EntityList->GetClientEntity(index);

	if (localPlayer && interfaces::Get().engine->IsInGame() && localPlayer->IsAlive())
	{
		for (int i = 0; i < interfaces::Get().glow->glowObjects.size; i++)
		{
			IGlowManager::CGlowObject& glowObject = interfaces::Get().glow->glowObjects[i];

			if (glowObject.IsUnused())
				continue;

			if (!glowObject.entity)
				continue;

			switch (glowObject.entity->GetClientClass()->classID)
			{
			case CClientClass::CCSPlayer:
				if (!glowObject.entity->IsAlive())
					break;

				if (glowObject.entity->Team() != localPlayer->Team())
				{
					glowObject.SetColor(0.00784f, 0.9882f, 0.92549f);
				}
				break;

			case CClientClass::CWeaponAWP:
					glowObject.SetColor(1.f, 1.f, 1.f);					
					break;

			}
		}
	}
}

bool vis::enableTraceLine = false;
void vis::TraceLine()
{
	if (!interfaces::Get().engine->IsInGame())
		return;

	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::Get().EntityList->GetClientEntity(index);
	if (!localPlayer)
		return;

	for (int i = 1; i < 64; i++)
	{
		
		if (!localPlayer->IsAlive())
			continue;
		VMatrix matrix = interfaces::Get().engine->WorldToScreenMatrix();
        CMath::Get().UpdateViewMatrix(&matrix);
		auto player = interfaces::Get().EntityList->GetClientEntity(i);
		if (!player)
			continue;
		if (player == localPlayer)
			continue;
		if (!player->IsPlayer())
			continue;
		if (player->Health() <= 0 || player->Team() == localPlayer->Team() || player->IsDormant())
			continue;

		ImVec2 pos;
		
		matrix3x4_t bone_matrix[128];
		if (!player->SetupBones(bone_matrix, 128, 0x7FF00, interfaces::Get().globals->currentTime))
			continue;

		Vector headPos = CMath::Get().Origin(bone_matrix[8]);
        

		if (CMath::Get().WorldToScreen(headPos, pos))
		{	
			interfaces::Get().surface->DrawSetColor(2, 252, 236);
			interfaces::Get().surface->DrawLine(pos.x, pos.y, vis::screen_width / 2, 0);
		}
	}
}

bool vis::enableBoxESP = false;
void vis::BoxESP()
{
	if (!interfaces::Get().engine->IsInGame())
		return;

	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::Get().EntityList->GetClientEntity(index);
	if (!localPlayer)
		return;

	for (int i = 1; i < 64; i++)
	{
		if (!localPlayer->IsAlive())
			continue;
		VMatrix matrix = interfaces::Get().engine->WorldToScreenMatrix();
        CMath::Get().UpdateViewMatrix(&matrix);

		auto player = interfaces::Get().EntityList->GetClientEntity(i);
		if (!player)
			continue;
		if (player == localPlayer)
			continue;
		if (!player->IsPlayer())
			continue;
		if (player->Health() <= 0 || player->Team() == localPlayer->Team() || player->IsDormant())
			continue;

		ImVec2 top;
		ImVec2 bottom;

		Vector vecOrigin = player->GetAbsOrigin() - Vector{0.f, 0.f, 11.f};

		matrix3x4_t bone_matrix[128];
		if (!player->SetupBones(bone_matrix, 128, 0x7FF00, interfaces::Get().globals->currentTime))
			continue;

		Vector headPos = CMath::Get().Origin(bone_matrix[8]) + Vector{0.f, 0.f, 9.f};

		if (CMath::Get().WorldToScreen(headPos, top) && CMath::Get().WorldToScreen(vecOrigin, bottom))
		{
			float h = bottom.y - top.y;
			float w = h * 0.3f;

			int left = bottom.x - w;
			int right = bottom.x + w;

			interfaces::Get().surface->DrawSetColor(255, 255, 255);
			interfaces::Get().surface->DrawOutlinedRect(left, top.y, right, bottom.y);

			interfaces::Get().surface->DrawSetColor(0, 0, 0);
			interfaces::Get().surface->DrawOutlinedRect(left - 1, top.y - 1, right + 1, bottom.y + 1);
			interfaces::Get().surface->DrawOutlinedRect(left + 1, top.y + 1, right - 1, bottom.y - 1);

			interfaces::Get().surface->DrawOutlinedRect(left - 6, top.y - 1, right - 3, bottom.y + 1);
			
			float frac = player->Health() * 0.01f;

			interfaces::Get().surface->DrawSetColor((1.f - frac) * 255, 255 * frac, 0);
			interfaces::Get().surface->DrawFilledRect(left - 5, bottom.y - (h * frac), left, bottom.y);
		}
	}
}


bool vis::enableSkeletonESP = false;
void vis::SkeletonESP()
{
	if (!interfaces::Get().engine->IsInGame())
		return;

	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::Get().EntityList->GetClientEntity(index);
	if (!localPlayer)
		return;


	for (int j = 0; j < 64; j++)
	{
		if (!localPlayer->IsAlive())
			continue;
		auto player = interfaces::Get().EntityList->GetClientEntity(j);
		if (!player)
			continue;
		if (player == localPlayer)
			continue;
		if (!player->IsPlayer())
			continue;
		if (player->Health() <= 0 || player->Team() == localPlayer->Team() || player->IsDormant())
			continue;
		
		auto studio = interfaces::Get().modelInfo->GetStudioModel(player->GetModel());
		if (!studio)
			return;

		matrix3x4_t bone_matrix[128];
		if (!player->SetupBones(bone_matrix, 128, 0x7FF00, interfaces::Get().globals->currentTime))
			continue;

		for (int i = 0; i < studio->numBones; i++)
		{
			auto bone = studio->GetBone(i);
			if (bone && (bone->flags & 0x100) && (bone->parent != -1))
			{
				
				Vector bonePos = CMath::Get().Origin(bone_matrix[i]);
				Vector parentPos = CMath::Get().Origin(bone_matrix[bone->parent]);
				Vector boneScreen, parentScreen;

				if (!interfaces::Get().debugOverlay->ScreenPosition(bonePos, boneScreen) && !interfaces::Get().debugOverlay->ScreenPosition(parentPos, parentScreen))
				{
					interfaces::Get().surface->DrawSetColor(2, 252, 236);
					interfaces::Get().surface->DrawLine(boneScreen.x, boneScreen.y, parentScreen.x, parentScreen.y);
				}
			}
		}
	}
	
}


void changeSkin(CEntity* pWeapon, int skin)
{
	pWeapon->iItemIDHigh() = -1;
	pWeapon->FallbackPaintKit() = skin;
	pWeapon->FallbackWear() = 0.1f;
}

bool vis::enableskinChanger = false;
void vis::skinChanger()
{	
	
	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::Get().EntityList->GetClientEntity(index);

	if (localPlayer) { 
		for (int i = 0; i < 8; i++) 
		{
			DWORD* cWep = localPlayer->GetWeapons();

			if (!cWep)continue;

			auto pWeapon = interfaces::Get().EntityList->GetClientEntityFromHandle(cWep[i]);
			if (pWeapon) { 
				
				switch (pWeapon->GetClientClass()->classID)
				{
				case CClientClass::CAK47:
					changeSkin(pWeapon, 639);
					break;
				case CClientClass::CWeaponAWP:
					changeSkin(pWeapon, 344);
					break;
				case CClientClass::CWeaponM4A1:
					changeSkin(pWeapon, 16);
				case CClientClass::CDEagle:
					changeSkin(pWeapon, 37);
				}
				
			}
		}
	}
}


int vis::FOV = 90;
int vis::oldFOV = 90;
void vis::changeFOV(int FOV)
{
	if (FOV == oldFOV)
		return;

	if (!interfaces::Get().engine->IsInGame())
		return;
	
	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::Get().EntityList->GetClientEntity(index);

	if (localPlayer)
	{
		localPlayer->DefaultFOV() = FOV;
	}

	oldFOV = FOV;
}


bool vis::enableChams = false;