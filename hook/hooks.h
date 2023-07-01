#pragma once
#include "../GUI/gui.h"

namespace hooks
{
	void Setup();
	void Destroy();

	constexpr void* VirtualFunction(void* ptr, size_t index)
	{
		return (*(void***)(ptr))[index];
	}


	inline long (__thiscall* EndSceneOriginal)(void*, IDirect3DDevice9*) = nullptr;
	long __stdcall EndScene(IDirect3DDevice9* device);

	inline HRESULT (__thiscall* ResetOriginal)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);

}

