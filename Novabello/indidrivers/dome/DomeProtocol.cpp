////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DomeProtocol.h"

////////////////////////////////////////////////////////////////////////////////////////////////
namespace DomeProtocol
{
	//------------------------------------------------------------------------------------------
	uint8_t VersionHigh = 0;
	uint8_t VersionLow  = 1;

	//------------------------------------------------------------------------------------------
	const char HEXVALUES[] = "0123456789ABCDEF";
	const char m_cTerminationChar = 13;

	//------------------------------------------------------------------------------------------
	const char m_cCommands[COMMAND_TOTAL] = 
	{
		'V', //COMMAND_GET_VERSION
		'P', //COMMAND_PANIC_STOP_ALL
		'L', //COMMAND_SET_TRACE_LEVEL
		'T', //COMMAND_TRACE
		'E', //COMMAND_ERROR
		'A', //COMMAND_SET_AUTO_STATUS
		'D', //COMMAND_SET_DOME_POSITION
		'd', //COMMAND_GET_DOME_STATUS
		'S', //COMMAND_SET_SLOT_POSITION
		's', //COMMAND_GET_SLOT_STATUS
	};

	const char m_cErrors[ERROR_CODE_TOTAL] = 
	{
		'B', //ERROR_CODE_BUFFER_FULL
		'C', //ERROR_CODE_BAD_COMMAND
		'L', //ERROR_CODE_BAD_LENGTH
		'P', //ERROR_CODE_BAD_PARAMETER,
	};

	const char m_cTraceLevel[TRACE_LEVEL_TOTAL] = 
	{
		'0', //TRACE_LEVEL_OFF
		'1', //TRACE_LEVEL_1
		'2', //TRACE_LEVEL_2
	};

	const char m_cAutoUpdate[AUTO_UPDATE_TOTAL] = 
	{
		'0', //AUTO_UPDATE_OFF
		'1', //AUTO_UPDATE_ON
	};

	const char m_cDomeState[DOME_STATE_TOTAL] = 
	{
		'U', //DOME_STATE_UNKNOWN_POSITION
		'I', //DOME_STATE_INITIALIZING
		'P', //DOME_STATE_AT_POSITION
		'C', //DOME_STATE_MOVING_CLOCKWISE
		'O', //DOME_STATE_MOVING_COUNTER
	};

	const char m_cSlotPosition[SLOT_POSITION_TOTAL] = 
	{
		'C', //SLOT_POSITION_CLOSE
		'O', //SLOT_POSITION_OPEN
	};

	const char m_cSlotStatus[SLOT_STATUS_TOTAL] = 
	{
		'U', //SLOT_STATUS_UNKNOWN
		'O', //SLOT_STATUS_OPENED
		'o', //SLOT_STATUS_OPENING
		'C', //SLOT_STATUS_CLOSED
		'c', //SLOT_STATUS_CLOSING
	};

	const char m_cBadOffset = 'X';

