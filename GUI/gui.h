#pragma once
#include <d3d9.h>


//variable must be inline, since we define them multiple times, where the fucntion is defined only once
namespace gui {
	inline bool open = false;
	inline bool setup = false;
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = {};
	inline WNDPROC original_windowProcess = nullptr;

	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;


	bool SetupWindowClass(const char* windowClassName);
	void DestroyWindowClass();

	bool SetupWindow(const char* windowName);
	void DestroyWindow();

	bool SetupDirectX();
	void DestroyDirectX();

	void Setup();

	void SetupMenu(LPDIRECT3DDEVICE9 device);
	void Destroy();

	void Render();
};