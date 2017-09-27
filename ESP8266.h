/**
 *  @file     ESP8266.h
 *  @author   Clint Stevenson (Casco Logix)
 *  @license  GPLv3
 *
 *  @description
 *  This is a simple header file containing the standard AT command strings and
 *  responses for the Espressif ESP8266 WiFi module running the NONOS AT firmware.
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

/*************************************************************************************/
/*

From: ESP8266 AT Instruction Set
Version: 2.1.0

3.1. Overview

AT Tests AT startup.
AT+RST Restarts the module.
AT+GMR Checks version information.
AT+GSLP Enters Deep-sleep mode.
ATE Configures echoing of AT commands.
AT+RESTORE Restores the factory default settings of the module.
AT+UART UART configuration. [@deprecated]
AT+UART_CUR The current UART configuration.
AT+UART_DEF The default UART configuration, saved in flash.
AT+SLEEP Configures the sleep modes.
AT+WAKEUPGPIO Configures a GPIO to wake ESP8266 up from Light-sleep mode.
AT+RFPOWER Sets the maximum value of the RF TX Power.
AT+RFVDD Sets the RF TX Power according to VDD33.
AT+RFAUTOTRACE Sets RF frequency offset trace.
AT+SYSRAM Checks the available RAM size.
AT+SYSADC Checks the ADC value.
AT+SYSIOSETCFG Sets configuration of IO pins.
AT+SYSIOGETCFG Gets configuration of IO pins.
AT+SYSGPIODIR Configures the direction of GPIO.
AT+SYSGPIOWRITE Configures the GPIO output level
AT+SYSGPIOREAD Checks the GPIO input level.

4.1. Overview

AT+CWMODE Sets the Wi-Fi mode (Station/AP/Station+AP). [@deprecated]
AT+CWMODE_CUR Sets the Wi-Fi mode (Station/AP/Station+AP); configuration not saved in the flash.
AT+CWMODE_DEF Sets the default Wi-Fi mode (Station/AP/Station+AP); configuration saved in the flash.
AT+CWJAP Connect to an AP. [@deprecated]
AT+CWJAP_CUR Connects to an AP; configuration not saved in the flash.
AT+CWJAP_DEF Connects to an AP; configuration saved in the flash.
AT+CWLAPOPT Sets the configuration of command AT+CWLAP.
AT+CWLAP Lists available APs.
AT+CWQAP Disconnects from an AP.
AT+CWSAP Sets the configuration of the ESP8266 SoftAP. [@deprecated]
AT+CWSAP_CUR Sets the current configuration of the ESP8266 SoftAP; configuration not saved in the flash.
AT+CWSAP_DEF Sets the configuration of the ESP8266 SoftAP; configuration saved in the flash.
AT+CWLIF Gets the Station IP to which the ESP8266 SoftAP is connected.
AT+CWDHCP Enables/Disables DHCP. [@deprecated]
AT+CWDHCP_CUR Enables/Disables DHCP; configuration not saved in the flash.
AT+CWDHCP_DEF Enable/Disable DHCP; configuration saved in the flash.
AT+CWDHCPS_CUR Sets the IP range of the DHCP server; configuration not saved in the flash.
AT+CWDHCPS_DEF Sets the IP range of the DHCP server; configuration saved in the flash.
AT+CWAUTOCONN Connects to an AP automatically on power-up.
AT+CIPSTAMAC Sets the MAC address of the ESP8266 Station. [@deprecated]
AT+CIPSTAMAC_CUR Sets the MAC address of the ESP8266 Station; configuration not saved in the flash.
AT+CIPSTAMAC_DEF Sets the MAC address of ESP8266 station; configuration saved in the flash.
AT+CIPAPMAC Sets the MAC address of the ESP8266 SoftAP. [@deprecated]
AT+CIPAPMAC_CUR Sets the MAC address of the ESP8266 SoftAP; configuration not saved in the flash.
AT+CIPAPMAC_DEF Sets the MAC address of the ESP8266 SoftAP; configuration saved in the flash.
AT+CIPSTA Sets the IP address of the ESP8266 Station. [@deprecated]
AT+CIPSTA_CUR Sets the IP address of the ESP8266 Station; configuration not saved in the flash.
AT+CIPSTA_DEF Sets the IP address of the ESP8266 Station; configuration saved in the flash.
AT+CIPAP Sets the IP address of ESP8266 SoftAP. [@deprecated]
AT+CIPAP_CUR Sets the IP address of ESP8266 SoftAP; configuration not saved in the flash.
AT+CIPAP_DEF Sets the IP address of ESP8266 SoftAP; configuration saved in the flash.
AT+CWSTARTSMART Starts SmartConfig.
AT+CWSTOPSMART Stops SmartConfig.
AT+CWSTARTDISCOVER Enables the mode that ESP8266 can be found by WeChat.
AT+CWSTOPDISCOVER Disables the mode that ESP8266 can be found by WeChat.
AT+WPS Sets the WPS function.
AT+MDNS Sets the MDNS function.
AT+CWHOSTNAME Sets the host name of the ESP8266 Station.

5.1. Overview

AT+CIPSTATUS Gets the connection status
AT+CIPDOMAIN DNS function
AT+CIPSTART Establishes TCP connection, UDP transmission or SSL connection
AT+CIPSSLSIZE Sets the size of SSL buffer
AT+CIPSEND Sends data
AT+CIPSENDEX Sends data when length of data is <length>, or when \0 appears in the data
AT+CIPSENDBUF Writes data into TCP-send-buffer
AT+CIPBUFRESET Resets the segment ID count
AT+CIPBUFSTATUS Checks the status of TCP-send-buffer
AT+CIPCHECKSEQ Checks if a specific segment is sent or not
AT+CIPCLOSE Closes TCP/UDP/SSL connection
AT+CIFSR Gets the local IP address
AT+CIPMUX Configures the multiple connections mode
AT+CIPSERVER Deletes/Creates a TCP server
AT+CIPMODE Configures the transmission mode
AT+SAVETRANSLINK Saves the transparent transmission link in the flash
AT+CIPSTO Sets timeout when ESP8266 runs as TCP server
AT+PING Ping packets
AT+CIUPDATE Upgrades the software through network
AT+CIPDINFO Shows remote IP and remote port with +IPD
AT+CIPSNTPCFG Configures the time domain and SNTP server.
AT+CIPSNTPTIME Queries the SNTP time.
AT+CIPDNS_CUR Sets user-defined DNS servers; configuration not saved in the flash
AT+CIPDNS_DEF Sets user-defined DNS servers; configuration saved in the flash

*/
/*************************************************************************************/

