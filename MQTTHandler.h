#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H


// handle incoming messages

void callback(char* topic_str, byte* payload_str, unsigned int length)
{
	String topic = String(topic_str);
	String payload = String((char*)payload_str);

    Serial.print("Received from Topic: "); Serial.print(topic); Serial.print("=> "); Serial.print(payload); Serial.print("  Length: "); Serial.println(length,DEC);
   
    if ((payload.equalsIgnoreCase("on:")))
    {
    BypassOn = true;
    client.publish("feedback", "Device <ON>");
    }
  
      else if ((payload.equalsIgnoreCase("off:")))
        {
        BypassOn = false;
        client.publish("feedback", "Device <OFF>");
        }


      else if ((payload.equalsIgnoreCase("time:")))
        {
        char timeBuff[25];
        sprintf(timeBuff, "%02d:%02d - Date is %02d/%02d/%02d", DateTime.hour,DateTime.minute,DateTime.day,DateTime.month,DateTime.year);
        String MQTTTime = "Time is: ";
        MQTTTime.concat(timeBuff);
        client.publish("feedback", MQTTTime.c_str());
        Serial.println("feedback Device Current Time is " + MQTTTime);
        }

      else if((payload.startsWith("on time:")))
        {
        String OnTime = payload;
            if (OnTime.length()==14) 
            {
            OnTime.remove(0,9);
            Serial.print("Device on time is: "); Serial.println(OnTime);
            config.TurnOnHour = OnTime.substring(0,3).toInt();
            config.TurnOnMinute = OnTime.substring(3,5).toInt();
            char minBuff[5];                                                                // All this just to print leading zero's!
            sprintf(minBuff, "%02d:%02d", config.TurnOnHour, config.TurnOnMinute);
            String pubOnTime ="On time set to: ";
            		pubOnTime.concat(minBuff);
            client.publish("feedback",pubOnTime.c_str());
            WriteConfig();
            }
             else 
               {
               Serial.println("");
               Serial.println("Time must be entered in the following format:- on time: 12:00 or on time: 06:30 etc.");
               client.publish("feedback", "Incorrectly formatted message...!");
               }
         }

      else if((payload.startsWith("off time:")))
        {
        String OffTime = payload;
          if (OffTime.length()==15) 
            {
            OffTime.remove(0,10);
            Serial.print("Device off time is: "); Serial.println(OffTime);
            config.TurnOffHour = OffTime.substring(0,3).toInt();
            config.TurnOffMinute = OffTime.substring(3,5).toInt();
            char minBuff[5];                                                                  // All this just to print leading zero's!
            sprintf(minBuff, "%02d:%02d",config.TurnOffHour, config.TurnOffMinute);
            String pubOffTime = "Off time set to: ";
            pubOffTime.concat(minBuff);
            client.publish("feedback",pubOffTime.c_str());
            WriteConfig();
            }
              else 
              {
               Serial.println("");
               Serial.println("Time must be entered in the following format:- off time: 12:00 or off time: 06:30 etc.");
               client.publish("feedback", "Incorrectly formatted message...!");
              }
         }


      else if((payload.startsWith("auto turn on:")))
        {
        String MQTTAutoOffOn = payload;
        MQTTAutoOffOn.remove(0,14);
           if (MQTTAutoOffOn == "enable")
              {
              config.AutoTurnOn = true;
              Serial.println("Device automatic turn on enabled: ");
              client.publish("feedback", "Automatic turn on is enabled");
              WriteConfig();
              }
           else if (MQTTAutoOffOn == "disable")
              {
               config.AutoTurnOn = false; 
               Serial.println("Device automatic turn on disabled: ");
               client.publish("feedback", "Automatic turn on is disabled");
               WriteConfig();
              }           
                else
                 {
                 Serial.println("");
                 Serial.println("Time must be entered in the following format:- auto turn on: enable or auto turn on: disable.");
                 String errorString="?? ";
                 errorString.concat(MQTTAutoOffOn);
                 client.publish("feedback",  + errorString.c_str());
                 }
         }

      else if((payload.startsWith("auto turn off:")))
        {
        String MQTTAutoOffOn = payload;
        MQTTAutoOffOn.remove(0,15);
           if (MQTTAutoOffOn == "enable")
              {
              config.AutoTurnOff = true;
              Serial.println("Device automatic turn off enabled: ");
              client.publish("feedback", "Automatic turn off is enabled");
              WriteConfig();
              }
           else if (MQTTAutoOffOn == "disable")
              {
               config.AutoTurnOff = false; 
               Serial.println("Device automatic turn off disabled: ");
               client.publish("feedback", "Automatic turn off is disabled");
               WriteConfig();
              }           
                else
                  {
                 Serial.println("");
                 Serial.println("Time must be entered in the following format:- auto turn off: enable or auto turn off: disable.");
                 String errorString="?? ";
                 errorString.concat(MQTTAutoOffOn);
                 client.publish("feedback", errorString.c_str());
                 }
         }


     else if ((payload.equalsIgnoreCase("status:")))
       {
        String statTurnOn;
        String statTurnOff;
        String bypassStat;
        String timerStat;
        if (config.AutoTurnOn)
            {
            statTurnOn = "Auto Turn on is: Enabled";
            } 
            else
            {
            statTurnOn = "Auto Turn on is: Disabled";
            }
  
        if (config.AutoTurnOff)
            {
            statTurnOff = "Auto Turn 0ff is: Enabled";
            }
            else
            {
            statTurnOff = "Auto Turn 0ff is: Disabled";
            }
        if (BypassOn)
            {
            bypassStat = "Bypass is: Enabled";
            }
            else
            {
            bypassStat = "Bypass is: Disabled";
            }
        if (devStat)
            {
            timerStat = "Timer status is: Enabled";
            }
            else 
            {
            timerStat = "Timer status is: Disabled";
            }
        char onBuff[5];
        sprintf(onBuff, "%02d:%02d", config.TurnOnHour, config.TurnOnMinute);
        String pubOnBuff = "Turn on time is: ";
        pubOnBuff.concat(onBuff);
        char offBuff[5];
        sprintf(offBuff, "%02d:%02d",config.TurnOffHour, config.TurnOffMinute);
        String pubOffBuff = "Turn off time is: ";
        pubOffBuff.concat(offBuff);
        client.publish("feedback", "Current setup status: ");
        delay(100);
        client.publish("feedback", statTurnOn.c_str());
        delay(100);
        client.publish("feedback", statTurnOff.c_str());
        delay(100);
        client.publish("feedback", pubOnBuff.c_str());
        delay(100);
        client.publish("feedback",  + pubOffBuff.c_str());
        delay(100);
        client.publish("feedback", bypassStat.c_str());
        delay(100);
        client.publish("feedback", timerStat.c_str());
      }
    else  
    {
    String errorString = "?? ";
    errorString.concat(payload);
    client.publish("feedback", errorString.c_str());
    Serial.print(" \tsent: feedback=> ???? '"); Serial.print(payload); Serial.println("'");
    }
}

void setupMQTT(void)
{
	client.setCallback(callback);                          // Complete PubSubClient initalisation after client instance creation in globals
	client.setServer(config.MQTTBroker.c_str(), config.MQTTport);
}

void ConnectMQTT()
 {
   if (WiFi.status() == WL_CONNECTED)
    {
    Serial.println("");
    Serial.print("Connecting to MQTT Broker:"); //Serial.print(config.MQTTBroker);
    int retries = 4;
      while (!client.connect(config.clientID.c_str(), config.MQTTuser.c_str(), config.MQTTpass.c_str()) && retries-- )
      {
      delay(500);
      Serial.print(".");
      }
        if(client.connected())                   // Connected to MQTT server
        {
        Serial.println("");
        Serial.println("");
        Serial.print("MQTT Client: ");
        Serial.print(config.clientID);
        Serial.print(": is connected to MQTT server: ");
        Serial.println(config.MQTTBroker);
        client.publish("feedback", "hello from esp8266");
        Serial.println("");
        client.subscribe("device");
        Serial.println("Subscribed to: 'device'");
        Serial.println("");
        }
        else
        {
        Serial.println("\nfatal: MQTT server Connection failed");
        }
     }
 }

#endif      
    
