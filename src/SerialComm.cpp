#include "SerialComm.h"

SerialComm::SerialComm()
{
	_handler = nullptr;
	state = State::IDLE;
}

void SerialComm::Setup()
{
	Serial.begin(BAUD_RATE);
}

void SerialComm::AttachHandler(MsgHandler handler)
{
	_handler = handler;
}

void SerialComm::Update()
{
	// SendByte(state);

	if(state == State::MESSAGE_AVAILABLE)
	{
		if(_handler)
		{
			_handler(_buffer[0], _buffer[1], _buffer + 2);
			state = State::IDLE;
		}

		return;
	}

	while(Serial.available() > 0 && state != State::MESSAGE_AVAILABLE)
	{
		switch(state)
		{
		case State::MESSAGE_AVAILABLE:
			// Processed before
			break;

		case State::IDLE:
			if(Serial.read() == MSG_BEGIN_BYTE)
			{
				state = State::RECEIVING_META;
				_bytesRead = 0;
			}
			break;

		case State::RECEIVING_META:
			_buffer[_bytesRead++] = Serial.read();

			if(_bytesRead >= 2)
			{
				if(_buffer[1] == 0)
					state = State::MESSAGE_AVAILABLE;
				else
					state = State::RECEIVING_DATA;
			}
			break;

		case State::RECEIVING_DATA:
			uint8_t len = _buffer[1];
			uint8_t dataBytesRead = _bytesRead - 2;
			uint8_t dataBytesRemain = len - dataBytesRead;

			uint8_t bytesAvailable = Serial.available();

			uint8_t bytesToRead = min(bytesAvailable, dataBytesRemain);
			Serial.readBytes(_buffer + _bytesRead, bytesToRead);
			_bytesRead += bytesToRead;

			if(_bytesRead - 2 == len)
				state = State::MESSAGE_AVAILABLE;

			break;
		}
	}
}

bool SerialComm::Available() const
{
	return state == State::MESSAGE_AVAILABLE;
}

void SerialComm::GetMessage(uint8_t* msg, uint8_t* len, const uint8_t** data)
{
	*msg = _buffer[0];
	*len = _buffer[1];
	*data = _buffer + 2;

	state = State::IDLE;
}

void SerialComm::SendReportFail()
{
	SendMessage(RPRT_FAIL, 0, nullptr);
}

void SerialComm::SendReportOk()
{
	SendMessage(RPRT_OK, 0, nullptr);
}

void SerialComm::SendReportInvalidMsgId()
{
	SendMessage(RPRT_INVALID_MSG_ID, 0, nullptr);
}

void SerialComm::SendReportMalformedMsg()
{
	SendMessage(RPRT_MALFORMED_MSG, 0, nullptr);
}

void SerialComm::SendMessage(uint8_t msg, uint8_t len, const uint8_t* data)
{
	uint8_t query[] = {MSG_BEGIN_BYTE, msg, len};
	Serial.write(query, 3);

	if(len > 0 && data)
	{
		Serial.write(data, len);
	}
}

void SerialComm::SendByte(uint8_t b)
{
	Serial.write(&b, 1);
}