#ifndef _ESP8266_H_
#define _ESP8266_H_

#include <stdint.h>
#include <IPAddress.h>


/*************************************************************************************/
// Command Response Timeouts														 */
/*************************************************************************************/
#define ESP8266_CMD_RESPONSE_TIMEOUT 	1000
#define ESP8266_CMD_PING_TIMEOUT 		3000
#define ESP8266_WIFI_CONNECT_TIMEOUT 	30000
#define ESP8266_CMD_RESET_TIMEOUT 		5000
#define ESP8266_CLIENT_CONNECT_TIMEOUT 	5000

#define ESP8266_MAX_SOCK_NUM 			5
#define ESP8266_SOCK_NOT_AVAIL 			255


/*************************************************************************************/
/* Basic AT Commands																 */
/*************************************************************************************/
#define ESP8266_AT_TEST				"AT" 				// Tests AT startup.
#define ESP8266_AT_TERMINATE		"\r\n"				// Terminates the AT command
#define ESP8266_AT_CMD				"AT+" 				// Prefix to all AT commands.
#define ESP8266_RESET				"RST" 				// Restarts the module.
#define ESP8266_GMR 				"GMR" 				// Checks version information.
#define ESP8266_GSLP 				"GSLP" 				// Enters Deep-sleep mode.
#define ESP8266_ATE1 				"ATE1" 				// Configures echoing of AT commands to enabled.
#define ESP8266_ATE0 				"ATE0" 				// Configures echoing of AT commands to disabled.
#define ESP8266_RESTORE 			"RESTORE" 			// Restores the factory default settings of the module.
#define ESP8266_UART 				"UART" 				// UART configuration. [@deprecated]
#define ESP8266_UART_CUR 			"UART_CUR" 			// The current UART configuration.
#define ESP8266_UART_DEF 			"UART_DEF" 			// The default UART configuration, saved in flash.
#define ESP8266_SLEEP 				"SLEEP" 			// Configures the sleep modes.
#define ESP8266_WAKEUPGPIO 			"WAKEUPGPIO" 		// Configures a GPIO to wake ESP8266 up from Light-sleep mode.
#define ESP8266_RFPOWER 			"RFPOWER" 			// Sets the maximum value of the RF TX Power.
#define ESP8266_RFVDD 				"RFVDD" 			// Sets the RF TX Power according to VDD33.
#define ESP8266_RFAUTOTRACE 		"RFAUTOTRACE" 		// Sets RF frequency offset trace.
#define ESP8266_SYSRAM 				"SYSRAM" 			// Checks the available RAM size.
#define ESP8266_SYSADC 				"SYSADC" 			// Checks the ADC value.
#define ESP8266_SYSIOSETCFG 		"SYSIOSETCFG" 		// Sets configuration of IO pins.
#define ESP8266_SYSIOGETCFG 		"SYSIOGETCFG" 		// Gets configuration of IO pins.
#define ESP8266_SYSGPIODIR 			"SYSGPIODIR" 		// Configures the direction of GPIO.
#define ESP8266_SYSGPIOWRITE		"SYSGPIOWRITE" 		// Configures the GPIO output level
#define ESP8266_SYSGPIOREAD 		"SYSGPIOREAD" 		// Checks the GPIO input level.


