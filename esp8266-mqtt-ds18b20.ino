
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

  Latest version 2.0.0
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
#include "MQTTHandler.h"
#include "connections.h"
/*
Include the HTML, STYLE and Script "Pages"
*/
#include "Page_Root.h"
#include "Page_Admin.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_NTPSettings.h"
#include "Page_Information.h"
#include "Page_General.h"
#include "PAGE_NetworkConfiguration.h"
#include "PAGE_MQTTConfiguration.h"


#define ACCESS_POINT_NAME  "ESP"
#define ACCESS_POINT_PASSWORD  "12345678" 
#define AdminTimeOut 180                                   // Defines the Time in Seconds, when the Admin-Mode will be disabled

void setup ( void ) 
  {
  EEPROM.begin(512);
  Serial.begin(115200);
  delay(500);
  Serial.println("");
  Serial.println("Starting esp8266");
  Serial.println("");
  pinMode(LED,OUTPUT);                                    // Set GPIO LED pin as device output device
  delay(10);
 

  if (!ReadConfig())
  {
    // DEFAULT CONFIG
    config.WiFissid = "MYSSID";
    config.WiFipass = "MYPASSWORD";
    config.dhcp = true;
    config.IP[0] = 192;config.IP[1] = 168;config.IP[2] = 1;config.IP[3] = 100;
    config.Netmask[0] = 255;config.Netmask[1] = 255;config.Netmask[2] = 255;config.Netmask[3] = 0;
    config.Gateway[0] = 192;config.Gateway[1] = 168;config.Gateway[2] = 1;config.Gateway[3] = 1;
    config.ntpServerName = "0.au.pool.ntp.org";
    config.Update_Time_Via_NTP_Every =  0;
    config.timezone = 0;
    config.daylight = true;
    config.DeviceName = "Not Named";
    config.AutoTurnOff = false;
    config.AutoTurnOn = false;
    config.TurnOffHour = 0;
    config.TurnOffMinute = 0;
    config.TurnOnHour = 0;
    config.TurnOnMinute = 0;
    config.MQTTBroker = "MYBROKER";                       // MQTT broker parameters URL
    config.MQTTport = 0;                                  // MQTT port
    config.MQTTuser = "MYMQTTUSER";                       // MQTT username
    config.MQTTpass = "MYMQTTPASS";                       // MQTT password
    config.clientID = "MYID";                             // MQTT client ID - this device
    WriteConfig();
    Serial.println("General config applied");
  }
  
  client.setCallback(callback);                          // Complete PubSubClient initalisation after client instance creation in globals
  client.setServer(config.MQTTBroker.c_str(), config.MQTTport);
  
  
  if (AdminEnabled)
    {
    WiFi.mode(WIFI_AP_STA);                               // If AdminEnabled = true, start as WiFi access point so we can connect to it
    WiFi.softAP( ACCESS_POINT_NAME , ACCESS_POINT_PASSWORD);
    Serial.println("Admin mode enabled");
    }
      else
      {
      Serial.println("Starting WiFi");                    // If AdminEnable = false, start as a WiFi client
      WiFi.mode(WIFI_STA);
      ConfigureWifi();                                    // Configure WiFi client
      delay(2000);
      }
  
  if (!AdminEnabled)                                      // If AdminEnable = false we are starting as a WiFi client so connect to an MQTT Broker
    {
    ConnectMQTT();
    }

  // Start web server
  
  server.on ( "/admin.html", []() { Serial.println("admin.html"); server.send ( 200, "text/html", reinterpret_cast<const __FlashStringHelper *>(PAGE_AdminMainPage ));   }  );
  server.on ( "/config.html", send_network_configuration_html );
  server.on ( "/mqttconfig.html", send_mqtt_configuration_html );
  server.on ( "/info.html", []() { Serial.println("info.html"); server.send ( 200, "text/html", reinterpret_cast<const __FlashStringHelper *>(PAGE_Information ));   }  );
  server.on ( "/ntp.html", send_NTP_configuration_html  );
  server.on ( "/general.html", send_general_html  );
  server.on ( "/style.css", []() { Serial.println("style.css"); server.send ( 200, "text/plain", reinterpret_cast<const __FlashStringHelper *>(PAGE_Style_css ));  } );
  server.on ( "/microajax.js", []() { Serial.println("microajax.js"); server.send ( 200, "text/plain", reinterpret_cast<const __FlashStringHelper *>(PAGE_microajax_js ));  } );
  server.on ( "/admin/values", send_network_configuration_values_html );
  server.on ( "/admin/mqttvalues", send_mqtt_configuration_values_html );
  server.on ( "/admin/connectionstate", send_connection_state_values_html );
  server.on ( "/admin/infovalues", send_information_values_html );
  server.on ( "/admin/ntpvalues", send_NTP_configuration_values_html );
  server.on ( "/admin/generalvalues", send_general_configuration_values_html);
  server.on ( "/admin/devicename",     send_devicename_value_html);
  server.onNotFound ( []() { Serial.println("Page Not Found"); server.send ( 400, "text/html", "Page not Found" );   }  );
  server.begin();
  Serial.println( "HTTP server started" );
  Serial.println("");
  
  tkSecond.attach(1,Second_Tick);                 // Start internal timer
  
  Serial.print("Starting UDP ");
  UDPNTPClient.begin(localNtpPort);               // Start UDPNTP client
  Serial.print("Client started local port is: "); 
  Serial.println(UDPNTPClient.localPort());
  delay(500);
  } 


