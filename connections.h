#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#define ACCESS_POINT_NAME  "ESP"
#define ACCESS_POINT_PASSWORD  "12345678"

void checkConnection()  
{
	if (!AdminEnabled && (WiFi.status() == WL_CONNECTED) )
	{
		static long previousMillis = 0;
		long currentMillis = millis();

		if(currentMillis - previousMillis > connectionCheckInterval)
		{
			previousMillis = currentMillis;
			if (client.connected())
			{
				Serial.println("MQTT connection OK");
			}
			else
			{
				Serial.println("MQTT NOT connected! - Starting MQTT Client. ");
				ConnectMQTT();
				//UDPNTPClient.begin(localNtpPort);                                     // May need to uncomment this if having trouble with NTP starting up again after a failure
			}
		}
	}
}

void handleMQTTConnection (void)
{
	if (!AdminEnabled)
	{
		checkConnection();                                                    // Check if we are still connected to an MQTT broker
		client.loop();                                                        // Check MQTT for incomming messages
	}
}

void startNetwork(void)
{
	Serial.println("Starting WiFi");
	if (AdminEnabled || !ConfigureWifi())
	{
		AdminEnabled = true;
		WiFi.mode(WIFI_AP_STA);
		delay(1000);
		WiFi.softAP( ACCESS_POINT_NAME , ACCESS_POINT_PASSWORD);
		Serial.println("Admin mode enabled");
	}
	else
	{
		delay(3000);
		ConnectMQTT();
	}

}
void startNTPClient(void)
{
	Serial.print("Starting NTP ");
	UDPNTPClient.begin(localNtpPort);
	Serial.print("Client started local port is: ");
	Serial.println(UDPNTPClient.localPort());
}

void NTPRefresh()
  {
  if (WiFi.status() == WL_CONNECTED)
    {
      IPAddress timeServerIP;
      WiFi.hostByName(config.ntpServerName, timeServerIP);
      Serial.println("");
      Serial.print("sending NTP packet to  "); Serial.print(timeServerIP);
      memset(packetBuffer, 0, NTP_PACKET_SIZE);
      packetBuffer[0] = 0b11100011;                 // LI, Version, Mode
      packetBuffer[1] = 0;                          // Stratum, or type of clock
      packetBuffer[2] = 6;                          // Polling Interval
      packetBuffer[3] = 0xEC;                       // Peer Clock Precision
      packetBuffer[12]  = 49;
      packetBuffer[13]  = 0x4E;
      packetBuffer[14]  = 49;
      packetBuffer[15]  = 52;
      int retries = 5;
       do
        {
        UDPNTPClient.beginPacket(timeServerIP, 123);
        UDPNTPClient.write(packetBuffer, NTP_PACKET_SIZE);
        UDPNTPClient.endPacket();
        delay(1000);
        Serial.print(".");
        cb = UDPNTPClient.parsePacket();
        if (!cb)                                            // I dont know why but sometimes we have to restart UDPNTP again and re-write the port
            {
            UDPNTPClient.begin(localNtpPort);
            }
        } while (!cb && retries--);
          if (!cb)
            {
            Serial.println("");
            Serial.println("Unable to get NTP packet.....!");
            }
        else
        {
        Serial.println("");
        Serial.print("NTP packet received, length = ");
        Serial.println(cb);
        UDPNTPClient.read(packetBuffer, NTP_PACKET_SIZE);                   // read the packet into the buffer
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        const unsigned long seventyYears = 2208988800UL;
        unsigned long epoch = secsSince1900 - seventyYears;
        UnixTimestamp = epoch;
        }
     }
}

void handleNTPUpdate(void)
{
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
}

#endif
