#pragma once
#include "../../src/virtual/virtual.h"

enum EVGuiPanel
{
	PANEL_ROOT = 0,
	PANEL_GAMEUIDLL,
	PANEL_CLIENTDLL,
	PANEL_TOOLS,
	PANEL_INGAMESCREENS,
	PANEL_GAMEDLL,
	PANEL_CLIENTDLL_TOOLS
};

class IEngineVGui
{
public:
	std::uintptr_t GetPanel(std::int32_t type) noexcept
	{
		return vt::Call<std::uintptr_t>(this, 1, type);
	}
};