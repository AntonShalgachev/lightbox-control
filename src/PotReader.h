#pragma once

#include <Arduino.h>

class PotReader
{
public:
	PotReader(uint8_t potPin);

	uint8_t Read();

private:
	static const uint8_t NUM_READINGS = 5;

	uint8_t potPin_;

	uint8_t readings_[NUM_READINGS];
	uint8_t index_;
	uint16_t sum_;
};
