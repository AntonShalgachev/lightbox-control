#include "SerialComm.h"

SerialComm::SerialComm()
{
	handler_ = nullptr;
	state_ = State::IDLE;
}

void SerialComm::Setup()
{
	Serial.begin(BAUD_RATE);
}

void SerialComm::AttachHandler(MsgHandler handler)
{
	handler_ = handler;
}

void SerialComm::Update()
{
	// SendByte(state_);

	if(state_ == State::MESSAGE_AVAILABLE)
	{
		if(handler_)
		{
			handler_(buffer_[0], buffer_[1], buffer_ + 2);
			state_ = State::IDLE;
		}

		return;
	}

	while(Serial.available() > 0 && state_ != State::MESSAGE_AVAILABLE)
	{
		switch(state_)
		{
		case State::MESSAGE_AVAILABLE:
			// Processed before
			break;

		case State::IDLE:
			if(Serial.read() == MSG_BEGIN_BYTE)
			{
				state_ = State::RECEIVING_META;
				bytesRead_ = 0;
			}
			break;

		case State::RECEIVING_META:
			buffer_[bytesRead_++] = Serial.read();

			if(bytesRead_ >= 2)
			{
				if(buffer_[1] == 0)
					state_ = State::MESSAGE_AVAILABLE;
				else
					state_ = State::RECEIVING_DATA;
			}
			break;

		case State::RECEIVING_DATA:
			uint8_t len = buffer_[1];
			uint8_t dataBytesRead = bytesRead_ - 2;
			uint8_t dataBytesRemain = len - dataBytesRead;

			uint8_t bytesAvailable = Serial.available();

			uint8_t bytesToRead = min(bytesAvailable, dataBytesRemain);
			Serial.readBytes(buffer_ + bytesRead_, bytesToRead);
			bytesRead_ += bytesToRead;

			if(bytesRead_ - 2 == len)
				state_ = State::MESSAGE_AVAILABLE;

			break;
		}
	}
}

bool SerialComm::Available() const
{
	return state_ == State::MESSAGE_AVAILABLE;
}

void SerialComm::GetMessage(uint8_t* msg, uint8_t* len, const uint8_t** data)
{
	*msg = buffer_[0];
	*len = buffer_[1];
	*data = buffer_ + 2;

	state_ = State::IDLE;
}

void SerialComm::SendReportFail()
{
	SendMessage(REPORT_FAIL);
}

void SerialComm::SendReportOk()
{
	SendMessage(REPORT_OK);
}

void SerialComm::SendReportInvalidMsgId()
{
	SendMessage(REPORT_INVALID_MSG_ID);
}

void SerialComm::SendReportMalformedMsg()
{
	SendMessage(REPORT_MALFORMED_MSG);
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
