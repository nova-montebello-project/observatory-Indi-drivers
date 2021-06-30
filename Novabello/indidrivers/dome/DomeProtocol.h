////////////////////////////////////////////////////////////////////////////////////////////////
//	PROTOCOL DESIGN BEGIN
////////////////////////////////////////////////////////////////////////////////////////////////
//	Protocol Design (all bytes are Ascii).
//		Byte0: Command/Response
//		Byte1-M: Optional data
//		ByteN: Termination character
////////////////////////////////////////////////////////////////////////////////////////////////
namespace DomeProtocol
{
	//------------------------------------------------------------------------------------------
	// Commands sent to client. Client responds to messages with the same command value or the error command if not understood
	//------------------------------------------------------------------------------------------
	typedef enum eCommand
	{
		COMMAND_GET_VERSION			, //data is blank, reply data is two bytes containing the version as hex nibbles (MSN first).
		COMMAND_PANIC_STOP_ALL		, //data is blank, reply data is blank.
		COMMAND_SET_TRACE_LEVEL		, //data is one byte containing the character for eTraceLevel, reply data is blank.
		COMMAND_TRACE				, //can only be sent from client, sent whenever needed and only when trace is turned on - data is text.
		COMMAND_ERROR				, //can only be sent from client, sent when in trouble - data is one byte containing the character for eErrorCode.
		COMMAND_SET_AUTO_STATUS		, //data is one byte containing the character for eAutoUpdate, reply data is blank.
		COMMAND_SET_DOME_POSITION	, //data is four bytes containing the dome position as hex nibbles (MSN first), reply data is blank.
		COMMAND_GET_DOME_STATUS		, //data is blank, reply data is one byte containing the character for eDomeStatus, then four bytes containing the current position as hex nibbles (MSN first).
		COMMAND_SET_SLOT_POSITION	, //data is one byte containing the character for eSlotPosition, reply data is blank.
		COMMAND_GET_SLOT_STATUS		, //data is blank, reply data is one byte containing the character for eSlotStatus. 
		//------------------------------------------------------------
		COMMAND_TOTAL		//Must reside as last in enum!
	} teCommand;

	//------------------------------------------------------------------------------------------
	typedef enum eErrorCode
	{
		ERROR_CODE_BUFFER_FULL,
		ERROR_CODE_BAD_COMMAND,
		ERROR_CODE_BAD_LENGTH,
		ERROR_CODE_BAD_PARAMETER,
		//------------------------------------------------------------
		ERROR_CODE_TOTAL		//Must reside as last in enum!
	} teErrorCode;

	//------------------------------------------------------------------------------------------
	typedef enum eTraceLevel
	{
		TRACE_LEVEL_OFF,
		TRACE_LEVEL_1,		//informative
		TRACE_LEVEL_2,		//extensive for debugging
		//------------------------------------------------------------
		TRACE_LEVEL_TOTAL	//Must reside as last in enum!
	} teTraceLevel;

	//------------------------------------------------------------------------------------------
	typedef enum eAutoUpdate
	{
		AUTO_UPDATE_OFF,
		AUTO_UPDATE_ON,		//client will update status on change
		//------------------------------------------------------------
		AUTO_UPDATE_TOTAL	//Must reside as last in enum!
	} teAutoUpdate;

	//------------------------------------------------------------------------------------------
	typedef enum eDomeStatus
	{
		DOME_STATE_UNKNOWN_POSITION,
		DOME_STATE_INITIALIZING,
		DOME_STATE_AT_POSITION,
		DOME_STATE_MOVING_CLOCKWISE,
		DOME_STATE_MOVING_COUNTER,
		//------------------------------------------------------------
		DOME_STATE_TOTAL	//Must reside as last in enum!
	} teDomeStatus;

	//------------------------------------------------------------------------------------------
	typedef enum eSlotPosition
	{
		SLOT_POSITION_CLOSE,
		SLOT_POSITION_OPEN,
		//------------------------------------------------------------
		SLOT_POSITION_TOTAL	//Must reside as last in enum!
	} teSlotPosition;

	//------------------------------------------------------------------------------------------
	typedef enum eSlotStatus
	{
		SLOT_STATUS_UNKNOWN,
		SLOT_STATUS_OPENED,
		SLOT_STATUS_OPENING,
		SLOT_STATUS_CLOSED,
		SLOT_STATUS_CLOSING,
		//------------------------------------------------------------
		SLOT_STATUS_TOTAL	//Must reside as last in enum!
	} teSlotStatus;

	//------------------------------------------------------------------------------------------
	extern const char		m_cTerminationChar;

	extern uint8_t			GetVersionHigh		();
	extern uint8_t			GetVersionLow		();

	extern teCommand		GetCommand			(const char& rcByte);
	extern teErrorCode		GetError			(const char& rcByte);
	extern teTraceLevel		GetTraceLevel		(const char& rcByte);
	extern teAutoUpdate		GetAutoUpdate		(const char& rcByte);
	extern teDomeStatus		GetDomeStatus		(const char& rcByte);
	extern teSlotPosition	GetSlotPosition		(const char& rcByte);
	extern teSlotStatus		GetSlotStatus		(const char& rcByte);

	extern char 			GetCharCmd			(teCommand		eValue);
	extern char 			GetCharError		(teErrorCode	eValue);
	extern char 			GetCharTraceLevel	(teTraceLevel	eValue);
	extern char				GetCharAutoUpdate	(teAutoUpdate	eValue);
	extern char 			GetCharDomeStatus	(teDomeStatus	eValue);
	extern char 			GetCharSlotPosition	(teSlotPosition	eValue);
	extern char 			GetCharSlotStatus	(teSlotStatus	eValue);

	//------------------------------------------------------------------------------------------
	extern bool				GetNibbleValueFromHexChar(const char& rcChar, uint8_t& rReturnValue);
	extern const char&		GetHexCharFromNibbleValue(uint8_t Value);

	extern bool				ReadByte8Hex	(const char* pBuffer, uint8_t&  rReturnValue);
	extern bool				ReadByte16Hex	(const char* pBuffer, uint16_t& rReturnValue);
	extern bool				ReadByte32Hex	(const char* pBuffer, uint32_t& rReturnValue);

	extern void				WriteByte8Hex	(uint8_t  Value, char* pBuffer);
	extern void				WriteByte16Hex	(uint16_t Value, char* pBuffer);
	extern void				WriteByte32Hex	(uint32_t Value, char* pBuffer);

	//------------------------------------------------------------------------------------------
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	PROTOCOL DESIGN END
////////////////////////////////////////////////////////////////////////////////////////////////
