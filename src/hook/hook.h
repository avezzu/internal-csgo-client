#pragma once
#include <Windows.h>
#include "../../dx9/dx9.h"
#include "../math/types/vmatrix.h"
#include "../virtual/virtual.h"
#include "../../sdk/interfaces.h"
#include <iostream>



namespace hook{

    enum ClientFrameStage_t : int {
        FRAME_UNDEFINED = -1,
        FRAME_START,
        FRAME_NET_UPDATE_START,
        FRAME_NET_UPDATE_POSTDATAUPDATE_START,
        FRAME_NET_UPDATE_POSTDATAUPDATE_END,
        FRAME_NET_UPDATE_END,
        FRAME_RENDER_START,
        FRAME_RENDER_END
    };

    void Init();
    void Shutdown();


    constexpr void* VirtualFunction(void* ptr, size_t index)
	{
		return (*(void***)(ptr))[index];
	}
	
    inline long (__thiscall* EndSceneOriginal)(void*, IDirect3DDevice9*) = nullptr;
	long __stdcall EndScene(IDirect3DDevice9* device);

	inline HRESULT (__thiscall* ResetOriginal)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);

    inline bool(__thiscall* OriginalCreateMove)(void*, float, UserCmd*) = nullptr;
	bool __stdcall CreateMove(float frameTime, UserCmd* userCmd);

	inline void(__thiscall* DoPostScreenSpaceEffectsOriginal)(void*, const void*) = nullptr;
	void __stdcall DoPostScreenSpaceEffects(const void* viewSetup);

    inline void(__thiscall* FrameStageNotifyOriginal)(void*, ClientFrameStage_t) = nullptr;
    void __stdcall FrameStageNotify(ClientFrameStage_t curStage);
    
    inline void(__thiscall* PaintTraverseOriginal)(void*, uintptr_t, bool, bool) = nullptr;
    void __stdcall PaintTraverse(uintptr_t panel, bool forceRepaint, bool allowForce);

    inline void(__thiscall* DrawModelOriginal)(void*, void*, const CDrawModelInfo&, matrix3x4_t*, float*, float*, const Vector&, const std::int32_t) = nullptr;
    void __stdcall DrawModel(void* result, const CDrawModelInfo& info, matrix3x4_t* bones, float* flexWeights, float* flexDelayedWeights, const Vector& modelOrigin, const std::int32_t flags);
}

