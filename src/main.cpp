#include "main.h"

void setup()
{
	comm.Setup();
	comm.AttachHandler(OnMessageAvailable);
	control.Setup();

	StartupAnimation2();

	control.SetWhiteBrightness(0x00);
	control.SetRGBBrightness(0x00);
	control.SetRGBBackground(Color::Red);

	comm.SendMessage(DebugMessage::SETUP_END, 0, nullptr);
}

void loop()
{
	control.SetWhiteBrightness(readerWhite.Read());
	control.SetRGBBrightness(readerRGB.Read());

	Update();
}

void Update()
{
	comm.Update();
	control.Update();
}

void StartupAnimation1()
{
	control.SetWhiteBrightness(0x00);
	control.SetRGBBrightness(0x00);

	const uint8_t nColors = 4;
	const Color colors[nColors] = {Color::Red, Color::Green, Color::Blue, Color::White};

	for(uint8_t i = 0; i < nColors; i++)
	{
		control.SetRGBBackground(colors[i]);

		uint8_t br = 1;
		uint8_t dir = 1;
		while(br > 0)
		{
			control.SetRGBBrightness(br);
			control.Update();
			delayMicroseconds(500);

			br += dir;
			if(br == UINT8_MAX)
				dir = -dir;
		}
	}
}

void StartupAnimation2()
{
	control.SetWhiteBrightness(0x00);

	control.SetRGBBackground(Color::Black);
	control.SetRGBBrightness(0xff);

	Color col1 = Color::Red;
	Color col2 = Color::Blue;
	Color colOverlap = Color::Magenta;

	uint8_t brightnessMain = 0xff;
	uint8_t brightnessSide = 0x10;
	uint8_t brightnessOverlap = 0x80;

	int8_t i = 0;
	uint8_t dir = 1;
	while(i >= 0)
	{
		control.SetRGBBackground(Color::Black);

		if(i == 0)
		{
			uint8_t iNext = i + 1;

			uint8_t led1 = i;
			uint8_t led1Next = iNext;

			uint8_t led2 = LedControl::NUM_LEDS - i - 1;
			uint8_t led2Next = LedControl::NUM_LEDS - iNext - 1;

			control.SetRGBLed(led1, col1, brightnessMain);
			control.SetRGBLed(led1Next, col1, brightnessSide);

			control.SetRGBLed(led2, col2, brightnessMain);
			control.SetRGBLed(led2Next, col2, brightnessSide);
		}
		else if(i == LedControl::NUM_LEDS - 1)
		{
			uint8_t iPrev = i - 1;

			uint8_t led1Prev = iPrev;
			uint8_t led1 = i;

			uint8_t led2Prev = LedControl::NUM_LEDS - iPrev - 1;
			uint8_t led2 = LedControl::NUM_LEDS - i - 1;

			control.SetRGBLed(led1Prev, col1, brightnessSide);
			control.SetRGBLed(led1, col1, brightnessMain);

			control.SetRGBLed(led2Prev, col2, brightnessSide);
			control.SetRGBLed(led2, col2, brightnessMain);
		}
		else
		{
			uint8_t iPrev = i - 1;
			uint8_t iNext = i + 1;

			uint8_t led1Prev = iPrev;
			uint8_t led1 = i;
			uint8_t led1Next = iNext;

			uint8_t led2Prev = LedControl::NUM_LEDS - iPrev - 1;
			uint8_t led2 = LedControl::NUM_LEDS - i - 1;
			uint8_t led2Next = LedControl::NUM_LEDS - iNext - 1;

			if(led1Next == led2)
			{
				control.SetRGBLed(led1Prev, col1, brightnessSide);
				control.SetRGBLed(led1, colOverlap, brightnessOverlap);
				control.SetRGBLed(led1Next, colOverlap, brightnessOverlap);

				control.SetRGBLed(led2Prev, col2, brightnessSide);
			}
			else if(led1Prev == led2)
			{
				control.SetRGBLed(led1Next, col1, brightnessSide);

				control.SetRGBLed(led2Prev, colOverlap, brightnessOverlap);
				control.SetRGBLed(led2, colOverlap, brightnessOverlap);
				control.SetRGBLed(led2Next, col2, brightnessSide);
			}
			else
			{
				control.SetRGBLed(led1Prev, col1, brightnessSide);
				control.SetRGBLed(led1, col1, brightnessMain);
				control.SetRGBLed(led1Next, col1, brightnessSide);

				control.SetRGBLed(led2Prev, col2, brightnessSide);
				control.SetRGBLed(led2, col2, brightnessMain);
				control.SetRGBLed(led2Next, col2, brightnessSide);
			}
		}

		if(i == LedControl::NUM_LEDS - 1)
			dir = -dir;

		i += dir;

		control.Update();
		delay(50);
	}
}

