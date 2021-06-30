///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include <string>

#include "DomeProtocol.h"
#include "DomeHandler.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int DomeProtocolHandler::LocalInterpret(const std::string& rsReceived)
{
	std::string::size_type EndIndex = rsReceived.find(DomeProtocol::m_cTerminationChar);
	if (EndIndex == std::string::npos)
		return 0;
	const std::string sData = rsReceived.substr(0, EndIndex);
	if(!sData.length())
	{
		DebugOutputReceived("Got empty message");
		return (EndIndex + 1);
	}
	DebugOutputReceived("Received " + sData);
	const DomeProtocol::teCommand Command = DomeProtocol::GetCommand(sData[0]);
	std::string sPayload = sData.substr(1, sData.length() - 1);
	const std::string::size_type PayloadLength = sPayload.length();
	switch(Command) 
	{
	case DomeProtocol::COMMAND_GET_VERSION:
		{
			uint8_t VersionHigh = 0;
			uint8_t VersionLow  = 0;
			if ((PayloadLength != 2) ||
				(!DomeProtocol::GetNibbleValueFromHexChar(sPayload[0], VersionHigh)) ||
				(!DomeProtocol::GetNibbleValueFromHexChar(sPayload[1], VersionLow )) )
				DebugOutputReceived("Bad version received");
			ReplyVersion(VersionHigh, VersionLow);
		}
		break;
	case DomeProtocol::COMMAND_PANIC_STOP_ALL:
		ReplyPanicStopAll();
		break;
	case DomeProtocol::COMMAND_SET_TRACE_LEVEL:
		ReplySetTraceLevel();
		break;
	case DomeProtocol::COMMAND_TRACE:
		ReplyFirmwareTrace(sPayload);
		break;
	case DomeProtocol::COMMAND_ERROR:
		ReplyFirmwareError((PayloadLength > 0) ? DomeProtocol::GetError(sPayload[0]) : DomeProtocol::ERROR_CODE_TOTAL);
		break;
	case DomeProtocol::COMMAND_SET_DOME_POSITION:
		ReplySetDomePosition();
		break;
	case DomeProtocol::COMMAND_GET_DOME_STATUS:
		{
			DomeProtocol::teDomeStatus eState = DomeProtocol::DOME_STATE_TOTAL;
			uint16_t Position = 0;
			if (PayloadLength != 5)
				DebugOutputReceived("Bad dome position");
			else
			{
				eState = DomeProtocol::GetDomeStatus(sPayload[0]);
				DomeProtocol::ReadByte16Hex(&sPayload[1], Position); 
			}
			ReplyGetDomeStatus(eState, Position);
		}
		break;
	case DomeProtocol::COMMAND_SET_AUTO_STATUS:
		ReplySetAutoUpdate();
		break;
	case DomeProtocol::COMMAND_SET_SLOT_POSITION:
		ReplySetSlotPosition();
		break;
	case DomeProtocol::COMMAND_GET_SLOT_STATUS:
		ReplyGetSlotStatus((PayloadLength > 0) ? DomeProtocol::GetSlotStatus(sPayload[0]) : DomeProtocol::SLOT_STATUS_TOTAL);
		break;
	default:
		DebugOutputReceived("Invalid response");
		break;
	}
	return (EndIndex + 1);
}

void DomeProtocolHandler::Interpret(const std::string& rsNewBytes)
{
	m_sBuffer.insert(m_sBuffer.end(), rsNewBytes.begin(), rsNewBytes.end());
	for(;;)//ever
	{
		const unsigned long ulUsedBytes = LocalInterpret(m_sBuffer);
		if(!ulUsedBytes)
			break;
		m_sBuffer.erase(0, ulUsedBytes);
	}
}

void DomeProtocolHandler::Interpret(const unsigned char* pData, unsigned long ulDataLength)
{
	Interpret(std::string((const char*) pData, ulDataLength));
}

void DomeProtocolHandler::Interpret(DomeProtocolHandler& rHandler, const std::string& rsReceived)
{
	rHandler.Interpret(rsReceived);
}

void DomeProtocolHandler::Interpret(DomeProtocolHandler& rHandler, const unsigned char* pData, unsigned long ulDataLength)
{
	rHandler.Interpret(pData, ulDataLength);
}

std::string DomeProtocolHandler::CreateString(DomeProtocol::teCommand eCommand, const std::string* psData)
{
	std::string sData; 
	sData.push_back(DomeProtocol::GetCharCmd(eCommand));
	if (psData)
		sData += *psData; 
	sData.push_back(DomeProtocol::m_cTerminationChar);
	return sData;
}

std::string DomeProtocolHandler::CreateStringGetVersion()
{
	return CreateString(DomeProtocol::COMMAND_GET_VERSION);
}

std::string DomeProtocolHandler::CreateStringPanicStopAll()
{
	return CreateString(DomeProtocol::COMMAND_PANIC_STOP_ALL);
}

std::string DomeProtocolHandler::CreateStringSetTraceLevel(DomeProtocol::teTraceLevel eLevel)
{
	std::string sData; 
	sData.push_back(DomeProtocol::GetCharTraceLevel(eLevel));
	return CreateString(DomeProtocol::COMMAND_SET_TRACE_LEVEL, &sData);
}

std::string DomeProtocolHandler::CreateStringSetDomePosition(uint16_t Position)
{
	std::string sData; 
	sData.resize(4); 
	DomeProtocol::WriteByte16Hex(Position, &sData[0]);
	return CreateString(DomeProtocol::COMMAND_SET_DOME_POSITION, &sData);

}

std::string DomeProtocolHandler::CreateStringGetDomeStatus()
{
	return CreateString(DomeProtocol::COMMAND_GET_DOME_STATUS);
}

std::string DomeProtocolHandler::CreateStringSetAutoUpdate(DomeProtocol::teAutoUpdate eUpdate)
{
	std::string sData; 
	sData.push_back(DomeProtocol::GetCharAutoUpdate(eUpdate));
	return CreateString(DomeProtocol::COMMAND_SET_AUTO_STATUS, &sData);
}

std::string DomeProtocolHandler::CreateStringSetSlotPosition(DomeProtocol::teSlotPosition ePosition)
{
	std::string sData; 
	sData.push_back(DomeProtocol::GetCharSlotPosition(ePosition));
	return CreateString(DomeProtocol::COMMAND_SET_SLOT_POSITION, &sData);
}

std::string DomeProtocolHandler::CreateStringGetSlotStatus()
{
	return CreateString(DomeProtocol::COMMAND_GET_SLOT_STATUS);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
