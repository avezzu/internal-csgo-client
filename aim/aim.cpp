#include "aim.h"
#include <iostream>
#include "../Interface/interfaces.h"


bool aim::enableTriggerBot = false;

void aim::triggerBot(UserCmd* cmd) {
	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::EntityList->GetClientEntity(index);

	if (!localPlayer || !localPlayer->IsAlive())
		return;

	CVector eyePosition;
	localPlayer->GetEyePosition(eyePosition);

	CVector aimPunch;
	localPlayer->GetAimPunch(aimPunch);

	CVector dst = eyePosition + CVector{cmd->viewAngles + aimPunch}.ToVector() * 1000.f;

	CTrace trace;
	interfaces::engineTrace->TraceRay({eyePosition, dst}, 0x46004009, localPlayer, trace);

	if (!trace.entity || !trace.entity->IsPlayer())
		return;

	if (trace.entity->IsDormant())
		return;

	if (!trace.entity->IsAlive() || trace.entity->GetTeam() == localPlayer->GetTeam())
		return;

	cmd->buttons |= IN_ATTACK;
}


constexpr CVector CalculateAngle(CVector localPos, CVector enemyPos, CVector viewAngles)
{
	return (enemyPos - localPos).ToAngle() - viewAngles;
}

bool aim::enableAimBot = false;
void aim::AimBot()
{
	
	if (!GetAsyncKeyState(VK_MENU))
		return;

	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::EntityList->GetClientEntity(index);

	if (!localPlayer)
		return;

	CVector localEyePostion;
	localPlayer->GetEyePosition(localEyePostion);

	CVector aimPunch;
	localPlayer->GetAimPunch(aimPunch);

	CVector viewAngles;
	interfaces::engine->GetViewAngles(viewAngles);

	auto bestFov = 150.f;
	auto bestAngle = CVector{};


	for (int i = 1; i <= 64; i++) 
	{
		auto player = interfaces::EntityList->GetClientEntity(i);
		if (!player)
			continue;

		if (!player->IsPlayer())
			continue;

		if (player->Team() == localPlayer->Team())
			continue;
		
		if (player->IsDormant())
			continue;

		if (!player->Health())
			continue;

		
		CMatrix3x4 bone_matrix[128];
		if (!player->SetupBones(bone_matrix, 128, 0x7FF00, interfaces::globals->currentTime))
			continue;
		
		auto headPos = bone_matrix[8].Origin();

	
		CTrace trace;
		interfaces::engineTrace->TraceRay({localEyePostion, bone_matrix[8].Origin()}, 0x46004009, localPlayer, trace);
		if (!trace.entity || trace.fraction < 0.97f)
			continue;


		auto angle = CalculateAngle(localEyePostion, headPos, viewAngles + aimPunch);
		auto fov = std::hypot(angle.x, angle.y);


		if (fov < bestFov)
		{
			bestFov = fov;
			bestAngle = angle;
		}	
	}

	if (!bestAngle.isZero())
		interfaces::engine->SetViewAngles(viewAngles + bestAngle); //to smooth devide bestAngle, i.e. make it smaller	
}



bool aim::enableSilentAimBot = false;
void aim::SilentAimBot(UserCmd* cmd)
{

	if (!GetAsyncKeyState(VK_MENU))
		return;


	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::EntityList->GetClientEntity(index);

	if (!localPlayer)
		return;

	CVector localEyePostion;
	localPlayer->GetEyePosition(localEyePostion);

	CVector aimPunch;
	localPlayer->GetAimPunch(aimPunch);

	auto bestFov = 150.f;
	auto bestAngle = CVector{};


	for (int i = 1; i <= 64; i++)
	{
		auto player = interfaces::EntityList->GetClientEntity(i);
		if (!player)
			continue;

		if (!player->IsPlayer())
			continue;

		if (player->Team() == localPlayer->Team())
			continue;

		if (player->IsDormant())
			continue;

		if (!player->IsAlive())
			continue;


		if (!player->SpottedByMask())
			continue;

		CMatrix3x4 bone_matrix[128];
		if (!player->SetupBones(bone_matrix, 128, 0x7FF00, interfaces::globals->currentTime))
			continue;

		auto headPos = bone_matrix[8].Origin();

		auto angle = CalculateAngle(localEyePostion, headPos, cmd->viewAngles + aimPunch);
		auto fov = std::hypot(angle.x, angle.y);

		if (fov < bestFov)
		{
			bestFov = fov;
			bestAngle = angle;
		}
	}

	if (!bestAngle.isZero())
		cmd->viewAngles = cmd->viewAngles + bestAngle; //to smooth devide bestAngle, i.e. make it smaller	

}


bool aim::enableRC = false;
CVector aim::oldPunch{0,0,0};
void aim::recoilControl()
{
	
	auto index = interfaces::engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::EntityList->GetClientEntity(index);
	if (!localPlayer)
		return;

	int shotsFired = localPlayer->ShotsFired();

	CVector viewAngles;
	interfaces::engine->GetViewAngles(viewAngles);

	CVector aimPunch;
	localPlayer->GetAimPunch(aimPunch);

	if (shotsFired)
	{

		auto newAngles = viewAngles + oldPunch - aimPunch;

		if (newAngles.x > 89.f)
			newAngles.x = 89.f;

		if (newAngles.x < -89.f)
			newAngles.x = -89.f;

		if (newAngles.y > 180.f)
			newAngles.y -= 360.f;

		if (newAngles.y < -180.f)
			newAngles.y += 360.f;

		interfaces::engine->SetViewAngles(newAngles);
	}
	oldPunch = aimPunch;
}
