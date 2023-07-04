#include "vis.h"
#include "../utils/cvector.h"
#include "../Interface/interfaces.h"
#include <iostream>
#include "../GUI/drawing.h"
#include "../utils/cmatrix.h"
#include <vector>
#include <string>

int vis::screen_width = 0;
int vis::screen_hight = 0;

bool vis::antiflash = false;
void vis::AntiFlash() 
{	
	if (!interfaces::engine->IsInGame())
		return;

	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = (interfaces::EntityList->GetClientEntity(index));
	if (localPlayer)
	{
		localPlayer->FlashDuration() = 0;
	}
}

bool vis::enableRadar = false;
void vis::EnableRadar()
{
	if (!interfaces::engine->IsInGame())
		return;

	for (int i = 1; i <= 64; i++)
	{
		auto entity = interfaces::EntityList->GetClientEntity(i);
		if (!entity)
			continue;

		entity->Spotted() = true;
	}
}


bool vis::enableGlowESP = false;
void vis::GlowESP()
{
	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::EntityList->GetClientEntity(index);

	if (localPlayer && interfaces::engine->IsInGame() && localPlayer->IsAlive())
	{
		for (int i = 0; i < interfaces::glow->glowObjects.size; i++)
		{
			IGlowManager::CGlowObject& glowObject = interfaces::glow->glowObjects[i];

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
	if (!interfaces::engine->IsInGame())
		return;

	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::EntityList->GetClientEntity(index);
	if (!localPlayer)
		return;

	for (int i = 1; i < 64; i++)
	{
		
		if (!localPlayer->IsAlive())
			continue;
		CMatrix4x4 matrix = interfaces::engine->WorldToScreenMatrix();
		auto player = interfaces::EntityList->GetClientEntity(i);
		if (!player)
			continue;
		if (player == localPlayer)
			continue;
		if (!player->IsPlayer())
			continue;
		if (player->Health() <= 0 || player->Team() == localPlayer->Team() || player->IsDormant())
			continue;

		Vec2 pos;
		
		CMatrix3x4 bone_matrix[128];
		if (!player->SetupBones(bone_matrix, 128, 0x7FF00, interfaces::globals->currentTime))
			continue;

		auto headPos = bone_matrix[8].Origin();

		if (matrix.WorldToSchreen(headPos, pos))
		{	
			interfaces::surface->DrawSetColor(2, 252, 236);
			interfaces::surface->DrawLine(pos.x, pos.y, vis::screen_width / 2, 0);
		}
	}
}

bool vis::enableBoxESP = false;
void vis::BoxESP()
{
	if (!interfaces::engine->IsInGame())
		return;

	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::EntityList->GetClientEntity(index);
	if (!localPlayer)
		return;

	for (int i = 1; i < 64; i++)
	{
		if (!localPlayer->IsAlive())
			continue;
		CMatrix4x4 matrix = interfaces::engine->WorldToScreenMatrix();
		auto player = interfaces::EntityList->GetClientEntity(i);
		if (!player)
			continue;
		if (player == localPlayer)
			continue;
		if (!player->IsPlayer())
			continue;
		if (player->Health() <= 0 || player->Team() == localPlayer->Team() || player->IsDormant())
			continue;

		Vec2 top;
		Vec2 bottom;

		CVector vecOrigin = player->GetAbsOrigin() - CVector{0.f, 0.f, 11.f};

		CMatrix3x4 bone_matrix[128];
		if (!player->SetupBones(bone_matrix, 128, 0x7FF00, interfaces::globals->currentTime))
			continue;

		auto headPos = bone_matrix[8].Origin() + CVector{0.f, 0.f, 9.f};

		if (matrix.WorldToSchreen(headPos, top) && matrix.WorldToSchreen(vecOrigin, bottom))
		{
			float h = bottom.y - top.y;
			float w = h * 0.3f;

			int left = bottom.x - w;
			int right = bottom.x + w;

			interfaces::surface->DrawSetColor(255, 255, 255);
			interfaces::surface->DrawOutlinedRect(left, top.y, right, bottom.y);

			interfaces::surface->DrawSetColor(0, 0, 0);
			interfaces::surface->DrawOutlinedRect(left - 1, top.y - 1, right + 1, bottom.y + 1);
			interfaces::surface->DrawOutlinedRect(left + 1, top.y + 1, right - 1, bottom.y - 1);

			interfaces::surface->DrawOutlinedRect(left - 6, top.y - 1, right - 3, bottom.y + 1);
			
			float frac = player->Health() * 0.01f;

			interfaces::surface->DrawSetColor((1.f - frac) * 255, 255 * frac, 0);
			interfaces::surface->DrawFilledRect(left - 5, bottom.y - (h * frac), left, bottom.y);
		}
	}
}


bool vis::enableSkeletonESP = false;
void vis::SkeletonESP()
{
	if (!interfaces::engine->IsInGame())
		return;

	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::EntityList->GetClientEntity(index);
	if (!localPlayer)
		return;


	for (int j = 0; j < 64; j++)
	{
		if (!localPlayer->IsAlive())
			continue;
		auto player = interfaces::EntityList->GetClientEntity(j);
		if (!player)
			continue;
		if (player == localPlayer)
			continue;
		if (!player->IsPlayer())
			continue;
		if (player->Health() <= 0 || player->Team() == localPlayer->Team() || player->IsDormant())
			continue;
		
		auto studio = interfaces::modelInfo->GetStudioModel(player->GetModel());
		if (!studio)
			return;

		CMatrix3x4 bone_matrix[128];
		if (!player->SetupBones(bone_matrix, 128, 0x7FF00, interfaces::globals->currentTime))
			continue;

		for (int i = 0; i < studio->numBones; i++)
		{
			auto bone = studio->GetBone(i);
			if (bone && (bone->flags & 0x100) && (bone->parent != -1))
			{
				
				CVector bonePos = bone_matrix[i].Origin();
				CVector parentPos = bone_matrix[bone->parent].Origin();
				CVector boneScreen, parentScreen;

				if (!interfaces::debugOverlay->ScreenPosition(bonePos, boneScreen) && !interfaces::debugOverlay->ScreenPosition(parentPos, parentScreen))
				{
					interfaces::surface->DrawSetColor(2, 252, 236);
					interfaces::surface->DrawLine(boneScreen.x, boneScreen.y, parentScreen.x, parentScreen.y);
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

bool vis::enableskinChanger;
void vis::skinChanger()
{	
	
	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::EntityList->GetClientEntity(index);

	if (localPlayer) { 
		for (int i = 0; i < 8; i++) 
		{
			DWORD* cWep = localPlayer->GetWeapons();

			if (!cWep)continue;

			auto pWeapon = interfaces::EntityList->GetClientEntityFromHandle(cWep[i]);
			if (pWeapon) { 
				
				switch (pWeapon->GetClientClass()->classID)
				{
				case CClientClass::CAK47:
					changeSkin(pWeapon, 639);
					break;
				case CClientClass::CWeaponAWP:
					changeSkin(pWeapon, 344);
					break;
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

	if (!interfaces::engine->IsInGame())
		return;
	
	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::EntityList->GetClientEntity(index);

	if (localPlayer)
	{
		localPlayer->DefaultFOV() = FOV;
	}

	oldFOV = FOV;
}