/*************************************************************************************/
// WiFi AT Commands
/*************************************************************************************/
#define ESP8266_CWMODE 				"CWMODE" 			// Sets the Wi-Fi mode (Station/AP/Station+AP). [@deprecated]
#define ESP8266_CWMODE_CUR 			"CWMODE_CUR" 		// Sets the Wi-Fi mode (Station/AP/Station+AP); configuration not saved in the flash.
#define ESP8266_CWMODE_DEF 			"CWMODE_DEF" 		// Sets the default Wi-Fi mode (Station/AP/Station+AP); configuration saved in the flash.
#define ESP8266_CWJAP 				"CWJAP" 			// Connect to an AP. [@deprecated]
#define ESP8266_CWJAP_CUR 			"CWJAP_CUR" 		// Connects to an AP; configuration not saved in the flash.
#define ESP8266_CWJAP_DEF 			"CWJAP_DEF" 		// Connects to an AP; configuration saved in the flash.
#define ESP8266_CWLAPOPT 			"CWLAPOPT" 			// Sets the configuration of command AT+CWLAP.
#define ESP8266_CWLAP 				"CWLAP" 			// Lists available APs.
#define ESP8266_CWQAP 				"CWQAP" 			// Disconnects from an AP.
#define ESP8266_CWSAP 				"CWSAP" 			// Sets the configuration of the ESP8266 SoftAP. [@deprecated]
#define ESP8266_CWSAP_CUR 			"CWSAP_CUR" 		// Sets the current configuration of the ESP8266 SoftAP; configuration not saved in the flash.
#define ESP8266_CWSAP_DEF 			"CWSAP_DEF" 		// Sets the configuration of the ESP8266 SoftAP; configuration saved in the flash.
#define ESP8266_CWLIF 				"CWLIF" 			// Gets the Station IP to which the ESP8266 SoftAP is connected.
#define ESP8266_CWDHCP 				"CWDHCP" 			// Enables/Disables DHCP. [@deprecated]
#define ESP8266_CWDHCP_CUR 			"CWDHCP_CUR" 		// Enables/Disables DHCP; configuration not saved in the flash.
#define ESP8266_CWDHCP_DEF 			"CWDHCP_DEF" 		// Enable/Disable DHCP; configuration saved in the flash.
#define ESP8266_CWDHCPS_CUR 		"CWDHCPS_CUR" 		// Sets the IP range of the DHCP server; configuration not saved in the flash.
#define ESP8266_CWDHCPS_DEF 		"CWDHCPS_DEF" 		// Sets the IP range of the DHCP server; configuration saved in the flash.
#define ESP8266_CWAUTOCONN 			"CWAUTOCONN" 		// Connects to an AP automatically on power-up.
#define ESP8266_CIPSTAMAC 			"CIPSTAMAC" 		// Sets the MAC address of the ESP8266 Station. [@deprecated]
#define ESP8266_CIPSTAMAC_CUR 		"CIPSTAMAC_CUR" 	// Sets the MAC address of the ESP8266 Station; configuration not saved in the flash.
#define ESP8266_CIPSTAMAC_DEF 		"CIPSTAMAC_DEF" 	// Sets the MAC address of ESP8266 station; configuration saved in the flash.
#define ESP8266_CIPAPMAC 			"CIPAPMAC" 			// Sets the MAC address of the ESP8266 SoftAP. [@deprecated]
#define ESP8266_CIPAPMAC_CUR 		"CIPAPMAC_CUR" 		// Sets the MAC address of the ESP8266 SoftAP; configuration not saved in the flash.
#define ESP8266_CIPAPMAC_DEF 		"CIPAPMAC_DEF" 		// Sets the MAC address of the ESP8266 SoftAP; configuration saved in the flash.
#define ESP8266_CIPSTA 				"CIPSTA" 			// Sets the IP address of the ESP8266 Station. [@deprecated]
#define ESP8266_CIPSTA_CUR 			"CIPSTA_CUR" 		// Sets the IP address of the ESP8266 Station; configuration not saved in the flash.
#define ESP8266_CIPSTA_DEF 			"CIPSTA_DEF" 		// Sets the IP address of the ESP8266 Station; configuration saved in the flash.
#define ESP8266_CIPAP 				"CIPAP" 			// Sets the IP address of ESP8266 SoftAP. [@deprecated]
#define ESP8266_CIPAP_CUR 			"CIPAP_CUR" 		// Sets the IP address of ESP8266 SoftAP; configuration not saved in the flash.
#define ESP8266_CIPAP_DEF 			"CIPAP_DEF" 		// Sets the IP address of ESP8266 SoftAP; configuration saved in the flash.
#define ESP8266_CWSTARTSMART 		"CWSTARTSMART" 		// Starts SmartConfig.
#define ESP8266_CWSTOPSMART 		"CWSTOPSMART" 		// Stops SmartConfig.
#define ESP8266_CWSTARTDISCOVER		"CWSTARTDISCOVER" 	// Enables the mode that ESP8266 can be found by WeChat.
#define ESP8266_CWSTOPDISCOVER 		"CWSTOPDISCOVER" 	// Disables the mode that ESP8266 can be found by WeChat.
#define ESP8266_WPS 				"WPS" 				// Sets the WPS function.
#define ESP8266_MDNS 				"MDNS" 				// Sets the MDNS function.
#define ESP8266_CWHOSTNAME 			"CWHOSTNAME" 		// Sets the host name of the ESP8266 Station.


