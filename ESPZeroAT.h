




#ifndef _CASCO_LOGIX_ESP8266_ZERO_LIBRARY_H_
#define _CASCO_LOGIX_ESP8266_ZERO_LIBRARY_H_


#include <Arduino.h>
#include <ESP8266.h>


class ESPZeroAT : public Stream
{
public:
	ESPZeroAT();
	void begin(HardwareSerial* ptrHWSerial, uint32_t baud);

	// Basic AT Commands
	bool test();
	bool reset();
	int16_t getVer(char ATversion[], char SDKversion[], char compileTime[]);
	bool disableEcho();
	bool enableEcho();
	bool setBaud(uint32_t baud);

	// WiFi Functions
	int16_t getMode();
	int16_t setMode(esp8266_wifi_mode mode);
	int16_t setMode(int8_t mode);
	int16_t connect(const char * ssid);
	int16_t connect(const char * ssid, const char * pwd);
	int16_t getAP(char * ssid);
	int16_t localMAC(char * mac);
	int16_t disconnect();
	IPAddress localIP();

	/////////////////////
	// TCP/IP Commands //
	/////////////////////
	int16_t status();
	int16_t updateStatus();
	int16_t tcpConnect(uint8_t linkID, const char * destination, uint16_t port, uint16_t keepAlive);
	int16_t tcpSend(uint8_t linkID, const uint8_t *buf, size_t size);
	int16_t close(uint8_t linkID);
	int16_t setTransferMode(uint8_t mode);
	int16_t setMux(uint8_t mux);
	int16_t configureTCPServer(uint16_t port, uint8_t create = 1);
	int16_t ping(IPAddress ip);
	int16_t ping(char * server);
private:
    HardwareSerial* _ptrHWSerial;
    uint32_t _baud;
	//////////////////////////
	// Command Send/Receive //
	//////////////////////////
	void sendCommand(const char * cmd, enum esp8266_command_type type = ESP8266_CMD_EXECUTE, const char * params = NULL);
	int16_t getResponse(const char * rsp, uint8_t* len);
	int16_t getResponse(const char * rsp, uint8_t* len, uint32_t timeout);
	int16_t readForResponses(const char * pass, const char * fail, uint32_t timeout);

	//////////////////
	// Buffer Stuff //
	//////////////////
	void clearBuffer(); 				// Reset buffer pointer, set all values to 0

	unsigned int readByteToBuffer();	// Read first byte from UART receive buffer and store it in rxBuffer.


	char * searchBuffer(const char * test);		// Search buffer for string [test]
	/// Success: Returns pointer to beginning of string
	/// Fail: returns NULL
	//! TODO: Fix this function so it searches circularly

	esp8266_status _status;

	uint8_t sync();
};

extern ESPZeroAT ESP8266;

#endif	// _CASCO_LOGIX_ESP8266_ZERO_LIBRARY_H_