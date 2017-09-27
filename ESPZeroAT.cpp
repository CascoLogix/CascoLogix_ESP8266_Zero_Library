



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
	sendCommand(ESP8266_GMR); // Send AT+GMR
	// Example Response: AT version:0.30.0.0(Jul  3 2015 19:35:49)\r\n (43 chars)
	//                   SDK version:1.2.0\r\n (19 chars)
	//                   compile time:Jul  7 2015 18:34:26\r\n (36 chars)
	//                   OK\r\n
	// (~101 characters)
	// Look for "OK":
	int16_t rsp = (_ptrHWSerial->find(ESP8266_RESPONSE_OK));

	/*
	if (rsp > 0)
	{
		char *p, *q;

		// Look for "AT version" in the rxBuffer
		p = strstr(esp8266RxBuffer, "AT version:");

		if (p == 0)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		p += strlen("AT version:");
		q = strchr(p, '\r'); // Look for \r

		if (q == 0)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		strncpy(ATversion, p, q-p);

		// Look for "SDK version:" in the rxBuffer
		p = strstr(esp8266RxBuffer, "SDK version:");

		if (p == 0)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		p += strlen("SDK version:");
		q = strchr(p, '\r'); // Look for \r

		if (q == 0)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		strncpy(SDKversion, p, q-p);

		// Look for "compile time:" in the rxBuffer
		p = strstr(esp8266RxBuffer, "compile time:");

		if (p == 0)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		p += strlen("compile time:");
		q = strchr(p, '\r'); // Look for \r

		if (q == 0)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		strncpy(compileTime, p, q-p);
	}
	*/

	return rsp;
}

bool ESPZeroAT::disableEcho()
{
	sendCommand(ESP8266_ATE0);

	if (_ptrHWSerial->find(ESP8266_RESPONSE_OK))
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool ESPZeroAT::enableEcho()
{
	sendCommand(ESP8266_ATE1);

	if (_ptrHWSerial->find(ESP8266_RESPONSE_OK))
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool ESPZeroAT::setBaud(uint32_t baud)
{
	// Send AT+UART=baud,databits,stopbits,parity,flowcontrol
	//sendCommand();

	if (_ptrHWSerial->find(ESP8266_RESPONSE_OK))
	{
		return true;
	}

	else
	{
		return false;
	}
}

//////////////////////////////////////////////////
// Private, Low-Level, Ugly, Hardware Functions //
//////////////////////////////////////////////////

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
