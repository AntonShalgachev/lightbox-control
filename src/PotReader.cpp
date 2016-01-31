#include "PotReader.h"

PotReader::PotReader(uint8_t potPin)
{
	_potPin = potPin;

	for(uint8_t i = 0; i < NUM_READINGS; ++i)
		_readings[i] = 0;

	_sum = 0;
	_index = 0;
}

uint8_t PotReader::Read()
{
	_sum -= _readings[_index];
	_readings[_index] = analogRead(_potPin) >> 2;
	_sum += _readings[_index];

	_index = (_index + 1) % NUM_READINGS;

	return _sum / NUM_READINGS;
}