	//------------------------------------------------------------------------------------------
	int GetOffset(const char& rcByte, const char* pBuffer, int iTotal)
	{
		for (int iOffset = 0; iOffset < iTotal; ++iOffset)
			if (rcByte == *(pBuffer++))
				return iOffset;
		return iTotal;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t DomeProtocol::GetVersionHigh()
{
	return VersionHigh;
}

uint8_t DomeProtocol::GetVersionLow()
{
	return VersionLow;
}

DomeProtocol::teCommand DomeProtocol::GetCommand(const char& rcByte)
{
	return (teCommand) GetOffset(rcByte, m_cCommands, COMMAND_TOTAL);
}

DomeProtocol::teErrorCode DomeProtocol::GetError(const char& rcByte)
{
	return (teErrorCode) GetOffset(rcByte, m_cErrors, ERROR_CODE_TOTAL);
}

DomeProtocol::teTraceLevel DomeProtocol::GetTraceLevel(const char& rcByte)
{
	return (teTraceLevel) GetOffset(rcByte, m_cTraceLevel, TRACE_LEVEL_TOTAL);
}

DomeProtocol::teAutoUpdate DomeProtocol::GetAutoUpdate(const char&rcByte)
{
	return (teAutoUpdate) GetOffset(rcByte, m_cAutoUpdate, AUTO_UPDATE_TOTAL);
}

DomeProtocol::teDomeStatus DomeProtocol::GetDomeStatus(const char& rcByte)
{
	return (teDomeStatus) GetOffset(rcByte, m_cDomeState, DOME_STATE_TOTAL);
}

DomeProtocol::teSlotPosition DomeProtocol::GetSlotPosition(const char& rcByte)
{
	return (teSlotPosition) GetOffset(rcByte, m_cSlotPosition, SLOT_POSITION_TOTAL);
}

DomeProtocol::teSlotStatus DomeProtocol::GetSlotStatus(const char& rcByte)
{
	return (teSlotStatus) GetOffset(rcByte, m_cSlotStatus, SLOT_STATUS_TOTAL);
}

char DomeProtocol::GetCharCmd(teCommand eValue)
{
	return (eValue < COMMAND_TOTAL) ? m_cCommands[eValue] : m_cBadOffset;
}

char DomeProtocol::GetCharError(teErrorCode eValue)
{
	return (eValue < ERROR_CODE_TOTAL) ? m_cErrors[eValue] : m_cBadOffset;
}

char DomeProtocol::GetCharTraceLevel(teTraceLevel eValue)
{
	return (eValue < TRACE_LEVEL_TOTAL) ? m_cTraceLevel[eValue] : m_cBadOffset;
}

char DomeProtocol::GetCharAutoUpdate (teAutoUpdate eValue)
{
	return (eValue < AUTO_UPDATE_TOTAL) ? m_cAutoUpdate[eValue] : m_cBadOffset;
}

char DomeProtocol::GetCharDomeStatus(teDomeStatus eValue)
{
	return (eValue < DOME_STATE_TOTAL) ? m_cDomeState[eValue] : m_cBadOffset;
}

char DomeProtocol::GetCharSlotPosition(teSlotPosition eValue)
{
	return (eValue < SLOT_POSITION_TOTAL) ? m_cSlotPosition[eValue] : m_cBadOffset;
}

char DomeProtocol::GetCharSlotStatus(teSlotStatus eValue)
{
	return (eValue < SLOT_STATUS_TOTAL) ? m_cSlotStatus[eValue] : m_cBadOffset;
}

bool DomeProtocol::GetNibbleValueFromHexChar(const char& rcChar, uint8_t& rReturnValue)
{
	if((rcChar >= '0') && (rcChar <= '9'))
	{
		rReturnValue = (uint8_t) (rcChar - '0');
		return true;
	}
	if((rcChar >= 'A') && (rcChar <= 'F'))
	{
		rReturnValue = 10 + (uint8_t) (rcChar - 'A');
		return true;
	}
	return false;	//not a hex character
}

const char& DomeProtocol::GetHexCharFromNibbleValue(uint8_t Value)
{
	return HEXVALUES[Value & 0x0F];
}

bool DomeProtocol::ReadByte8Hex(const char* pBuffer, uint8_t& rReturnValue)
{
	uint8_t MSN = 0;
	if(!GetNibbleValueFromHexChar(*pBuffer, MSN))
		return false;
	uint8_t LSN = 0;
	if(!GetNibbleValueFromHexChar(*(pBuffer + 1), LSN))
		return false;
	rReturnValue = (MSN << 4) + LSN; 
	return true;
}

bool DomeProtocol::ReadByte16Hex(const char* pBuffer, uint16_t& rReturnValue)
{
	uint8_t MSB = 0;
	if(!ReadByte8Hex(pBuffer, MSB))
		return false;
	uint8_t LSB = 0;
	if(!ReadByte8Hex(pBuffer + 2, LSB))
		return false;
	rReturnValue = (((uint16_t) MSB) << 8) + (uint16_t) LSB; 
	return true;
}

bool DomeProtocol::ReadByte32Hex(const char* pBuffer, uint32_t& rReturnValue)
{
	uint16_t MSW = 0;
	if(!ReadByte16Hex(pBuffer, MSW))
		return false;
	uint16_t LSW = 0;
	if(!ReadByte16Hex(pBuffer + 4, LSW))
		return false;
	rReturnValue = (((uint32_t) MSW) << 16) + (uint32_t) LSW; 
	return true;
}

void DomeProtocol::WriteByte8Hex(uint8_t Value, char* pBuffer)
{
	*pBuffer		= GetHexCharFromNibbleValue(Value >> 4);
	*(pBuffer + 1)	= GetHexCharFromNibbleValue(Value);
}

void DomeProtocol::WriteByte16Hex(uint16_t Value, char* pBuffer)
{
	WriteByte8Hex((uint8_t) ((Value >> 8) & 0xFF), pBuffer);
	WriteByte8Hex((uint8_t)  (Value & 0xFF)		 , pBuffer + 2);
}

void DomeProtocol::WriteByte32Hex(uint32_t Value, char* pBuffer)
{
	WriteByte16Hex((uint16_t) ((Value >> 16) & 0xFFFF), pBuffer);
	WriteByte16Hex((uint16_t)  (Value & 0xFFFF)		  , pBuffer + 4);
}

////////////////////////////////////////////////////////////////////////////////////////////////
