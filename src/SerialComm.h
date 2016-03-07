#pragma once

/*
 * Message structure:
 * MSG_BEGIN_BYTE, MSG_ID, DATA_LEN, DATA
 * 
 * MSG_BEGIN_BYTE, MSG_ID and DATA_LEN are 1 byte; DATA is DATA_LEN-byte long
 *
 * MSG_BEGIN_BYTE is a byte indicating message start. MSG_BEGIN_BYTE = 16
 * MSG_ID is message id byte. Values 0-15 are reserved, 16-255 are valid
 * DATA_LEN is the length of data in message. This is byte, thus valid values are 0-255
 * DATA is data, associated with particular message. Its length is DATA_LEN
 *
 * SerialComm reads bytes 
 */

#include <Arduino.h>

class SerialComm
{
private:
	typedef void (*MsgHandler)(uint8_t msg, uint8_t len, const uint8_t* data);
	enum State
	{
		IDLE,
		RECEIVING_META,
		RECEIVING_DATA,
		MESSAGE_AVAILABLE
	};

public:
	SerialComm();
	void Setup();

	void AttachHandler(MsgHandler handler);

	void Update();
	bool Available() const;

	void GetMessage(uint8_t* msg, uint8_t* len, const uint8_t** data);

	static void SendReportFail();
	static void SendReportOk();
	static void SendReportInvalidMsgId();
	static void SendReportMalformedMsg();
	static void SendMessage(uint8_t msg, uint8_t len = 0, const uint8_t* data = nullptr);
	static void SendByte(uint8_t b);

private:
	uint8_t buffer_[UINT8_MAX + 2]; // MSG_ID (1 byte), DATA_LEN (1 byte), DATA (DETA_LEN bytes)
	State state_;
	uint16_t bytesRead_;
	MsgHandler handler_;

	static const unsigned long BAUD_RATE = 9600;

	static const uint8_t MSG_BEGIN_BYTE = 0x06;

	static const uint8_t REPORT_FAIL = 0x00;
	static const uint8_t REPORT_OK = 0x01;
	static const uint8_t REPORT_INVALID_MSG_ID = 0x02;
	static const uint8_t REPORT_MALFORMED_MSG = 0x03;
};