void OnMessageAvailable(uint8_t msg, uint8_t len, const uint8_t* data)
{
	switch(msg)
	{
	case Message::HANDSHAKE:				// 0x10
		MsgHandshakeHandler(len, data);
		break;

	case Message::SET_SINGLE_LED:			// 0x11
		MsgSetSingleLed(len, data);
		break;

	case Message::SET_LED:					// 0x12
		MsgSetLed(len, data);
		break;

	case Message::SET_ALL_LEDS:				// 0x13
		MsgSetAllLeds(len, data);
		break;

	case Message::SET_BACKGROUND:			// 0x14
		MsgSetBackground(len, data);
		break;

	default:
		comm.SendReportInvalidMsgId();
	}
}

//
// Message id: 0x10
// Sends back unique sequence of bytes: 0x13 0xd9 0xf0 0x72
//
void MsgHandshakeHandler(uint8_t len, const uint8_t* data)
{
	uint8_t response[4] = {0x13, 0xd9, 0xf0, 0x72};
	comm.SendMessage(Message::HANDSHAKE, 4, response);
}

//
// Message id: 0x11
// Length: 4
// Data: LED_ID COLOR_R COLOR_G COLOR_B
//
void MsgSetSingleLed(uint8_t len, const uint8_t* data)
{
	if(len == 4)
	{
		control.SetRGBSingleLed(data[0], Color(data[1], data[2], data[3]));
		comm.SendReportOk();
	}
	else
	{
		comm.SendReportMalformedMsg();
	}
}

//
// Message id: 0x12
// Length: 4
// Data: LED_ID COLOR_R COLOR_G COLOR_B
//
void MsgSetLed(uint8_t len, const uint8_t* data)
{
	if(len == 4)
	{
		control.SetRGBLed(data[0], Color(data[1], data[2], data[3]));
		comm.SendReportOk();
	}
	else
	{
		comm.SendReportMalformedMsg();
	}
}

//
// Message id: 0x13
// Length: 3 * 24 = 72
// Data: COLOR_R_0 COLOR_G_0 COLOR_B_0 ... COLOR_R_23 COLOR_G_23 COLOR_B_23
//
void MsgSetAllLeds(uint8_t len, const uint8_t* data)
{
	if(len == 3 * LedControl::NUM_LEDS)
	{
		for(uint8_t led = 0; led < LedControl::NUM_LEDS; led++)
		{
			Color col(data[3*led + 0], data[3*led + 1], data[3*led + 2]);
			control.SetRGBLed(led, col);
		}

		comm.SendReportOk();
	}
	else
	{
		comm.SendReportMalformedMsg();
	}
}

//
// Message id: 0x14
// Length: 3
// Data: COLOR_R COLOR_G COLOR_B
//
void MsgSetBackground(uint8_t len, const uint8_t* data)
{
	if(len == 3)
	{
		control.SetRGBBackground(Color(data[0], data[1], data[2]));
		comm.SendReportOk();
	}
	else
	{
		comm.SendReportMalformedMsg();
	}
}
