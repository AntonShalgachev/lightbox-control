#include "LedControl.h"

const uint8_t LedControl::PIN_R[NUM_LEDS] = {68, 70, 9, 27, 6, 44, 20, 54, 65, 52, 31, 63, 28, 13, 49, 40, 38, 18, 12, 2, 32, 35, 43, 41};
const uint8_t LedControl::PIN_G[NUM_LEDS] = {60, 62, 26, 15, 7, 42, 10, 59, 51, 56, 1, 64, 34, 53, 47, 61, 45, 22, 23, 30, 25, 67, 29, 21};
const uint8_t LedControl::PIN_B[NUM_LEDS] = {71, 69, 8, 11, 4, 14, 17, 57, 66, 58, 46, 33, 3, 50, 55, 36, 48, 37, 16, 5, 24, 39, 0, 19};

LedControl::LedControl(uint8_t pinWhite) : pinWhite_(pinWhite)
{
	SetWhiteBrightness(0x00);
	SetRGBBrightness(0x00);

	SetRGBBackground(Color::Black);
}

void LedControl::Setup()
{
	pinMode(pinWhite_, OUTPUT);
	Tlc.init(0);
}

void LedControl::SetWhiteBrightness(uint8_t level)
{
	brightnessWhite_ = level;
}

void LedControl::SetRGBBrightness(uint8_t level)
{
	brightnessRGB_ = level;

	memset(brightnessData_, level, NUM_LEDS);
}

void LedControl::SetRGBSingleLed(uint8_t led, const Color& color, bool overrideBrightness, uint8_t brightness)
{
	for(uint8_t _led = 0; _led < NUM_LEDS; ++_led)
	{
		if(_led == led)
		{
			colorData_[_led].SetColor(color);
			if(overrideBrightness)
				brightnessData_[led] = brightness;
		}
		else
		{
			colorData_[_led].SetColor(Color::Black);
		}
	}
}

void LedControl::SetRGBLed(uint8_t led, const Color& color, bool overrideBrightness, uint8_t brightness)
{
	if(led >= 0 && led < NUM_LEDS)
	{
		colorData_[led].SetColor(color);
		if(overrideBrightness)
			brightnessData_[led] = brightness;
	}
}

void LedControl::SetRGBBackground(const Color& color)
{
	for(uint8_t led = 0; led < NUM_LEDS; ++led)
		SetRGBLed(led, color);
}

void LedControl::Update() const
{
	analogWrite(pinWhite_, brightnessWhite_);

	for(uint8_t led = 0; led < NUM_LEDS; ++led)
	{
		// uint16_t ledR = (1024ULL * brightnessRGB_ * colorData_[led].r * RED_MAX) / (255UL * 255 * 255);
		// uint16_t ledG = (1024ULL * brightnessRGB_ * colorData_[led].g * GREEN_MAX) / (255UL * 255 * 255);
		// uint16_t ledB = (1024ULL * brightnessRGB_ * colorData_[led].b * BLUE_MAX) / (255UL * 255 * 255);

		uint16_t ledR = ((uint16_t)brightnessData_[led] * colorData_[led].r) >> (24-LED_BRIGHTNESS_MAX_BITS-RED_MAX_BITS);
		uint16_t ledG = ((uint16_t)brightnessData_[led] * colorData_[led].g) >> (24-LED_BRIGHTNESS_MAX_BITS-GREEN_MAX_BITS);
		uint16_t ledB = ((uint16_t)brightnessData_[led] * colorData_[led].b) >> (24-LED_BRIGHTNESS_MAX_BITS-BLUE_MAX_BITS);

		Tlc.set(PIN_R[led], ledR);
		Tlc.set(PIN_G[led], ledG);
		Tlc.set(PIN_B[led], ledB);
	}
	Tlc.update();
}
