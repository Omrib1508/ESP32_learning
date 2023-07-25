/********************************************************************
* @name     WiFiNetwork.h
* @brief    This file is the library of WiFi network functions
* @author   Omri Ben-Shahar Dgany
* @date     19/07/2023
*********************************************************************/
#ifndef __WIFINETWORK_H__
#define __WIFINETWORK_H__

/////////////////////////////////////////////////////////////////////
///                             INCLUDES                          ///
/////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <WiFi.h>
/////////////////////////////////////////////////////////////////////
///                             DEFINES                           ///
/////////////////////////////////////////////////////////////////////
#define SEARCHNETOWRKSTIMEOUT       10000
#define CLIENTCONNECTIONTIMEOUT     10000

WiFiServer    server(80); // Set web server port number to 80

/////////////////////////////////////////////////////////////////////
///                             FUNCTIONS                         ///
/////////////////////////////////////////////////////////////////////
/********************************************************************
* Scan for all the WiFi network in the area
*
* @param scanIteration: the number of iteration for scanning
* @return void
********************************************************************/
void scanNetworks(int scanIteration);

/********************************************************************
* Initiallize a network station with ssid and password
*
* @param ssid: the network name you want to connect
* @param password: passoword for the network
*
* @return the network connection status
********************************************************************/
wl_status_t initNetworkStation(const char* ssid, const char* password);

/////////////////////////////////////////////////////////////////////
///                             CALLBACKS                         ///
/////////////////////////////////////////////////////////////////////

#endif /* __WIFINETWORK_H__ */