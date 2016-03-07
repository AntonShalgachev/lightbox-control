#pragma once

#include <Arduino.h>
#include <Tlc5940.h>

#include "Color.h"

// #define NUM_LEDS 24

class LedControl
{
public:
	LedControl(uint8_t pinWhite);

	void Setup();

	void SetWhiteBrightness(uint8_t level);
	void SetRGBBrightness(uint8_t level);

	void SetRGBSingleLed(uint8_t led, const Color& color, bool overrideBrightness = false, uint8_t brightness = 0xff);
	void SetRGBLed(uint8_t led, const Color& color, bool overrideBrightness = false, uint8_t brightness = 0xff);
	void SetRGBBackground(const Color& color);

	void Update() const;

public:
	static const uint8_t NUM_LEDS = 24;

private:
	uint8_t pinWhite_;

	uint8_t brightnessWhite_;
	uint8_t brightnessRGB_;

	Color colorData_[NUM_LEDS];
	uint8_t brightnessData_[NUM_LEDS];

	// static const uint8_t RED_MAX = 255;
	// static const uint8_t GREEN_MAX = 100;
	// static const uint8_t BLUE_MAX = 70;

	static const uint8_t RED_MAX_BITS = 8; // 256
	static const uint8_t GREEN_MAX_BITS = 7; // 128
	static const uint8_t BLUE_MAX_BITS = 6; // 64

	static const uint16_t LED_BRIGHTNESS_MAX_BITS = 10; // 1024

	static const uint8_t PIN_R[NUM_LEDS];
	static const uint8_t PIN_G[NUM_LEDS];
	static const uint8_t PIN_B[NUM_LEDS];
};
