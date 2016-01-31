#pragma once

#include <Arduino.h>

#include "SerialComm.h"
#include "PotReader.h"
#include "LedControl.h"

enum DebugMessage
{
	SETUP_END = 0xff,
	MESSAGE_RECEIVED = 0xfe
};

enum Message
{
	HANDSHAKE = 0x10,
	SET_SINGLE_LED = 0x11,	// Set particular LED to color, turn off others
	SET_LED = 0x12,			// Set particular LED to color, leave others as they were
	SET_ALL_LEDS = 0x13,	// Set all LEDs with particular color
	SET_BACKGROUND = 0x14	// Set all LEDs with one color
};

void setup();
void loop();

void Update();
void StartupAnimation1();
void StartupAnimation2();

void OnMessageAvailable(uint8_t msg, uint8_t len, const uint8_t* data);

void MsgHandshakeHandler(uint8_t len, const uint8_t* data);
void MsgSetSingleLed(uint8_t len, const uint8_t* data);
void MsgSetLed(uint8_t len, const uint8_t* data);
void MsgSetAllLeds(uint8_t len, const uint8_t* data);
void MsgSetBackground(uint8_t len, const uint8_t* data);

SerialComm comm;
PotReader readerWhite(0);
PotReader readerRGB(1);
LedControl control(5);