void loop () 
{
  server.handleClient();
  
  if (AdminEnabled)
    {
      if (AdminTimeOutCounter > AdminTimeOut)
        {
        AdminEnabled = false;                                   // Start up WiFi and MQTT after Admin timeout expires.
        Serial.println("Admin Mode disabled!");
        WiFi.mode(WIFI_STA);
        delay(2000);
        ConnectMQTT();
        delay(1000);
        }
    }
    
  if (config.Update_Time_Via_NTP_Every  > 0 )
    {
    if (cNTP_Update > 5 && firstStart)
      {
      NTPRefresh();
      cNTP_Update =0;
      firstStart = false;
      }
    else if ( cNTP_Update > (config.Update_Time_Via_NTP_Every * 60) )
      {
      NTPRefresh();
      cNTP_Update =0;
      }
    }

                       

  if (DateTime.hour != 0 && DateTime.minute != 0 && (cb))      //Dont turn on anything if set time = 00:00 and we dont have NTP time at power up
    {
   
      if(DateTime.minute != Minute_Old)
          {
          Minute_Old = DateTime.minute;                                                               // Turn on device if AutoTurn on enabled and time set = NTP time
          if (config.AutoTurnOn)
          {
           if (DateTime.hour == config.TurnOnHour && DateTime.minute == config.TurnOnMinute)          
              {
              devStat = true;                                                                         // Set status to indicate that we are under timer mode
              digitalWrite(LED,LOW);
              Serial.println("Device On");
              client.publish("feedback", "Device <ON>");
              }
          }
    
         Minute_Old = DateTime.minute;                                                                // Turn off device if AutoTurn off enabled and time set = NTP time
         if (config.AutoTurnOff)
            {
            if (DateTime.hour == config.TurnOffHour && DateTime.minute == config.TurnOffMinute)     
              {
              devStat = false;                                                                        // Set status to indicate that we have finished timer mode
              digitalWrite(LED,HIGH);
              Serial.println("Device Off");
              client.publish("feedback", "Device <OFF>");
              }
            }
          }   
    }
 
   if (BypassOn)                                                        // Turn on device if Bypass enabled
      {
      digitalWrite(LED,LOW);
      }
      else if ((!BypassOn) && (devStat))                                // Turn on device if Bypass disabled as timer activated
      {
      digitalWrite(LED,LOW); 
      }
      else if ((!BypassOn) && (!devStat))                               // Turn off device if Bypass disabled and timer deactivated
      {
      digitalWrite(LED,HIGH);
      }

   if ((config.AutoTurnOff==false) && (!BypassOn))                      // Turn off if AutoTurnOff is disabled and Bypass disabled     
      {                                                                 // forces device to turn off in timer mode, cannot be activated again by eanbling AutoTurnoff, use bypass if you wish to turn device on  
        devStat = false;                                                
      }   
     
   if ((DateTime.hour == config.TurnOffHour && DateTime.minute == config.TurnOffMinute) && (BypassOn)) // turn off device as off time reached (just in case we left Bypass on by mistake!)
      {
      digitalWrite(LED, HIGH);                                           
      BypassOn = false;                                                  
      }

  
  if (!AdminEnabled)
  {
  checkConnection();                                                    // Check if we are still connected to an MQTT broker
  client.loop();                                                        // Check MQTT for incomming messages
  
   
  
  //if (Refresh)                                                        // Put other items you want checked here GPIO etc., as refresh runs every second
    //{
     //Refresh = false;
    //}    
  }
}




