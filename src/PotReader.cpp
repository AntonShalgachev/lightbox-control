#include "PotReader.h"

PotReader::PotReader(uint8_t potPin)
{
	potPin_ = potPin;

	for(uint8_t i = 0; i < NUM_READINGS; ++i)
		readings_[i] = 0;

	sum_ = 0;
	index_ = 0;
}

uint8_t PotReader::Read()
{
	sum_ -= readings_[index_];
	readings_[index_] = analogRead(potPin_) >> 2;
	sum_ += readings_[index_];

	index_ = (index_ + 1) % NUM_READINGS;

	return sum_ / NUM_READINGS;
}
