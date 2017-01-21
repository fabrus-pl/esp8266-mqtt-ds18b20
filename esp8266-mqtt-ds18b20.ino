
/* 
  Name: ESP8266-mqtt-ds18b20
  Date: 20 Jan 2017
  This SW is based on MqqtTimer by Godfather from esp8266.com
  Copyright (c) 2016 Godfather
  Copyright (c) 2015 Frank Parisi. All rights reserved.
  Copyright (c) 2015 John Lassen. All rights reserved.
  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Latest version 2.0.0 - 2017-01-21
  Make compatible with PubSubClient v 2.4, some refactoring and clean up
  -----------------------------------------------------------------------------------------------
  History

  Version: 1.1.3  - 2015-07-20
  Changed the loading of the Javascript and CCS Files, so that they will successively loaded and that only one request goes to the ESP.

  Version: 1.1.2  - 2015-07-17
  Added URLDECODE for some input-fields (SSID, PASSWORD...)

  Version  1.1.1 - 2015-07-12
  First initial version to the public  
  */
  
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include "conversions.h"
#include "globals.h"
#include "config.h"
#include "MQTTHandler.h"
#include "connections.h"
#include "webserver.h"

void setup ( void ) 
{
	initialize();

	setupConfig();
	setupMQTT();

    startNetwork();
	startWebServer();
	startNTPClient();
	startTimers();

	delay(500);
}


void loop () 
{
	handleAdminMode();
	handleWebServer();
	handleNTPUpdate();
	handleTurnOnAndOff();
	handleMQTTConnection();
	handleRefresh();
}




