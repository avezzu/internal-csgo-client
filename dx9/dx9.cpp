#include "dx9.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"

#include <stdexcept>


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


bool dx9::SetupWindowClass(const char* windowClassName) 
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	if (!RegisterClassEx(&windowClass)) 
	{
		return false;
	}

	return true;

}

void dx9::DestroyWindowClass()
{
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool dx9::SetupWindow(const char* windowName)
{
	window = CreateWindow(windowClass.lpszClassName, windowName, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, 0, 0, windowClass.hInstance, 0);

	if (!window)
	{
		return false;
	}

	return true;
}
void dx9::DestroyWindow()
{
	if (window)
	{
		DestroyWindow(window);
	}
}

bool dx9::SetupDirectX()
{
	auto handle = GetModuleHandle("d3d9.dll");

	if (!handle)
	{
		return false;
	}
	
	//hook Direct3DCreate9 function
	auto create = (LPDIRECT3D9(WINAPI*)(UINT))GetProcAddress(handle, "Direct3DCreate9");

	if (!create)
	{
		return false;
	}

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9) 
	{
		return false;
	}

	D3DPRESENT_PARAMETERS params = {};
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device) < 0) {
		return false;
	}

	return true;
}
void dx9::DestroyDirectX()
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

void dx9::Setup()
{
	if (!SetupWindowClass("imgui_class"))
		throw std::runtime_error("Failed to create window class.");

	if (!SetupWindow("imgui_window"))
		throw std::runtime_error("Failed to create window.");

	if (!SetupDirectX())
		throw std::runtime_error("Failed to create device.");


	DestroyWindow();
	DestroyWindowClass();

}

void dx9::SetupMenu(LPDIRECT3DDEVICE9 device)
{
	auto params = D3DDEVICE_CREATION_PARAMETERS{};
	device->GetCreationParameters(&params);

	window = params.hFocusWindow;

	//replace WindowProcess and returns original WindowProcess
	original_windowProcess = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)(WindowProcess));
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	setup = true;

}

void dx9::Destroy()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)(original_windowProcess));

	DestroyDirectX();
}


LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {


	if (dx9::open && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) 
	{
		return 1L;
	}

	return CallWindowProc(dx9::original_windowProcess, hWnd, msg, wParam, lParam);
}