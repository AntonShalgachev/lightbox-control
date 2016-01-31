#include "Color.h"

Color::Color()
{
	SetColor(0, 0, 0);
}

Color::Color(uint8_t r, uint8_t g, uint8_t b)
{
	SetColor(r, g, b);
}

Color::Color(uint32_t color)
{
	SetColor(color);
}

Color::Color(const Color& color)
{
	SetColor(color);
}

void Color::SetColor(uint8_t r, uint8_t g, uint8_t b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}

void Color::SetColor(uint32_t color)
{
	SetColor((color >> 16) & 0xff, (color >> 8) & 0xff, (color)& 0xff);
}

void Color::SetColor(const Color& color)
{
	SetColor(color.r, color.g, color.b);
}

const Color Color::Red = Color(0xff0000);
const Color Color::Green = Color(0x00ff00);
const Color Color::Blue = Color(0x0000ff);
const Color Color::White = Color(0xffffff);
const Color Color::Black = Color(0x000000);
const Color Color::Magenta = Color(0xff00ff);
const Color Color::Cyan = Color(0x00ffff);
const Color Color::Yellow = Color(0xffff00);
