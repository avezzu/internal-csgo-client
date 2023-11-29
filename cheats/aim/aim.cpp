#include "aim.h"
#include "../../src/math/math.h"
#include <iostream>



bool aim::enableTriggerBot = false;

void aim::triggerBot(UserCmd* cmd) {
	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::Get().EntityList->GetClientEntity(index);

	if (!localPlayer || !localPlayer->IsAlive())
		return;

	Vector eyePosition;
	localPlayer->GetEyePosition(eyePosition);

	Vector aimPunch;
	localPlayer->GetAimPunch(aimPunch);

	Vector dst = eyePosition + Vector{cmd->viewAngles + aimPunch}.ToVector() * 1000.f;

	CTrace trace;
	interfaces::Get().engineTrace->TraceRay({eyePosition, dst}, 0x46004009, localPlayer, trace);

	if (!trace.entity || !trace.entity->IsPlayer())
		return;

	if (trace.entity->IsDormant())
		return;

	if (!trace.entity->IsAlive() || trace.entity->GetTeam() == localPlayer->GetTeam())
		return;

	cmd->buttons |= IN_ATTACK;
}


Vector CalculateAngle(Vector localPos, Vector enemyPos, Vector viewAngles)
{
	return (enemyPos - localPos).ToAngle() - viewAngles;
}

bool aim::enableAimBot = false;
void aim::AimBot()
{
	
	if (!GetAsyncKeyState(VK_MENU))
		return;

	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::Get().EntityList->GetClientEntity(index);

	if (!localPlayer)
		return;

	Vector localEyePostion;
	localPlayer->GetEyePosition(localEyePostion);

	Vector aimPunch;
	localPlayer->GetAimPunch(aimPunch);

	Vector viewAngles;
	interfaces::Get().engine->GetViewAngles(viewAngles);

	auto bestFov = 150.f;
	auto bestAngle = Vector{};


	for (int i = 1; i <= 64; i++) 
	{
		auto player = interfaces::Get().EntityList->GetClientEntity(i);
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

		
		matrix3x4_t bone_matrix[128];
		if (!player->SetupBones(bone_matrix, 128, 0x7FF00, interfaces::Get().globals->currentTime))
			continue;
		
		Vector headPos = CMath::Get().Origin(bone_matrix[8]); 

	
		CTrace trace;
		interfaces::Get().engineTrace->TraceRay({localEyePostion, CMath::Get().Origin(bone_matrix[8])}, 0x46004009, localPlayer, trace);
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
		interfaces::Get().engine->SetViewAngles(viewAngles + bestAngle); //to smooth devide bestAngle, i.e. make it smaller	*/
}



bool aim::enableSilentAimBot = false;
void aim::SilentAimBot(UserCmd* cmd)
{

	if (!GetAsyncKeyState(VK_MENU))
		return;


	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::Get().EntityList->GetClientEntity(index);

	if (!localPlayer)
		return;

	Vector localEyePostion;
	localPlayer->GetEyePosition(localEyePostion);

	Vector aimPunch;
	localPlayer->GetAimPunch(aimPunch);

	auto bestFov = 150.f;
	auto bestAngle = Vector{};


	for (int i = 1; i <= 64; i++)
	{
		auto player = interfaces::Get().EntityList->GetClientEntity(i);
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

		matrix3x4_t bone_matrix[128];
		if (!player->SetupBones(bone_matrix, 128, 0x7FF00, interfaces::Get().globals->currentTime))
			continue;

		Vector headPos = CMath::Get().Origin(bone_matrix[8]);

		auto angle = CalculateAngle(localEyePostion, headPos, cmd->viewAngles + aimPunch);
		auto fov = std::hypot(angle.x, angle.y);

		if (fov < bestFov)
		{
			bestFov = fov;
			bestAngle = angle;
		}
	}

	if (!bestAngle.isZero())
		cmd->viewAngles = cmd->viewAngles + bestAngle; //to smooth devide bestAngle, i.e. make it smaller	*/
	
}


bool aim::enableRC = false;
Vector aim::oldPunch{0,0,0};
void aim::recoilControl()
{
	
	auto index = interfaces::Get().engine->GetLocalPlayerIndex();
	auto localPlayer = interfaces::Get().EntityList->GetClientEntity(index);
	if (!localPlayer)
		return;

	int shotsFired = localPlayer->ShotsFired();

	Vector viewAngles;
	interfaces::Get().engine->GetViewAngles(viewAngles);

	Vector aimPunch;
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

		interfaces::Get().engine->SetViewAngles(newAngles);
	}
	oldPunch = aimPunch;
}