/*************************************************************************************/
// TCP/IP-Related AT Commands
/*************************************************************************************/
#define ESP8266_CIPSTATUS			"CIPSTATUS"			// Gets the connection status
#define ESP8266_CIPDOMAIN			"CIPDOMAIN"			// DNS function
#define ESP8266_CIPSTART			"CIPSTART"			// Establishes TCP connection, UDP transmission or SSL connection
#define ESP8266_CIPSSLSIZE			"CIPSSLSIZE"		// Sets the size of SSL buffer
#define ESP8266_CIPSEND				"CIPSEND"			// Sends data
#define ESP8266_CIPSENDEX			"CIPSENDEX"			// Sends data when length of data is <length>, or when \0 appears in the data
#define ESP8266_CIPSENDBUF			"CIPSENDBUF"		// Writes data into TCP-send-buffer
#define ESP8266_CIPBUFRESET			"CIPBUFRESET"		// Resets the segment ID count
#define ESP8266_CIPBUFSTATUS 		"CIPBUFSTATUS" 		// Checks the status of TCP-send-buffer
#define ESP8266_CIPCHECKSEQ			"CIPCHECKSEQ"		// Checks if a specific segment is sent or not
#define ESP8266_CIPCLOSE			"CIPCLOSE"			// Closes TCP/UDP/SSL connection
#define ESP8266_CIFSR				"CIFSR"				// Gets the local IP address
#define ESP8266_CIPMUX				"CIPMUX"			// Configures the multiple connections mode
#define ESP8266_CIPSERVER			"CIPSERVER"			// Deletes/Creates a TCP server
#define ESP8266_CIPMODE				"CIPMODE"			// Configures the transmission mode
#define ESP8266_SAVETRANSLINK		"SAVETRANSLINK"		// Saves the transparent transmission link in the flash
#define ESP8266_CIPSTO				"CIPSTO"			// Sets timeout when ESP8266 runs as TCP server
#define ESP8266_PING				"PING"				// Ping packets
#define ESP8266_CIUPDATE			"CIUPDATE"			// Upgrades the software through network
#define ESP8266_CIPDINFO			"CIPDINFO"			// Shows remote IP and remote port with +IPD
#define ESP8266_CIPSNTPCFG			"CIPSNTPCFG"		// Configures the time domain and SNTP server.
#define ESP8266_CIPSNTPTIME			"CIPSNTPTIME"		// Queries the SNTP time.
#define ESP8266_CIPDNS_CUR			"CIPDNS_CUR"		// Sets user-defined DNS servers; configuration not saved in the flash
#define ESP8266_CIPDNS_DEF			"CIPDNS_DEF"		// Sets user-defined DNS servers; configuration saved in the flash


