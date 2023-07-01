#include "drawing.h"

void draw::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* device)
{
	D3DRECT rect = { x, y, x + w, y + h };
	device->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}


void draw::DrawLine(int x1, int y1, int x2, int y2, int t, D3DCOLOR color, IDirect3DDevice9* device)
{
	ID3DXLine* LineL;
	D3DXCreateLine(device, &LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);

	LineL->SetWidth(t);
	LineL->Draw(Line, 2, color);
	LineL->Release();
}