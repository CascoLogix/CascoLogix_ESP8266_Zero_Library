/**
 *  @file     ESPZeroAT.cpp
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

#include <ESPZeroAT.h>

ESPZeroAT::ESPZeroAT()
{
	_baud = 115200;
	_ptrHWSerial = 0;
}

bool ESPZeroAT::begin(HardwareSerial* ptrHWSerial, uint32_t baud)
{
	_baud = baud;
	_ptrHWSerial = ptrHWSerial;

	_ptrHWSerial->begin(_baud);			// This is the 'Serial.begin()' method
	
	if (this->test())
	{
		return true;
	}

	else
	{
		return false;
	}
}


/*************************************************************************************/
/* Basic AT Commands 																 */
/*************************************************************************************/
bool ESPZeroAT::test()
{
	_ptrHWSerial->print(ESP8266_AT_TEST); 		// Send AT
	_ptrHWSerial->print(ESP8266_AT_TERMINATE);	// Terminate command string

	if (_ptrHWSerial->find(ESP8266_RESPONSE_OK))
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool ESPZeroAT::reset()
{
	sendCommand(ESP8266_RESET); // Send AT+RST

	if (_ptrHWSerial->find(ESP8266_RESPONSE_READY))
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool ESPZeroAT::getVer(char ATversion[], char SDKversion[], char compileTime[])
{
	// Example Response: AT version:0.30.0.0(Jul  3 2015 19:35:49)\r\n (43 chars)
	//                   SDK version:1.2.0\r\n (19 chars)
	//                   compile time:Jul  7 2015 18:34:26\r\n (36 chars)
	//                   OK\r\n
	// (~101 characters)
	// Look for "OK":

	int16_t bytesRead = 0;
	int16_t totalBytesRead = 0;
	uint8_t findResult = 0;

	struct {
		char* pStrResponse;
		char* pStrResult;
	} parseTable[] = {
	//	{pStrResponse, pStrResult},
		{"AT version:", ATversion},
		{"SDK version:", SDKversion},
		{"compile time:", compileTime}
	};

	sendCommand(ESP8266_GMR); // Send AT+GMR

	uint8_t idx;
	for(idx = 0; idx < 3; idx++)
	{
		findResult = _ptrHWSerial->find(parseTable[idx].pStrResponse);

		if(findResult)
		{
			bytesRead = _ptrHWSerial->readBytesUntil('\r', parseTable[idx].pStrResult, 30);
			totalBytesRead += bytesRead;

			*(parseTable[idx].pStrResult + bytesRead - 1) = 0;	// Replace '\r' with null terminator

			_ptrHWSerial->read();	// Read '\n' character
			totalBytesRead++;
		}
	}

	findResult = _ptrHWSerial->find(ESP8266_RESPONSE_OK);

	return totalBytesRead;
}

bool ESPZeroAT::deepSleep(uint32_t t_mS)
{
	char strTime_mS[12];

	itoa(t_mS, strTime_mS, 10);
	sendCommand(ESP8266_GSLP, ESP8266_CMD_SETUP, strTime_mS);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::disableEcho()
{
	sendCommand(ESP8266_ATE0);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::enableEcho()
{
	sendCommand(ESP8266_ATE1);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::restore()
{
	sendCommand(ESP8266_RESTORE);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::setUART(uint32_t baud, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl)
{
	// TODO
	// Send AT+UART_DEF=baud,databits,stopbits,parity,flowcontrol
	// Example: AT+UART_DEF=115200,8,1,0,0

	char params[12] = "";
	uint8_t stringIndex = 0;

	itoa(baud, params, 10);
	stringIndex = strlen(params);
	params[stringIndex++] = dataBits + '0';
	params[stringIndex++] = stopBits + '0';
	params[stringIndex++] = parity + '0';
	params[stringIndex++] = flowControl + '0';
	params[stringIndex] = 0;

	sendCommand(ESP8266_UART_DEF, ESP8266_CMD_SETUP, params);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::sleep(uint8_t sleepMode)
{
	sendCommand(ESP8266_SLEEP, sleepMode);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::wakeGPIO(bool trigEnable, uint8_t trigSourcePin, bool trigLogicLevel)
{
	char params[8] = "";
	uint8_t stringIndex = 0;

	params[stringIndex++] = trigEnable + '0';

	itoa(trigSourcePin, &params[stringIndex], 10);
	stringIndex = strlen(params);

	params[stringIndex++] = trigLogicLevel + '0';

	params[stringIndex] = 0;

	sendCommand(ESP8266_WAKEUPGPIO, ESP8266_CMD_SETUP, params);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::wakeGPIO(bool trigEnable, uint8_t trigSourcePin, bool trigLogicLevel, uint8_t trigSourceFlag, bool awakeLogicLevel)
{
	char params[8] = "";
	uint8_t stringIndex = 0;

	params[stringIndex++] = trigEnable + '0';

	itoa(trigSourcePin, &params[stringIndex], 10);
	stringIndex = strlen(params);

	params[stringIndex++] = trigLogicLevel + '0';

	itoa(trigSourceFlag, &params[stringIndex], 10);
	stringIndex = strlen(params);

	params[stringIndex++] = awakeLogicLevel + '0';

	params[stringIndex] = 0;

	sendCommand(ESP8266_WAKEUPGPIO, ESP8266_CMD_SETUP, params);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

/* TODO
bool ESPZeroAT::setRFpower()
{

}
*/

/* TODO
bool ESPZeroAT::setRFbyVDD33()
{

}
*/

/* TODO
bool ESPZeroAT::setRFautoTrace()
{

}
*/

/* TODO
bool ESPZeroAT::checkSystemRAM()
{

}
*/

bool ESPZeroAT::checkSystemADC(uint16_t* adcVal)
{
	int16_t bytesRead = 0;
	int16_t totalBytesRead = 0;
	uint8_t findResult = 0;
	char strResponse[strlen(ESP8266_SYSADC) + 3] = "+";
	char strResult[5] = "";

	strcat(strResponse, ESP8266_SYSADC);
	strcat(strResponse, ":");

	sendCommand(ESP8266_SYSADC, ESP8266_CMD_QUERY);

	findResult = _ptrHWSerial->find(strResponse);

	if(findResult)
	{
		bytesRead = _ptrHWSerial->readBytesUntil('\r', strResult, 5);

		if(bytesRead > 0)
		{
			strResult[bytesRead - 1] = 0;	// Replace '\r' with null terminator
			*adcVal = atoi(strResult);
		}
	}

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::setGPIOmode(uint8_t pin, uint8_t mode, bool pullup)
{
	char params[6] = "";
	uint8_t stringIndex = 0;

	itoa(pin, params, 10);
	stringIndex = strlen(params);

	itoa(mode, &params[stringIndex], 10);
	stringIndex = strlen(params);

	params[stringIndex++] = pullup + '0';

	params[stringIndex] = 0;

	sendCommand(ESP8266_SYSIOSETCFG, ESP8266_CMD_SETUP, params);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

/*
bool ESPZeroAT::getGPIOmode()
{
	// TODO

	return 0;
}
*/

bool ESPZeroAT::setGPIOdir(uint8_t pin, bool dir)
{
	char params[4] = "";
	uint8_t stringIndex = 0;

	itoa(pin, params, 10);
	stringIndex = strlen(params);

	params[stringIndex++] = dir + '0';

	params[stringIndex] = 0;

	sendCommand(ESP8266_SYSGPIODIR, ESP8266_CMD_SETUP, params);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::writeGPIO(uint8_t pin, bool level)
{
	char params[4] = "";
	uint8_t stringIndex = 0;

	itoa(pin, params, 10);
	stringIndex = strlen(params);

	params[stringIndex++] = level + '0';

	params[stringIndex] = 0;

	sendCommand(ESP8266_SYSGPIOWRITE, ESP8266_CMD_SETUP, params);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::readGPIO(uint8_t pin, bool* level)
{
	int16_t bytesRead = 0;
	int16_t totalBytesRead = 0;
	uint8_t findResult = 0;
	char strResponse[strlen(ESP8266_SYSGPIOREAD) + 3] = "+";
	char strResult = 0;
	char params[4] = "";
	uint8_t stringIndex = 0;

	strcat(strResponse, ESP8266_SYSGPIOREAD);
	strcat(strResponse, ":");

	itoa(pin, params, 10);
	stringIndex = strlen(params);

	params[stringIndex] = 0;

	sendCommand(ESP8266_SYSGPIOREAD, ESP8266_CMD_SETUP, params);

	findResult = _ptrHWSerial->find(strResponse);

	if(findResult)
	{
		_ptrHWSerial->find(',');			// Discard pin response
		_ptrHWSerial->find(',');			// Discard dir response
		strResult = _ptrHWSerial->read();	// Get level response

		*level = strResult - '0';
	}

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}


/*************************************************************************************/
/* WiFi Commands 																 */
/*************************************************************************************/
bool ESPZeroAT::setMode(uint8_t mode)
{
	bool findResult = 0;
	char strResponse[strlen(ESP8266_CWMODE_DEF) + 3] = "+";

	mode += '0';
	sendCommand(ESP8266_CWMODE_DEF, ESP8266_CMD_SETUP, (char*)&mode);

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::getMode(uint8_t* mode)
{
	bool findResult = 0;
	char strResponse[strlen(ESP8266_CWMODE_DEF) + 3] = "+";

	sendCommand(ESP8266_CWMODE_DEF, ESP8266_CMD_QUERY);

	strcat(strResponse, ESP8266_CWMODE_DEF);
	strcat(strResponse, ":");

	findResult = _ptrHWSerial->find(strResponse);

	if(findResult)
	{
		*mode = _ptrHWSerial->read() - '0';	// Get level response
	}

	return _ptrHWSerial->find(ESP8266_RESPONSE_OK);
}

bool ESPZeroAT::connect(const char * ssid, const char * pwd, uint8_t * errCode)
{
	bool retVal = 0;
	char strResponse[strlen(ESP8266_CWJAP_CUR) + 6] = "+"; 	// Add 6 for '+', ':', error#, '\r', '\n' and null
	char strCmdResponse[strlen(ESP8266_CWJAP_CUR) + 6 + strlen(ESP8266_RESPONSE_ERROR)] = "";
	char params[MAX_SSID_LEN + MAX_PWD_LEN + 2] = "";		// Add 2 for ',' and null
	uint8_t stringIndex = 0;

	strcat(params, ssid);
	strcat(params, ",");
	strcat(params, pwd);

	sendCommand(ESP8266_CWJAP_CUR, ESP8266_CMD_SETUP, params);

	strcat(strResponse, ESP8266_CWJAP_CUR);
	strcat(strResponse, ":");

	stringIndex = _ptrHWSerial->readBytesUntil('\r', strCmdResponse, 64);	// Read serial characters into string and return #bytes read

	if(strstr(strCmdResponse, ESP8266_RESPONSE_OK))
	{
		retVal = 1;
		*errCode = ESP8266_AP_CONN_ERR_NONE;
	}

	else
	{
		retVal = 0;
		stringIndex = strchr(strCmdResponse, ':');			// Get index of ':' character in string
		*errCode = strCmdResponse[stringIndex + 1] - '0'; 	// Get ASCII number following ':' and convert to numeric value
	}

	return retVal;
}

bool ESPZeroAT::configLAPcmd()
{
	// TODO: Not yet implemented. This whole function needs written
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getAP(apTable_t * apTable = 0)
{
	bool retVal = 0;
	char strResponse[] = "";
	char strCmdResponse[] = "";
	char params[] = "";
	uint8_t stringIndex = 0;

	// Prepare parameter string
	strcat(params, ssid);
	strcat(params, ",");
	strcat(params, pwd);

	// Send command with parameters
	sendCommand(ESP8266_CWJAP_CUR, ESP8266_CMD_SETUP, params);

	// Prepare response string
	strcat(strResponse, ESP8266_CWJAP_CUR);
	strcat(strResponse, ":");

	// Get response
	stringIndex = _ptrHWSerial->readBytesUntil('\r', strCmdResponse, 64);	// Read serial characters into string and return #bytes read

	// Check response
	if(strstr(strCmdResponse, ESP8266_RESPONSE_OK))
	{
		retVal = 1;
		*errCode = ESP8266_AP_CONN_ERR_NONE;
	}

	else
	{
		retVal = 0;
		stringIndex = strchr(strCmdResponse, ':');			// Get index of ':' character in string
		*errCode = strCmdResponse[stringIndex + 1] - '0'; 	// Get ASCII number following ':' and convert to numeric value
	}

	return retVal;
}

bool ESPZeroAT::disconnect()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::configSoftAP()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getSoftAPconfig()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getStationIP()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::enableDHCP()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::disableDHCP()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::queryDHCP()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::setSoftAPaddrRange()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getSoftAPaddrRange()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::enableAutoConnectAP()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::disableAutoConnectAP()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::queryAutoConnectAP()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::setStationMACaddr()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getStationMACaddr()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::setSoftAPmacAddr()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getSoftAPmacAddr()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::setStationIP()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getStationIP()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::setSoftAPaddr()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getSoftAPaddr()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::startSmartConfig(uint8_t type = 0)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::stopSmartConfig()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::enableWPS()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::disableWPS()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::configureMDNS()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::configureStationName()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::queryStationName()
{
	bool retVal = 0;

	return retVal;
}


/*************************************************************************************/
/* TCP/IP Commands 																 */
/*************************************************************************************/
bool ESPZeroAT::status()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::updateStatus()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::dnsFunction(char strDomainName[])
{
	// MAX_DNS_NAME_LEN
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::openConnection(int8_t linkID = -1, char * strConnType, char * strRemoteIP, char * strRemotePort, uint16_t tKeepAlive = 0)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::setSSLbufferSize()
{	// Can be either 2048 or 4096
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::sendDataLen(int8_t linkID = -1, uint16_t length, char * remoteIP = 0, char * remotePort = 0, uint8_t * ptrDataBuf)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::sendDataStr(int8_t linkID = -1, uint16_t length, char * remoteIP = 0, char * remotePort = 0, char * ptrDataBuf)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::sendDataBuf(int8_t linkID = -1, uint8_t segmentID, uint16_t length, uint8_t * ptrDataBuf)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::bufferReset(int8_t linkID = -1)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getBufferStatus(int8_t linkID = -1)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getSendStatus(int8_t linkID = -1, uint8_t segmentID)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::closeConnection(int8_t linkID = -1)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getLocalIPaddr(char * ipAddress)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::enableMultipleConnection()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::disableMultipleConnection()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::queryMultipleConnection()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::createServer()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::deleteServer()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::setTransmissionMode(uint8_t mode)
{
	bool retVal = 0;

	return retVal;
}

/*
bool ESPZeroAT::saveTransparentMode()
{
	bool retVal = 0;

	return retVal;
}
*/

bool ESPZeroAT::setTCPserverTimeout(uint16_t tTimeout)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getTCPserverTimeout(uint16_t tTimeout)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::ping(char* IPaddress, uint16_t tTimeMs)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::WiFiSoftwareUpdate(uint8_t option)
{
	bool retVal = 0;

	return retVal;
}

/*
bool ESPZeroAT::IPDinfoMode(uint8_t mode)
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::receiveNetworkData()
{
	bool retVal = 0;

	return retVal;
}
*/

bool ESPZeroAT::setSNTPconfig()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getSNTPconfig()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getSNTPtime()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::setUserDefDNSserver()
{
	bool retVal = 0;

	return retVal;
}

bool ESPZeroAT::getUserDefDNSserver()
{
	bool retVal = 0;

	return retVal;
}


//////////////////////////////////////////////////
// Private, Low-Level, Ugly, Hardware Functions //
//////////////////////////////////////////////////

void ESPZeroAT::sendCommand(const char * cmd, const char digit)
{
	_ptrHWSerial->print(ESP8266_AT_CMD);
	_ptrHWSerial->print(cmd);

	_ptrHWSerial->print("=");
	_ptrHWSerial->print(digit);

	_ptrHWSerial->print("\r\n");
}

void ESPZeroAT::sendCommand(const char * cmd, enum esp8266_command_type type, const char * params)
{
	_ptrHWSerial->print(ESP8266_AT_CMD);
	_ptrHWSerial->print(cmd);

	if (type == ESP8266_CMD_QUERY)
	{
		_ptrHWSerial->print('?');
	}

	else if (type == ESP8266_CMD_SETUP)
	{
		_ptrHWSerial->print("=");
		_ptrHWSerial->print(params);
	}

	_ptrHWSerial->print("\r\n");
}

/*
int16_t ESPZeroAT::getResponse(const char * rsp, uint8_t* len)
{
	uint32_t received = 0; // received keeps track of number of chars read

	clearBuffer();	// Clear the class receive buffer (esp8266RxBuffer)

	if (received > 0) // If we received any characters
	{
		return ESP8266_RSP_UNKNOWN; // Return unkown response error code
	}

	else // If we haven't received any characters
	{
		return ESP8266_RSP_TIMEOUT; // Return the timeout error code
	}
}
*/

/*
int16_t ESPZeroAT::getResponse(const char * rsp, uint8_t* len, uint32_t timeout)
{
	int16_t retVal = 0;

	_ptrHWSerial->setTimeout(timeout);			// Sets timeout
	retVal = _ptrHWSerial->find(rsp);			// Look for "\r\n"
	*len = _ptrHWSerial->available();			// Get #bytes in RX buffer
	_ptrHWSerial->readString(rsp);				// Read string from RX buffer
	return retVal;								// Return status
}
*/

/*
int16_t ESPZeroAT::readForResponses(const char * pass, const char * fail, uint32_t timeout)
{
	uint32_t timeIn = millis();	// Timestamp coming into function
	uint32_t received = 0; // received keeps track of number of chars read

	clearBuffer();	// Clear the class receive buffer (esp8266RxBuffer)
	while (timeIn + timeout > millis()) // While we haven't timed out
	{
		if (_ptrHWSerial->available()) // If data is available on UART RX
		{
			received += readByteToBuffer();

			if (searchBuffer(pass))	// Search the buffer for goodRsp
			{
				return received;	// Return how number of chars read
			}

			if (searchBuffer(fail))
			{
				return ESP8266_RSP_FAIL;
			}
		}
	}

	if (received > 0) // If we received any characters
	{
		return ESP8266_RSP_UNKNOWN; // Return unkown response error code
	}

	else // If we haven't received any characters
	{
		return ESP8266_RSP_TIMEOUT; // Return the timeout error code
	}
}
*/

//////////////////
// Buffer Stuff //
//////////////////
/*
void ESPZeroAT::clearBuffer()
{
	memset(esp8266RxBuffer, '\0', ESP8266_RX_BUFFER_LEN);
	bufferHead = 0;
}
*/

/*
unsigned int ESPZeroAT::readByteToBuffer()
{
	// Read the data in
	char c = _ptrHWSerial->read();

	// Store the data in the buffer
	esp8266RxBuffer[bufferHead] = c;
	//! TODO: Don't care if we overflow. Should we? Set a flag or something?
	bufferHead = (bufferHead + 1) % ESP8266_RX_BUFFER_LEN;

	return 1;
}
*/

/*
char * ESPZeroAT::searchBuffer(const char * test)
{
	int bufferLen = strlen((const char *)esp8266RxBuffer);
	// If our buffer isn't full, just do an strstr
	if (bufferLen < ESP8266_RX_BUFFER_LEN)
		return strstr((const char *)esp8266RxBuffer, test);
	else
	{	//! TODO
		// If the buffer is full, we need to search from the end of the
		// buffer back to the beginning.
		int testLen = strlen(test);
		for (int i = 0; i < ESP8266_RX_BUFFER_LEN; i++)
		{

		}
	}
}
*/

ESPZeroAT ESP8266;