/*************************************************************************************/
// ESP8266 Responses
/*************************************************************************************/
#define ESP8266_RESPONSE_FAIL		"FAIL"
#define ESP8266_RESPONSE_ERROR		"ERROR"
#define ESP8266_RESPONSE_OK			"OK"
#define ESP8266_RESPONSE_SUCCESS	"SUCCESS"
#define ESP8266_RESPONSE_CONNECTED	"ALREADY CONNECT"		// Used with connection commands
#define ESP8266_RESPONSE_READY		"ready"
#define ESP8266_RESPONSE_CRLF		ESP8266_AT_TERMINATE


/*************************************************************************************/
// Enums and Structs
/*************************************************************************************/
typedef enum esp8266_cmd_rsp {
	ESP8266_CMD_BAD = -5,
	ESP8266_RSP_MEMORY_ERR = -4,
	ESP8266_RSP_FAIL = -3,
	ESP8266_RSP_UNKNOWN = -2,
	ESP8266_RSP_TIMEOUT = -1,
	ESP8266_RSP_SUCCESS = 0
};

typedef enum esp8266_wifi_mode {
	ESP8266_MODE_STA = 1,
	ESP8266_MODE_AP = 2,
	ESP8266_MODE_STAAP = 3
};


typedef enum esp8266_command_type {
	ESP8266_CMD_TEST,			// Test Command AT+<x>=? Queries the Set Commands’ internal parameters and their range of values.
	ESP8266_CMD_QUERY,			// Query Command AT+<x>? Returns the current value of parameters.
	ESP8266_CMD_SETUP,			// Set Command AT+<x>=<…> Sets the value of user-defined parameters in commands, and runs these commands.
	ESP8266_CMD_EXECUTE			// Execute Command AT+<x> Runs commands with no user-defined parameters.
};

typedef enum esp8266_encryption {
	ESP8266_ECN_OPEN,
	ESP8266_ECN_WPA_PSK,
	ESP8266_ECN_WPA2_PSK,
	ESP8266_ECN_WPA_WPA2_PSK
};

typedef enum esp8266_connect_status {
	ESP8266_STATUS_GOTIP = 2,
	ESP8266_STATUS_CONNECTED = 3,
	ESP8266_STATUS_DISCONNECTED = 4,
	ESP8266_STATUS_NOWIFI = 5	
};

typedef enum esp8266_serial_port {
	ESP8266_SOFTWARE_SERIAL,
	ESP8266_HARDWARE_SERIAL
};

typedef enum esp8266_socket_state {
	AVAILABLE = 0,
	TAKEN = 1,
};

typedef enum esp8266_connection_type {
	ESP8266_TCP,
	ESP8266_UDP,
	ESP8266_TYPE_UNDEFINED
};

typedef enum esp8266_tetype {
	ESP8266_CLIENT,
	ESP8266_SERVER
};


/*************************************************************************************/

/*************************************************************************************/
struct esp8266_ipstatus
{
	uint8_t linkID;
	esp8266_connection_type type;
	IPAddress remoteIP;
	uint16_t port;
	esp8266_tetype tetype;
};

struct esp8266_status
{
	esp8266_connect_status stat;
	esp8266_ipstatus ipstatus[ESP8266_MAX_SOCK_NUM];
};



/*************************************************************************************/
/* Different 'struct' approach to AT command interface
/*************************************************************************************/
struct {
	char commandString[16];
	esp8266_command_type commandType;
	char commandResponseString[16];
	esp8266_cmd_rsp commandResponseCode;
	uint32_t commandResponseTimeout;
} const AT_command_outline[] = {
	{"AT", ESP8266_CMD_EXECUTE, "OK", ESP8266_RSP_SUCCESS, 0},
	{"AT+", ESP8266_CMD_EXECUTE, "OK", ESP8266_RSP_SUCCESS, 0},
	{"RST", ESP8266_CMD_EXECUTE, "OK", ESP8266_RSP_SUCCESS, 0},
	{"GMR", ESP8266_CMD_EXECUTE, "OK", ESP8266_RSP_SUCCESS, 0},
	{"GSLP", ESP8266_CMD_EXECUTE, "OK", ESP8266_RSP_SUCCESS, 0},
	{"ATE", ESP8266_CMD_EXECUTE, "OK", ESP8266_RSP_SUCCESS, 0}
};

#endif	// _ESP8266_H_
