#include "gui.h"
#include "../vis/vis.h"
#include "../move/move.h"
#include "../aim/aim.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"

#include <stdexcept>


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


bool gui::SetupWindowClass(const char* windowClassName) 
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

void gui::DestroyWindowClass()
{
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::SetupWindow(const char* windowName)
{
	window = CreateWindow(windowClass.lpszClassName, windowName, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, 0, 0, windowClass.hInstance, 0);

	if (!window)
	{
		return false;
	}

	return true;
}
void gui::DestroyWindow()
{
	if (window)
	{
		DestroyWindow(window);
	}
}

bool gui::SetupDirectX()
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
void gui::DestroyDirectX()
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

void gui::Setup()
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

void gui::SetupMenu(LPDIRECT3DDEVICE9 device)
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

void gui::Destroy()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)(original_windowProcess));

	DestroyDirectX();
}

void gui::Render()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	ImGui::Begin("Menu", &open);
	ImGui::BeginTabBar("tabs");

	if (ImGui::BeginTabItem("Aim")) {
		ImGui::Checkbox("TriggerBot", &aim::enableTriggerBot);
		ImGui::Checkbox("AimBot", &aim::enableAimBot);
		ImGui::Checkbox("SilentAimBot", &aim::enableSilentAimBot);
		ImGui::Checkbox("recoilControl", &aim::enableRC);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Movement")) {
		ImGui::Checkbox("Bhop", &move::bhop);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Visuals")) {
		ImGui::Checkbox("AntiFlash", &vis::antiflash);
		ImGui::Checkbox("GlowESP", &vis::enableGlowESP);
		ImGui::Checkbox("BoxESP", &vis::enableBoxESP);
		ImGui::Checkbox("SkeletonESP", &vis::enableSkeletonESP);
		ImGui::Checkbox("SnapLine", &vis::enableTraceLine);
		ImGui::SliderInt("FOV", &vis::FOV, -180, 180);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Misc")) {
		ImGui::Checkbox("Radar", &vis::enableRadar);
		ImGui::Checkbox("SkinChanger", &vis::enableskinChanger);

		if (ImGui::Button("Disable cheats"))
		{
			aim::enableTriggerBot = false;
			aim::enableAimBot = false;
			aim::enableSilentAimBot = false;
			aim::enableRC = false;
			move::bhop = false;
			vis::antiflash = false;
			vis::enableGlowESP = false;
			vis::enableBoxESP = false;
			vis::enableSkeletonESP = false;
			vis::enableTraceLine = false;
			vis::FOV = 90;
			vis::enableRadar = false;
			vis::enableskinChanger = false;
		}

		ImGui::EndTabItem();
	}


	ImGui::EndTabBar();
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}


LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if(GetAsyncKeyState(VK_INSERT) & 1)
	{
		gui::open = !gui::open;
	}

	if (gui::open && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) 
	{
		return 1L;
	}

	return CallWindowProc(gui::original_windowProcess, hWnd, msg, wParam, lParam);
}
