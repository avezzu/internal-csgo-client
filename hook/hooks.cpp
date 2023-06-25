#include "hooks.h"
#include "../minhook/minhook.h"
#include <stdexcept>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"
#include <intrin.h>

#include "../Interface/interfaces.h"
#include "../move/move.h"
#include "fucntions.h"


void hooks::Setup()
{
	if (MH_Initialize())
	{
		throw std::runtime_error("Unable to initialize minhook");
	}

	if (MH_CreateHook(VirtualFunction(gui::device, 42), &EndScene, (void**)(&EndSceneOriginal)))
	{
		throw std::runtime_error("Unable to hook EndScene()");
	}

	if (MH_CreateHook(VirtualFunction(gui::device, 16), &Reset, (void**)(&ResetOriginal)))
	{
		throw std::runtime_error("Unable to hook Reset()");
	}

	if (MH_CreateHook(VirtualFunction(interfaces::g_ClientMode, 24), &FN::CreateMove, (void**)(&FN::OriginalCreateMove)))
	{
		throw std::runtime_error("Unable to hook CreateMove()");
	}

	/*
	if (MH_CreateHook(VirtualFunction(interfaces::g_ClientMode, 44), &FN::DoPostScreenSpaceEffects, (void**)(&FN::OriginalDoPostScreenSpaceEffects)))
	{
		throw std::runtime_error("Unable to hook DoPostScreenSpaceEffects()");
	}*/

	if (MH_EnableHook(MH_ALL_HOOKS))
	{
		throw std::runtime_error("Unable to enable hooks");
	}

	gui::DestroyDirectX();
}

void hooks::Destroy()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize(); 
}


long __stdcall hooks::EndScene(IDirect3DDevice9* device)
{
	static const auto address = _ReturnAddress();
	auto result = EndSceneOriginal(device, device);

	if (_ReturnAddress() == address) {
		return result;
	}

	if (!gui::setup) {
		gui::SetupMenu(device);
	}

	if (gui::open) {
		gui::Render();
	}

	return result;
}


//needed if gui menu window is changed
HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
	return 0;
}
