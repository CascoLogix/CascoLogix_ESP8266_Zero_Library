



#include <CascoLogix_ESP8266_Zero_Library.h>

void CL_ESP8266::CL_ESP8266()
{
	
}

void CL_ESP8266::begin(Stream* port, uint32_t baud)
{
	_baud = baud;
	_port = port;

	_port->begin(_baud);			// This is the 'Serial.begin()' method
	
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
bool CL_ESP8266::test()
{
	_serial->print(ESP8266_AT_TEST); 		// Send AT
	_serial->print(ESP8266_AT_TERMINATE);	// Terminate command string

	if (readForResponse(RESPONSE_OK, COMMAND_RESPONSE_TIMEOUT) > 0)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool CL_ESP8266::reset()
{
	sendCommand(ESP8266_RESET); // Send AT+RST

	if (readForResponse(ESP8266_RESPONSE_READY, ESP8266_CMD_RESPONSE_TIMEOUT) > 0)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool CL_ESP8266::disableEcho()
{
	sendCommand(ESP8266_ECHO_DISABLE);

	if (readForResponse(ESP8266_RESPONSE_OK, ESP8266_CMD_RESPONSE_TIMEOUT) > 0)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool CL_ESP8266::enableEcho()
{
	sendCommand(ESP8266_ECHO_ENABLE);

	if (readForResponse(ESP8266_RESPONSE_OK, ESP8266_CMD_RESPONSE_TIMEOUT) > 0)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool CL_ESP8266::setBaud(uint32_t baud)
{
	char parameters[16];
	memset(parameters, 0, 16);
	// Constrain parameters:
	baud = constrain(baud, 110, 115200);

	// Put parameters into string
	sprintf(parameters, "%d,8,1,0,0", baud);

	// Send AT+UART=baud,databits,stopbits,parity,flowcontrol
	sendCommand(ESP8266_UART, ESP8266_CMD_SETUP, parameters);

	if (readForResponse(ESP8266_RESPONSE_OK, ESP8266_CMD_RESPONSE_TIMEOUT) > 0)
	{
		return true;
	}

	else
	{
		return false;
	}
}

int16_t CL_ESP8266::getVer(char ATversion[], char SDKversion[], char compileTime[])
{
	sendCommand(ESP8266_VERSION); // Send AT+GMR
	// Example Response: AT version:0.30.0.0(Jul  3 2015 19:35:49)\r\n (43 chars)
	//                   SDK version:1.2.0\r\n (19 chars)
	//                   compile time:Jul  7 2015 18:34:26\r\n (36 chars)
	//                   OK\r\n
	// (~101 characters)
	// Look for "OK":
	int16_t rsp = (readForResponse(ESP8266_RESPONSE_OK, ESP8266_CMD_RESPONSE_TIMEOUT) > 0);
	if (rsp > 0)
	{
		char *p, *q;

		// Look for "AT version" in the rxBuffer
		p = strstr(esp8266RxBuffer, "AT version:");

		if (p == NULL)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		p += strlen("AT version:");
		q = strchr(p, '\r'); // Look for \r

		if (q == NULL)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		strncpy(ATversion, p, q-p);

		// Look for "SDK version:" in the rxBuffer
		p = strstr(esp8266RxBuffer, "SDK version:");

		if (p == NULL)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		p += strlen("SDK version:");
		q = strchr(p, '\r'); // Look for \r

		if (q == NULL)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		strncpy(SDKversion, p, q-p);

		// Look for "compile time:" in the rxBuffer
		p = strstr(esp8266RxBuffer, "compile time:");

		if (p == NULL)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		p += strlen("compile time:");
		q = strchr(p, '\r'); // Look for \r

		if (q == NULL)
		{
			return ESP8266_RSP_UNKNOWN;
		}

		strncpy(compileTime, p, q-p);
	}

	return rsp;
}

//////////////////////////////////////////////////
// Private, Low-Level, Ugly, Hardware Functions //
//////////////////////////////////////////////////

void CL_ESP8266::sendCommand(const char * cmd, enum esp8266_command_type type, const char * params)
{
	_serial->print(ESP8266_AT_CMD);
	_serial->print(cmd);

	if (type == ESP8266_CMD_QUERY)
	{
		_serial->print('?');
	}

	else if (type == ESP8266_CMD_SETUP)
	{
		_serial->print("=");
		_serial->print(params);
	}

	_serial->print("\r\n");
}

int16_t CL_ESP8266::readForResponse(const char * rsp, uint32_t timeout)
{
	uint32_t timeIn = millis();	// Timestamp coming into function
	uint32_t received = 0; // received keeps track of number of chars read

	clearBuffer();	// Clear the class receive buffer (esp8266RxBuffer)
	while (timeIn + timeout > millis()) // While we haven't timed out
	{
		if (_serial->available()) // If data is available on UART RX
		{
			received += readByteToBuffer();

			if (searchBuffer(rsp))	// Search the buffer for goodRsp
			{
				return received;	// Return how number of chars read
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

int16_t CL_ESP8266::readForResponses(const char * pass, const char * fail, uint32_t timeout)
{
	uint32_t timeIn = millis();	// Timestamp coming into function
	uint32_t received = 0; // received keeps track of number of chars read

	clearBuffer();	// Clear the class receive buffer (esp8266RxBuffer)
	while (timeIn + timeout > millis()) // While we haven't timed out
	{
		if (_serial->available()) // If data is available on UART RX
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

//////////////////
// Buffer Stuff //
//////////////////
void CL_ESP8266::clearBuffer()
{
	memset(esp8266RxBuffer, '\0', ESP8266_RX_BUFFER_LEN);
	bufferHead = 0;
}

unsigned int CL_ESP8266::readByteToBuffer()
{
	// Read the data in
	char c = _serial->read();

	// Store the data in the buffer
	esp8266RxBuffer[bufferHead] = c;
	//! TODO: Don't care if we overflow. Should we? Set a flag or something?
	bufferHead = (bufferHead + 1) % ESP8266_RX_BUFFER_LEN;

	return 1;
}

char * CL_ESP8266::searchBuffer(const char * test)
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

CL_ESP8266 ESP8266;
