#pragma once
#include <d3d9.h>
#include <d3dx9.h>

namespace draw
{
	
	void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* device);
	void DrawLine(int x1, int y1, int x2, int y2, int t, D3DCOLOR color, IDirect3DDevice9* device);
}