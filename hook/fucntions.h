#pragma once
#include "../Interface/usercmd.h"
#include "../utils/cmatrix.h"
#include "../Interface/isurface.h"
#include "../Interface/istudiorender.h"


namespace FN
{
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

	inline bool(__thiscall* OriginalCreateMove)(void*, float, UserCmd*) = nullptr;
	bool __stdcall CreateMove(float frameTime, UserCmd* userCmd);

	inline void(__thiscall* DoPostScreenSpaceEffectsOriginal)(void*, const void*) = nullptr;
	void __stdcall DoPostScreenSpaceEffects(const void* viewSetup);


    inline void(__thiscall* FrameStageNotifyOriginal)(void*, ClientFrameStage_t) = nullptr;
    void __stdcall FrameStageNotify(ClientFrameStage_t curStage);
    
    inline void(__thiscall* PaintTraverseOriginal)(void*, uintptr_t, bool, bool) = nullptr;
    void __stdcall PaintTraverse(uintptr_t panel, bool forceRepaint, bool allowForce);

    inline void(__thiscall* DrawModelOriginal)(void*, void*, const CDrawModelInfo&, CMatrix3x4*, float*, float*, const CVector&, const std::int32_t) = nullptr;
    void __stdcall DrawModel(void* result, const CDrawModelInfo& info, CMatrix3x4* bones, float* flexWeights, float* flexDelayedWeights, const CVector& modelOrigin, const std::int32_t flags);
}
