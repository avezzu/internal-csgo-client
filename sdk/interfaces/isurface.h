#pragma once
#include "../../src/virtual/virtual.h"

enum EFontDrawType : std::int32_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE
};

enum EFontFlag
{
	FONT_FLAG_NONE,
	FONT_FLAG_ITALIC = 0x001,
	FONT_FLAG_UNDERLINE = 0x002,
	FONT_FLAG_STRIKEOUT = 0x004,
	FONT_FLAG_SYMBOL = 0x008,
	FONT_FLAG_ANTIALIAS = 0x010,
	FONT_FLAG_GAUSSIANBLUR = 0x020,
	FONT_FLAG_ROTARY = 0x040,
	FONT_FLAG_DROPSHADOW = 0x080,
	FONT_FLAG_ADDITIVE = 0x100,
	FONT_FLAG_OUTLINE = 0x200,
	FONT_FLAG_CUSTOM = 0x400,
	FONT_FLAG_BITMAP = 0x800,
};

class ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a = 255) noexcept
	{
		vt::Call<void>(this, 15, r, g, b, a);
	}

	void DrawFilledRect(int x, int y, int xx, int yy) noexcept
	{
		vt::Call<void>(this, 16, x, y, xx, yy);
	}

	void DrawOutlinedRect(int x, int y, int xx, int yy) noexcept
	{
		vt::Call<void>(this, 18, x, y, xx, yy);
	}

	void DrawLine(int x1, int y1, int x2, int y2)
	{
		vt::Call<void>(this, 19, x1, y1, x2, y2);
	}

	void DrawSetTextColor(int r, int g, int b, int a = 255) noexcept
	{
		vt::Call<void>(this, 25, r, g, b, a);
	}

	void DrawSetTextPos(int x, int y)
	{
		vt::Call<void>(this, 26, x, y);
	}

	void DrawPrintText(const wchar_t* text, int textLen, EFontDrawType drawType = FONT_DRAW_DEFAULT)
	{
		vt::Call<void>(this, 28, text, textLen);
	}
};

class IVPanel
{
public:
	const char* GetName(std::uint32_t panel) noexcept
	{
		return vt::Call<const char*>(this, 36, panel);
	}
};