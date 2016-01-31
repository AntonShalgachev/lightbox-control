#pragma once

#include <Arduino.h>

class PotReader
{
public:
	PotReader(uint8_t potPin);

	uint8_t Read();

private:
	static const uint8_t NUM_READINGS = 5;

	uint8_t _potPin;

	uint8_t _readings[NUM_READINGS];
	uint8_t _index;
	uint16_t _sum;
};
