#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DomeProtocolHandler
{
private:
	std::string	m_sBuffer; 
protected: 
	DomeProtocolHandler(): m_sBuffer() {}
public:
	virtual ~DomeProtocolHandler() {}
public:	//high level protocol callback
	virtual void ReplyVersion			(uint8_t High, uint8_t Low) = 0;
	virtual void ReplyPanicStopAll		() = 0;
	virtual void ReplySetTraceLevel		() = 0;
	virtual void ReplyFirmwareTrace		(const std::string& rsTraceText) = 0;
	virtual void ReplyFirmwareError		(DomeProtocol::teErrorCode eError) = 0;
	virtual void ReplySetAutoUpdate		() = 0;
	virtual void ReplySetDomePosition	() = 0;
	virtual void ReplyGetDomeStatus		(DomeProtocol::teDomeStatus eState, uint16_t Position) = 0;
	virtual void ReplySetSlotPosition	() = 0;
	virtual void ReplyGetSlotStatus		(DomeProtocol::teSlotStatus eState) = 0;
public:	//debugging helpers, implement if needed
	virtual void DebugOutputReceived(const std::string& rsReceived) {}
private:	//helper
	unsigned int LocalInterpret(const std::string& rsReceived);
public:		//protocol interpreter
	void		Interpret(const std::string& rsReceived);
	void		Interpret(const unsigned char* pData, unsigned long ulDataLength);
	static void Interpret(DomeProtocolHandler& rHandler, const std::string& rsReceived);
	static void Interpret(DomeProtocolHandler& rHandler, const unsigned char* pData, unsigned long ulDataLength);
public:		//low level protocol string creator
	static std::string CreateString(DomeProtocol::teCommand eCommand, const std::string* psData = NULL);
public:		//high level protocol string creators
	static std::string CreateStringGetVersion		();
	static std::string CreateStringPanicStopAll		();
	static std::string CreateStringSetTraceLevel	(DomeProtocol::teTraceLevel eLevel);
	static std::string CreateStringSetAutoUpdate	(DomeProtocol::teAutoUpdate eUpdate);
	static std::string CreateStringSetDomePosition	(uint16_t Position);
	static std::string CreateStringGetDomeStatus	();
	static std::string CreateStringSetSlotPosition	(DomeProtocol::teSlotPosition ePosition);
	static std::string CreateStringGetSlotStatus	();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
