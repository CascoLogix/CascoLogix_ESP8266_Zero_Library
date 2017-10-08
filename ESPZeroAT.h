/**
 *  @file     ESPZeroAT.h
 *  @author   Clint Stevenson (Casco Logix)
 *  @license  GPLv3
 *
 *  @description
 *  This is a AT command library for use with the Espressif ESP8266 WiFi module
 *  running the NONOS AT firmware. This library specifically omits the use of
 *  software serial and other features not compatible with the Arduino Zero and
 *  other SAMD21-based Arduino platforms.
 *
 *  Copyright (C) 2017  CascoLogix (http://cascologix.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ESP_ZERO_AT_H_
#define _ESP_ZERO_AT_H_


#include <Arduino.h>
#include <ESP8266.h>
#include <HardwareSerial.h>
//#include <Stream.h>


class ESPZeroAT //: public Stream
{
public:
	ESPZeroAT();
	bool begin(HardwareSerial* ptrHWSerial, uint32_t baud);

	/*********************/
	/* Basic AT Commands */
	/*********************/
	bool test();
	bool reset();
	bool getVer(char ATversion[], char SDKversion[], char compileTime[]);
	bool deepSleep(uint32_t t_mS);
	bool disableEcho(void);
	bool enableEcho(void);
	bool restore(void);
	bool setUART(uint32_t baud, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl);
	// TODO: bool setDataBits();
	// TODO: bool setStopBits();
	// TODO: bool setParity();
	// TODO: bool setFlowControl();
	bool sleep(uint8_t sleepMode);
	bool wakeGPIO(bool trigEnable, uint8_t trigSourcePin, bool trigLogicLevel);
	bool wakeGPIO(bool trigEnable, uint8_t trigSourcePin, bool trigLogicLevel, uint8_t trigSourceFlag, bool awakeLogicLevel);
	// TODO: bool setRFpower();
	// TODO: bool setRFbyVDD33();
	// TODO: bool setRFautoTrace();
	// TODO: bool checkSystemRAM();
	bool checkSystemADC(uint16_t* adcVal);
	bool setGPIOmode(uint8_t pin, uint8_t mode, bool pullup);
	// TODO: bool getGPIOmode();
	bool setGPIOdir(uint8_t pin, bool dir);
	bool writeGPIO(uint8_t pin, bool level);
	bool readGPIO(uint8_t pin, bool* level);

	/*******************/
	/* WiFi Functions  */
	/*******************/
	bool setMode(uint8_t mode);
	bool getMode(uint8_t* mode);
	bool connect(const char * ssid, const char * pwd, uint8_t* errCode);
	bool configLAPcmd();
	bool getAP(apTable_t * apTable);
	bool disconnect();
	bool configSoftAP();
	bool getSoftAPconfig();
	bool getStationIP();
	bool enableDHCP();
	bool disableDHCP();
	bool queryDHCP();
	bool setSoftAPaddrRange();
	bool getSoftAPaddrRange();
	bool enableAutoConnectAP();
	bool disableAutoConnectAP();
	bool queryAutoConnectAP();
	bool setStationMACaddr();
	bool getStationMACaddr();
	bool setSoftAPmacAddr();
	bool getSoftAPmacAddr();
	bool setStationIP();
	bool getStationIP();
	bool setSoftAPaddr();
	bool getSoftAPaddr();
	bool startSmartConfig(uint8_t type);
	bool stopSmartConfig();
	bool enableWPS();
	bool disableWPS();
	bool configureMDNS();
	bool configureStationName();
	bool queryStationName();

	/*******************/
	/* TCP/IP Commands */
	/*******************/
	bool status();
	bool updateStatus();
	bool dnsFunction(char strDomainName[]);
	bool openConnection(int8_t linkID, char * strConnType, char * strRemoteIP, char * strRemotePort, uint16_t tKeepAlive);
	bool setSSLbufferSize();
	bool sendDataLen(int8_t linkID, uint16_t length, char * remoteIP, char * remotePort, uint8_t * ptrDataBuf);
	bool sendDataStr(int8_t linkID, uint16_t length, char * remoteIP, char * remotePort, char * strData);
	bool sendDataBuf(int8_t linkID, uint8_t segmentID, uint16_t length, uint8_t * ptrDataBuf);
	bool bufferReset(int8_t linkID);
	bool getBufferStatus(int8_t linkID);
	bool getSendStatus(int8_t linkID, uint8_t segmentID);
	bool closeConnection(int8_t linkID);
	bool getLocalIPaddr(char * ipAddress);
	bool enableMultipleConnection();
	bool disableMultipleConnection();
	bool queryMultipleConnection();
	bool createServer();
	bool deleteServer();
	bool setTransmissionMode(uint8_t mode);
	bool setTCPserverTimeout(uint16_t tTimeout);
	bool getTCPserverTimeout(uint16_t tTimeout);
	bool ping(char* IPaddress, uint16_t tTimeMs);
	bool WiFiSoftwareUpdate(uint8_t option);
	bool IPDinfoMode(uint8_t mode);
	bool setSNTPconfig();
	bool getSNTPconfig();
	bool getSNTPtime();
	bool setUserDefDNSserver();
	bool getUserDefDNSserver();

private:
    HardwareSerial* _ptrHWSerial;
    uint32_t _baud;
	//////////////////////////
	// Command Send/Receive //
	//////////////////////////
    void sendCommand(const char * cmd, const char digit);
	void sendCommand(const char * cmd, enum esp8266_command_type type = ESP8266_CMD_EXECUTE, const char * params = 0);
	//int16_t getResponse(const char * rsp, uint8_t* len);
	//int16_t getResponse(const char * rsp, uint8_t* len, uint32_t timeout);
	//int16_t readForResponses(const char * pass, const char * fail, uint32_t timeout);

	//////////////////
	// Buffer Stuff //
	//////////////////

	/*
	void clearBuffer(); 				// Reset buffer pointer, set all values to 0

	unsigned int readByteToBuffer();	// Read first byte from UART receive buffer and store it in rxBuffer.


	char * searchBuffer(const char * test);		// Search buffer for string [test]
	/// Success: Returns pointer to beginning of string
	/// Fail: returns NULL
	//! TODO: Fix this function so it searches circularly

	esp8266_status _status;

	uint8_t sync();
	*/
};

extern ESPZeroAT ESP8266;

#endif	// _ESP_ZERO_AT_H_
